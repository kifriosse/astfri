using System.Collections;
using Zoznam = System.Collections.Generic.List<string>;
using System.Collections.Generic;


public class Program
{
    private const int constant = 6; // test comment
    private string test = "test";
    private IEnumerable enumerable;
    private unsafe volatile protected string* testPtr;

    static void RecursiveFunc(int i)
    {
        Console.WriteLine($"{i}");
        RecursiveFunc(i + 1);
    }

    static void Main(string[] args)
    {
        
        var rand = new Random();
        int num = unchecked((int)rand.NextInt64()) % 100;
        int guess;
        Console.WriteLine("Guess the number from 0 to 99");
        do
        {
            Console.WriteLine("Enter the guess");
            string? strGuess = Console.ReadLine();
            if (! int.TryParse(strGuess, out guess))
            {
                Console.WriteLine("Input a number");
                continue;
            }
            if (guess != num)
            {
                Console.WriteLine("Incorrect guess");
            }
            else
            {
                Console.WriteLine("Correct guess");        
            }
        }
        while (guess != num);

        int[] array = { 4, 5, 6, 8, 77, 47, 59 };
        IEnumerator<int> enumerator = array.AsEnumerable().GetEnumerator();

        while (enumerator.MoveNext())
        {
            Console.WriteLine($"{enumerator.Current}");
        }

        List<Person> people = [];
        for (
            int i = 0, j = 0;
            i < 10;
            ++i, j += 2, people.Add(new Person($"Person {i}", j))
        )
        {
            //...
        }
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
}