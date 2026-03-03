Library AST FRI for C\#
===

Requirements
---
- .NET SDK - if building manualy
- .NET Runtime - if running with executable
- When exporting .NET frameworks like ASP.NET Core (using `-p`/`--profile` argument)
  - framework needs to be installed on the system
  - should be installed in default location
    - if installed elsewhere, directory needs to be manualy specified with `-d`/`--dll-dir` argument
- WPF and WinForms can be exported using `-p`/`--profile` arguemnts only on Windows
  - on Linux and Mac OS (or other UNIX based operating systems) user needs to download DLLs manualy ([see here](#exporting-wpfwinforms-dlls-on-non-windows-os))

Depencency Instalation
---

### .NET
#### GNU/Linux
- SDK installation
```shell
sudo pacman -S dotnet-sdk # Arch based distributions
sudo apt-get update && sudo apt-get install -y dotnet-sdk-10.0 # Ubuntu/Debian based distributions
sudo dnf install dotnet-sdk-10.0 # Fedora/RHEL based distributions
sudo zypper install dotnet-sdk-10.0 # openSUSE
```
- Runtime installation
```shell
sudo pacman -S dotnet-runtime # Arch based distributions
sudo apt-get update && sudo apt-get install -y dotnet-runtime-10.0 # Ubuntu/Debian based distributions
sudo dnf install dotnet-runtime-10.0 # Fedora/RHEL based distributions
sudo zypper install dotnet-runtime-10.0 # openSUSE
```
#### Windows
**Using GUI:**
1. Go to the official [.NET Download Page](https://dotnet.microsoft.com/en-us/download/dotnet/10.0).
2. Download the SDK: Click the "x64" (or other CPU architecture depending on what you have) in Windows row and 
installer column under SDK section
   - Note: Installing the SDK automatically installs all the runtimes (Base, Desktop, and ASP.NET).
3. Run the .exe: Open the file, click Install, and confirm the Windows security prompt.
4. Finish: Click Close once it's done. No restart is usually required.

**Using CLI (winget):**
```powershell
winget install Microsoft.DotNet.SDK.10
```

Script Usage
---

### Argument Options
- `--help` - prints brief of all argument options
- Single DLL input:
  - `-i`/`--input` - input DLL file
  - `--dependency-dir` - directory containing other dependency DLLs (usualy in `bin` folder of a C# project)
  - `-p`/`--profile` - dependency framework for the input DLL (Default is .NET Core)
- DLL directory input:
  - `-d`/`--dll-dir` - input directory containing DLLs (usualy will be content of `bin` folder of a C# project)
  - `-e`/`--exclude` - list of exclution patterns. It can be file name or a 
  [glob](https://en.wikipedia.org/wiki/Glob_(programming)) pattern (only supports `*` and `?` pattern)
    - Default pattern pattern is `Microsoft.Extensions.*` (Note: if any pattern is defined this pattern will be 
    overwriten)
  - `-p`/`--profile` - dependency framework for the input DLL (Default is .NET Core)
- Exporting whole C# standard framework (APS.NET, WPF, WinForms)
  - `-p`/`--profile` - framework we want to export
- `-o`/`--output` - path to output export JSON

### Exporting WPF/WinForms DLLs on non-Windows OS

Download DLLs using one of these options:

### Direct download (`.zip`)
1. Go to the official [.NET Download Page](https://dotnet.microsoft.com/en-us/download/dotnet/10.0).
2. Download the SDK: Click the "x64" (or other CPU architecture depending on what you have) in Windows row and
   binary column under SDK section
3. Download and extract it
```shell
mkdir windows_desktop_libs
tar -xf dotnet-runtime-[x.y.z]-win-[architecture].tar.gz -C windows_desktop_libs
```
- replace
    - `[x.y.z]` with downloaded version
    - `[architecture]` with chosen architecture
4. DLLs are located in `shared/Microsoft.WindowsDesktop.App/[.NET version]/`
- replace `[.NET version]` with downloaded version

### Using `dotnet` CLI
```shell
mkdir temp_ref && cd temp_ref 
dotnet new console # creates temporary project
dotnet add package Microsoft.WindowsDesktop.App.Ref --version [version]
dotnet restore
cd ..
rm -rf temp_ref # removes temporary project
```
- DLLs will be located in `~/.nuget/packages/microsoft.windowsdesktop.app.ref/[.NET version]/ref/net[version]/`

### Using NuGet Download
1. Go to NuGet: [Microsoft.WindowsDesktop.App.Ref](https://www.nuget.org/packages/Microsoft.WindowsDesktop.App.Ref/).
2. Click Download package (in the right side-panel).
3. Right-click the file and "Open with Archive Manager" (or rename it to .zip).
4. Extract the ref/ folder inside the NuGet file, which contains the the DLLs.

After downloading the DLLs point the script to the downloaded folder which contains the framework DLLs using 
`-d`/`--dll-dir` argument



 
