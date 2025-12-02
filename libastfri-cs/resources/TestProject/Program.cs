using System.Collections;
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

    public interface IIntr<G> : IComparable where G : IComparable<int>
    {
        private static int s = 10;
        void Interfacing(Test5 t)
        {
            
        }


        public interface Test5
        {
            void testIntMethod()
            {
                Console.WriteLine();
            }

        }
    }

    public partial class Program
    {
        private const int constant = 6; // test comment
        private string test = "test";
        private IEnumerable enumerable;
        private unsafe volatile protected string* testPtr;

        static void Main2(string[] args)
        {
            
            int[] array = { 4, 5, 6, 8, 77, 47, 59 };
            while (true)
            {
                break;
            }
            
            do
            {
                break;
            } while (true);
            // List<Person> people = [];
            // for (
            //     int i = 0, j = 0;
            //     i < 10;
            //     ++i, j += 2, people.Add(new Person($"Person {i}", j))
            // )
            // {
            //     //...
            // }
            // foreach (Person person in people)
            // {
            //     Console.WriteLine($"Name: {person.Name} Age: {person.Age}");
            // }

            // decimal a_d = 1254.587m;
            // string b_s = @"test";

            // _ = 8;
            // string h = @"Multi
            // line
            // raw";

            // const int constant2 = 1;

            // int a = 4, b = 5;
            // float c = (float)a;

            // ref readonly int r = ref a;

            // int? nullable = null;
            // int bar = nullable ?? 5;

            // int foo = nullable is null ? bar : 6;

            // int brackets = (a + b) * 10;

            // int a1;
            // const int a2 = 1;
            // a1 = a;
        }

        unsafe public void UnsafeMethod()
        {
            fixed(string* testPtr = &test)
            {
                this.testPtr = testPtr;
            }
        }
    }

    /// <summary>
    /// Testing documentation comments
    /// </summary>
    public class Person
    {
        private static readonly int mf = 8;
        private string _firstName;
        private string _lastName;
        public string Name { 
            get => _firstName + " " + _lastName;
            set
            {
                string[] /* test */ parts = value.Split(" ");
                _firstName = parts[0];
                _lastName = parts[1];
            } 
        }
        public int Age { get; init; }
        private Pohlavie gender = Pohlavie.Muz;

        public Person(string fullName, int age)
        {
            Age = age;
            Name = fullName;
        }

        public void Print()
        {
            Console.WriteLine($"{Name}, {Age}");
        }

        protected virtual void TestVirtuality()
        {
        }

        ~Person() => Print();

        private enum Pohlavie
        {
            Zena, Muz, Ine
        }

        public delegate void ChangeDelegate(int something);
        public event ChangeDelegate OnChange;
    }

    class Child<T, U> : Person where U: IEnumerable<T>, IComparable<U>
    {
        static Child()
        {

        }
        public bool IsStudent;

        public Child(string name, int age, bool isStudent) 
            : base(name ?? "Unknown", age > 18 ? 18 : age)
        {
            IsStudent = isStudent;
        }

        public Child() 
            : base("DefaultName", 0)
        {
            IsStudent = true;
        }

        public G TestGenericMethod<G>() where G : new()
        {
            return new G();
        }

        protected sealed override void TestVirtuality()
        {
            base.TestVirtuality();
        }
    }
}


