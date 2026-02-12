// extern alias MyExternalAlias;

public delegate void MyDelegate(int x);

public record struct MyRecordStruct(int X, int Y);
public record MyRecordClass(string Name, int Age);


public class TestBase
{
    protected int baseField = 0;
    protected int shadowedField = 1;
    public virtual void VirtualMethod()
    {
        
    }

    public void BaseMethod()
    {
        
    }
}

public class Logger : StreamWriter
{
    private string _logPrefix;

    // Constructor passing a path to the base class constructor
    public Logger(string prefix, string path) : base(path)
    {
        _logPrefix = prefix;
    }

    // Overriding a method and calling the base implementation
    public override void Write(string? value)
    {
        string timestamp = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
        
        // Calling something in the base class (StreamWriter.Write)
        base.Write($"[{timestamp}] {_logPrefix}: ");
        base.Write(value);
    }

    public void CloseLogger()
    {
        // Calling another base class method
        Flush();
        Close();
    }
}

public class Test : TestBase
{
    protected new int shadowedField = 2;
    public override void VirtualMethod()
    {
        base.VirtualMethod();
    }

    public void TestMethod()
    {
        baseField = 10;
        base.shadowedField++;
        BaseMethod();
    }
    public void TestMethod(int x)
    {
        
    }

    public void TestMethod(string x)
    {
        x = x + "test";
    }
}

public partial class TestClass
{
    private readonly static int staticConst = 5;
    int variable = 10;

    Func<int, bool> func = (x) => true;

    public Vector2 Vector2 { get; set; }
    public int AutoProperty { get; set; }
    public int GetOnlyProp { get; }
    public int InitProp { get; init;}
    public int PrivateSetProp { get; private set; }
    public int PrivateInitProp { get; private init; }
    public int SetOnlyProp { set {} }
    public int AutoPropWithDefault { get; set; } = staticConst;
    public int ArrowProp => 42;
    public int PropertyWithBody
    {
        get { return variable; }
        private set { variable = value; }
    }

    public static void test(string x, int y)
    {
        x = x + y.ToString();
    }

    public int Item { get; set; }

    public int get_Item(int index)
    {
        return index * 2;
    }

    public unsafe void Main()
    {

        int value = 1;
        ref readonly int refValue = ref value;
        int* ptr = &value;
        int? nullableInt = null;
        ref int? refNullableInt = ref nullableInt;
        (string, float) tuple = ("STRING", 1.0f);
        
        List<int>? list = new List<int> { 1, 2, 3 };
        // list!.Add(4);
        
        delegate* managed <string, int, void> parsePointer = &TestClass.test;

        scoped Span<int> stackSpan = stackalloc int[10];
        Vector2.X = 0f;
        Vector2.Zero = new Vector2(0, 0);

        void test(int x)
        {
            
        }

        test(1);
        bool LocalFunc()=> true;


        CreateVector(1.0f, 2.0f);

        func(2);
        Func<bool> localFunc = LocalFunc;

        var vector = new Vector2(3.5f, 4.5f);
        string x = CreateVector(3.5f, 4.5f).X.ToString();
        float y = vector.Y;
        string z = localFunc().ToString();

        // string w = LocalFunc().ToString();
        System.Collections.Generic.List<int> l = new System.Collections.Generic.List<int>();
        Console.WriteLine($"Vector components: X = {x}, Y = {y}");
        System.Collections.Generic.IReadOnlyList<int>.Equals(list, list);

        ((Func<int, bool>)((x) => { return false; }))(1);
    //  ^   ^           ^          ^             ^
    //  |   |          |          |             |Lambda Call
    //  |   |          |          |Lambda Body
    //  |   |          |Lambda Parameters
    //  |   |Cast to a Delegate
    //  |Anonymous Lambda  
    }

    public static Vector2 CreateVector(float x = 0, float y = 0)
    {
        return new Vector2(x, y);
    }


    public void anotherTest()
    {
        
    }

    Action action;

    public void VariadicMethod(int x_p, params int[] numbers)
    {

    }

    public void RefParam(scoped ref int x)
    {
        x++;
    }

    public void ReadonlyPointerMethod(ref readonly int ptr)
    {
        int value = ptr;
    }
}