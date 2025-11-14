

using System.Threading.Tasks;

using Zoznam = System.Collections.Generic.List<string>;

namespace Test1;

// namespace CSharp.Test
// {
void test2()
{
    
}

public interface Intr : IComparable
{
    void Interfacing();
}

public partial class Program
{
    public string test = "test";
    static void Main(string[] args)
    {
        decimal a_d = 1254.587m;
        string b_s = @"test";

        _ = 8;
        string h = @"Multi
        line
        raw";


        int a = 4, b = 5;
        float c = (float)a;

        ref readonly int r = ref a;

        // 

        int? nullable = null;
        int bar = nullable ?? 5;

        int foo = nullable is null ? bar : 6;

        int a1;
        const int a2 = 1;
        a1 = a;
    }
}

// public class Test
// {
//     struct A
//     {
//         public int a = 1;
//         public A(int a)
//         {
//             this.a = a;
//         }
//     }
//     private A a;

//     public void foo(string bar)
//     {

//     }


//     private void Bar(int b)
//     {
//         string? a = null;
//         foo(a!);
//     }
// }
// }