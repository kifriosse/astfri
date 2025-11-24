using Zoznam = System.Collections.Generic.List<string>;

public class Program {
    public static void Main(string[] args) {
        Console.WriteLine("Hello World");
        Zoznam z = new();
    }
}

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


        public class Test5
        {
            int bar;
            public Test5()
            {
                Console.WriteLine();
            }

        }
    }

    public partial class Program
    {

        private const int constant = 6; // test comment
        private string test = "test";
        private unsafe volatile protected string* testPtr;

        static void Main2(string[] args)
        {
            int[] array = new int[2];
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

            int brackets = (a + b) * 10;

            int a1;
            const int a2 = 1;
            a1 = a;
        }

        unsafe public void UnsafeMethod()
        {
            fixed(string* testPtr = &test)
            {
                this.testPtr = testPtr;
            }
        }
    }

    public class Person
    {
        private static readonly int mf = 8;
        private string _firstName;
        private string _lastName;
        public string Name { 
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
            Console.WriteLine($"{Name}, {Age}");
        }

        ~Person() => Print();

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

    class Child : Person
    {
        public bool IsStudent;

        public Child(string name, int age, bool isStudent) 
            : base(name ?? "Unknown", age > 18 ? 18 : age) // complex base initializer
        {
            IsStudent = isStudent;
        }

        public Child() 
            : base("DefaultName", 0) // simpler base initializer
        {
            IsStudent = true;
        }
    }
}