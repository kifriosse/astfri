using System.Collections;
using Zoznam = System.Collections.Generic.List<string>;

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
        // try
        // {
    
        //     RecursiveFunc(1);   
        // }
        // catch (IOException ex) when (condition1)
        // {

        // }
        // catch (IOException ex) when (condition2)
        // {
        //     // ...
        // }
        // catch (IOException ex)
        // {
        //     // ...
        // }
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
}