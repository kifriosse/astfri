using System.Reflection;
using System.Runtime.InteropServices;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Text.RegularExpressions;
using CommandLine;
using CommandLine.Text;

namespace CSharpTypeExporter;

internal enum FrameworkProfile {
    Core,
    Aspnet,
    WinForms,
    Wpf
}

internal enum DeclType {
    Class,
    Struct,
    Interface,
    Enum,
    RecordClass,
    RecordStruct,
    Delegate,
    Unknow
}

internal class Options {
    [Option(
        'i', 
        "input",
        SetName = "File",
        HelpText = "Input DLL file. If not present will use standard library DLL"
    )]
    public string? InputDllFile { get; set; }

    [Option(
        "dependency-dir",
        SetName = "File",
        HelpText = "Directory for DLL files the Input DLL is dependant on (usually located in bin folder of C# project)"
    )]
    public string? DependencyDir { get; set; }

    [Option(
        'd',
        "dll-dir",
        SetName = "Directory",
        HelpText = "Directory containing DLL files to export"
    )]
    public string? InputDllDir { get; set; }

    [Option(
        'e',
        "exclude",
        SetName = "Directory",
        HelpText = "List of exclusion patterns"
    )]
    public IEnumerable<string> Excludes { get; set; } = ["Microsoft.Extensions.*"];

    [Option(
        'p',
        "profile",
        Default = FrameworkProfile.Core,
        HelpText = "Choose a preset profile. When combined with input arguments will be used as a dependency"
    )]
    public FrameworkProfile Profile { get; set; }

    [Option(
        'o',
        "output",
        Required = true,
        HelpText = "Output file for the Json"
    )]
    public required string OutputPath { get; set; }
}

internal record struct TypeMetadata(string Name, string Nms, int Arity, DeclType Type);

internal static class Program {
    
    private static readonly JsonSerializerOptions JsonOptions = new() {
        WriteIndented = true,
        Converters = { new JsonStringEnumConverter() }
    };

    private static ParserResult<Options>? _result;
    
    public static void Main(string[] args) {
        _result = Parser.Default.ParseArguments<Options>(args);
        _result.WithParsed(Main);
    }

    private static void Main(Options opt) {
        IEnumerable<TypeMetadata> metadata = GetTypeMetadata(opt);
        
        string? directoryPath = Path.GetDirectoryName(opt.OutputPath);
        if (!string.IsNullOrWhiteSpace(directoryPath)) {
            Directory.CreateDirectory(directoryPath);
        }
        using FileStream stream = File.Create(opt.OutputPath);
        JsonSerializer.Serialize(stream, metadata, JsonOptions);
    }

    private static IEnumerable<TypeMetadata> GetTypeMetadata(Options opt) {
        Assembly coreAssembly = typeof(object).Assembly;
        
        string coreDir = Path.GetDirectoryName(coreAssembly.Location)!;
        string version = Path.GetFileName(coreDir);
        string sharedRoot = Path.GetFullPath(Path.Combine(coreDir, "..", ".."));
        string folderName = opt.Profile switch {
            FrameworkProfile.Aspnet => "Microsoft.AspNetCore.App",
            FrameworkProfile.Wpf or FrameworkProfile.WinForms => "Microsoft.WindowsDesktop.App",
            _ => "Microsoft.NETCore.App"
        };
        string frameworkPath = Path.GetFullPath(Path.Combine(sharedRoot, folderName, version));
        
        if (!Directory.Exists(frameworkPath)) {
            Console.Error.WriteLine($"Error: Framework path \"{frameworkPath}\" doesn't exist on this machine " +
                                    "or framework is not installed in default directory");
            if (
                opt.Profile is FrameworkProfile.Wpf or FrameworkProfile.WinForms &&
                !RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
            ) {
                Console.Error.WriteLine("Note: WPF and WinForms can be exported only on Windows machine");
            }

            Environment.Exit(1);
        }

        List<string> allDlls = [];
        List<string>? exportDlls = null;
        List<string> frameworkDlls = GetAllDlls(frameworkPath).ToList();
        allDlls.AddRange(frameworkDlls);
        
        if (opt.Profile is not FrameworkProfile.Core)
            allDlls.AddRange(GetAllDlls(coreDir));
        
        if (! string.IsNullOrWhiteSpace(opt.InputDllFile) && ! string.IsNullOrWhiteSpace(opt.DependencyDir)) {
            exportDlls = [opt.InputDllFile];
            allDlls.Add(opt.InputDllFile);
            allDlls.AddRange(GetAllDlls(opt.DependencyDir));
        }
        else if (! string.IsNullOrWhiteSpace(opt.InputDllDir)) {
            List<string> fileContent = GetAllDlls(opt.InputDllDir).ToList();
            allDlls.AddRange(fileContent);
            Regex exclude = BuildFilter(opt.Excludes);
            exportDlls = fileContent
                .Where(path => ! exclude.IsMatch(Path.GetFileName(path)))
                .ToList();
        }
        else if (
            string.IsNullOrWhiteSpace(opt.InputDllFile) &&
            string.IsNullOrWhiteSpace(opt.DependencyDir) &&
            string.IsNullOrWhiteSpace(opt.InputDllDir)
        ) {
            exportDlls = frameworkDlls.ToList();
        }
        else if (opt.DependencyDir is null) {
            HelpText? helpText = HelpText.AutoBuild(_result, h => h, e => e);
            Console.WriteLine("Error: --dependency-dir is required when using -i");
            Console.WriteLine(helpText);
            Environment.Exit(1);
        }
        else if (opt.InputDllFile is null) {
            HelpText? helpText = HelpText.AutoBuild(_result, h => h, e => e);
            Console.WriteLine("Error: -i is required when using --dependency-dir");
            Console.WriteLine(helpText);
            Environment.Exit(1);
        }
        
        PathAssemblyResolver resolver = new(allDlls);
        using MetadataLoadContext context = new(resolver);
        
        foreach (string dllPath in exportDlls!) {
            Assembly assembly = context.LoadFromAssemblyPath(dllPath);
            IEnumerable<TypeMetadata?> types = assembly
                .GetExportedTypes()
                .Where(t => t.IsPublic || t.IsNestedPublic)
                .Select(MakeMetadata)
                .Where(tm => tm is not null);
            
            foreach (TypeMetadata? metadata in types)
                yield return metadata!.Value;
        }
    }

    private static void AddOuterTypes(ref string scope, Type type) {
        Type? current = type.DeclaringType;
        while (current is not null) {
            scope += ".";
            scope += RemoveArity(current.Name);
            current = current.DeclaringType;
        }
    }


    private static TypeMetadata? MakeMetadata(Type type) {
        if (type.FullName is null || type.Namespace is null)
            return null;

        string nms = type.Namespace;
        AddOuterTypes(ref nms, type);

        return new TypeMetadata {
            Name = RemoveArity(type.Name),
            Nms = nms,
            Arity = type.GetGenericArguments().Length,
            Type = GetDeclType(type)
        };
    }
    
    private static string RemoveArity(string name) {
        int index = name.IndexOf('`');
        return index < 0 ? name : name[..index];
    }
    
    private static DeclType GetDeclType(Type type) {
        if (type.IsInterface) return DeclType.Interface;
        if (type.IsEnum) return DeclType.Enum;
        if (type is { IsValueType: true, IsPrimitive: false }) return DeclType.Struct;

        bool isRecord = type.GetMethods().Any(m => m.Name == "<Clone>$");
        if (typeof(Delegate).IsAssignableFrom(type)) 
            return isRecord ? DeclType.RecordStruct : DeclType.Delegate;
        if (type.IsClass) 
            return isRecord ? DeclType.RecordClass : DeclType.Class;

        return DeclType.Unknow;
    }

    private static Regex BuildFilter(IEnumerable<string> globPatterns) {
        IEnumerable<string> regexParts = globPatterns.Select(glob => {
            string pattern = Regex.Escape(glob).Replace(@"\*", ".*").Replace(@"\*", ".");
            return $"({pattern})";
        });

        return new Regex($"{string.Join("|", regexParts)}^");
    }

    private static IEnumerable<string> GetAllDlls(string dir) {
        const string dllExtension = ".dll";
        IEnumerable<string> files = Directory.GetFiles(dir);
        return files.Where(f => Path.GetExtension(f).Equals(dllExtension));
    }
}