// decimal a = 1254.587m;
using System.Runtime.InteropServices;

// string b = @"test";

string h = @"Multi
line
raw";

int a = 4;
int b = 5;


float c = (float)a;

int? nullable = null;
int bar = nullable ?? 5;

int foo = nullable is null ? bar : 6;

int a1;
const int a2 = 1;
a1 = a;
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