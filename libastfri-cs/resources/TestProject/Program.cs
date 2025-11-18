using System.ComponentModel;
using System.Numerics;
using System.Reflection.Metadata;
using System.Runtime.CompilerServices;
using System.Runtime.ConstrainedExecution;
using System.Security.AccessControl;
using System.Security.Cryptography;
using System.Threading.Tasks;

using Zoznam = System.Collections.Generic.List<string>;

namespace CSharp.T.Test
{
    namespace Test2
    {
        internal delegate int Pr(int x);
    }

    public interface Intr : IComparable
    {
        static int s = 10;
        void Interfacing(Test5 t)
        {
            
        }
        class Test5
        {
            int bar;
            public Test5()
            {
                
            }

        }
    }

    public partial class Program
    {

        private const int constant = 6;
        private static readonly protected string test = "test";

        static void Main(string[] args)
        {
            decimal a_d = 1254.587m;
            string b_s = @"test";

            _ = 8;
            string h = @"Multi
            line
            raw";

            const int constant2 = 1;

            int a = 4, b = 5;
            float c = (float)a;

            ref readonly int r = ref a;

            int? nullable = null;
            int bar = nullable ?? 5;

            int foo = nullable is null ? bar : 6;

            int a1;
            const int a2 = 1;
            a1 = a;
        }
    }

    public class Person
    {
        private static readonly int mf = 8;
        private string _firstName;
        private string _lastName;
        public String Name { 
            get => _firstName + " " + _lastName;
            set
            {
                string[] parts = value.Split(" ");
                _firstName = parts[0];
                _lastName = parts[1];
            } 
        }
        public int Age { get; set; }

        public Person(string fullName, int age)
        {
            Age = age;
            Name = fullName;
        }

        public void Print()
        {
            
        }

        private class Child : Person
        {
            public Child(string fullName, int age) : base(fullName, age)
            {
            }
        }

        ~Person() {
            
        }

        private enum Pohlavie
        {
            Zena, Muz, Ine
        }

        private interface IRunnable
        {
            
        }

        private record class Something
        {
            
        }

        private record struct SomethingStruct
        {
            
        }

        private struct SomethingStruct2
        {
            
        }

        public delegate void ChangeDelegate(int something);
        public event ChangeDelegate OnChange;
    }

    public class Vector2(float x, float y)
    {
        // [Obsolete]
        // public static readonly Vector2 zero = new(0,0);
        public float X { get; set; } = x;
        public float Y { get; set; } = y;

        public float this[int i]
        {
            get => i switch
            {
                0 => X,
                1 => Y,
                _ => throw new ArgumentException($"Invalid index : {i}")
            };
            set
            {
                switch (i)
                {
                    case 0:
                    {
                        X = value;
                        break;
                    }
                    case 1:
                        Y = value;
                        break;
                    default: throw new ArgumentException($"Invalid index : {i}");
                }
            }
        }

        public static Vector2 operator +(Vector2 first, Vector2 second) =>
            new(first.X + second.X, first.Y + second.Y);

        public static explicit operator int(Vector2 v) => (int)v.X;
    }
}