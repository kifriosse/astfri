using System;
using CSharp.T.Test;

class Switch
{
    // record Person(string Name, int Age);
    // record Address(string City, string Country);
    // record User(string Name, Address Address);

    static void SwitchStmt()
    {
        object value = 42;
        switch (value)
        {
            default:
                Console.WriteLine("Unknown type");
                break;
            case int i:
            {
                Console.WriteLine($"Integer: {i}");
            }
            break;
            case string s:
                Console.WriteLine($"String: {s}");
                break;
            case null:
                Console.WriteLine("Null");
                break;
        }

        switch (value)
        {
            case 1:
            case 2:
                break;
        }

        {
            Console.WriteLine();
        }
//
//         // 2. Switch expression with type patterns
//         string Describe(object o) =>
//             o switch
//             {
//                 int i      => $"int: {i}",
//                 string s   => $"string: {s}",
//                 null       => "null",
//                 _          => "unknown"
//             };
//         Console.WriteLine(Describe("Hello"));
//
//         // 3. Relational + logical patterns (C# 9+)
        int temp = 23;
        string category;
        switch (temp)
        {
            case var t when t < 0:
                category = "Freezing";
                break;

            case var t when t >= 0 && t < 18:
                category = "Cold";
                break;
                
            case var t when t >= 18 && t < 25:
                category = "Warm";
                break;

            case var t when t >= 25 && t < 35:
                category = "Hot";
                break;

            default:
                category = "Extreme";
                break;
        }
//         Console.WriteLine(category);
//
//         // 4. Property pattern matching
//         Person p = new("Alice", 30);
//         string info = p switch
//         {
//             { Age: < 18 } => "Minor",
//             { Age: >= 18 and < 65 } => "Adult",
//             { Age: >= 65 } => "Senior"
//         };
//         Console.WriteLine(info);
//
//         // 5. Nested property patterns
//         User u = new("Bob", new Address("New York", "USA"));
//         var result = u switch
//         {
//             { Address: { Country: "USA", City: "New York" } } => "NY User",
//             { Address: { Country: "Slovakia" } }              => "Slovak User",
//             _                                                 => "Other"
//         };
//         Console.WriteLine(result);
//
//         // 6. List/collection patterns (C# 11+)
//         // int[] nums = new int[] { 1, 2, 3 };
//
//         // string listResult = nums switch
//         // {
//         //     []                 => "Empty",
//         //     [1]                => "Single with 1",
//         //     [1, .. var rest]   => $"Starts with 1, rest count = {rest.Length}",
//         //     [_, _, 3]          => "Ends with 3",
//         //     _                  => "Other"
//         // };
//         // Console.WriteLine(listResult);
//
//         // 7. Complex matching with guards (when)
//         object obj = 7;
//         string outcome = obj switch
//         {
//             int i when i % 2 == 0 => "Even integer",
//             int i                 => "Odd integer",
//             string s when s == "" => "Empty string",
//             string s              => $"String: {s}",
//             _                     => "Something else"
//         };
//         Console.WriteLine(outcome);
//
//         object val = new int();
//
//         if (value is int j)
//         {
//             Console.WriteLine($"Integer: {j}");
//         }
//
//         Person pers = new("Alice", 30);
//
//         if (pers is { Age: >= 18 })
//         {
//             Console.WriteLine("Adult");
//         }
    }
//
}
//

public static class OperatorProcessor
{
    public static double? ProcessOperation(double operand1, double operand2, string operatorSymbol)
    {
        string op = operatorSymbol.Trim().ToLower(); 

        if (op == "+" || op == "add")
        {
            return operand1 + operand2;
        }
        else if (op == "-" || op == "minus")
        {
            return operand1 - operand2;
        }
        else if (op == "*" || op == "multiply")
        {
            return operand1 * operand2;
        }
        else if (op == "/" || op == "divide")
        {
            if (operand2 == 0)
            {
                Console.WriteLine("Error: Division by zero is not allowed.");
                return null;
            }
            return operand1 / operand2;
        }
        else if (op == "%" || op == "modulus")
        {
            return operand1 % operand2;
        }
        else if (op == "^" || op == "power")
        {
            return Math.Pow(operand1, operand2);
        }
        else if (op == ">")
        {
            Console.WriteLine("Result is a boolean, not a number.");
            return operand1 > operand2 ? 1 : 0; 
        }
        else if (op == "or")
        {
            return (operand1 != 0 || operand2 != 0) ? 1 : 0;
        }
        return null;
    }
}

file class TestCases
{

    public int Property { get; set; }
    public int _d = 1;

    public void Run()
    {
        int a = 5;
        int b = 10;
        int c;
        int? d = null; 
        c = (a + b) * (1 - (b - 2));

        string verbatimString = @"""Hello from verbatin string""";
        string multilineString = @"Hello '
                                this
                                is
                                multiline
                                string";
    
        string interpolatedVerbatim = $@"{verbatimString}
            {verbatimString}
        ";

        string rawString = """
            {}
        """;


        string rawStringInterpolated = $"""
            {verbatimString}
        """;

        while(c < 10)
        {
            ++c;
        }


        do 
        {
            ++a;
        }
        while(a < 10);

        c = d == null ? 1 : a;

        // Parenthesized binary expressions
        int x_1, y_1 = 1;
        const int c_1 = 1, c_2 = 2;
        x_1 = ((a + b) * (c - (a + (b * c)))) + ((a * (b + c)) - 3);

        // Parenthesized expression inside if-pattern
        if ((c + 1) > (a * 2))
        {
            Console.WriteLine("Pattern matched");
        }
        // int j;
        // int i;
        for (int i = 0, j = 0; i < 10; i++, j += 2)
        {
            // ...
        }
     
        (int age, string name) tuple = (1, "");
        _ = tuple.name;
        _ = tuple.Item1;
        (int, string name, char) tuple1 = (1, "", 'c');
        _ = tuple1.Item2;
        Person person = new ("Jozko Mrkvicka", 18);
        float x1 = person.GetPossition().X;
        Person person_ref = person;
        int _d = 1;
        int d_local = _d;

        // List<Person> people = [];
        // foreach ((int age_f, string name_f, (float x, float y)) in people)
        // {
        // }
    }

    ~TestCases() {
        int d_local = _d;
    }

    public void ArrowBodyExample()
    {
        int d_local = _d;
        List<string> fruits = new() { "apple", "banana", "cherry", "date" };

        bool hasA = fruits.Any(fruit => fruit.Contains('a'));

        Console.WriteLine($"List contains a fruit with 'a': {hasA}");

        var fruitsWithE = fruits.Where(fruit => fruit.Contains('e')).ToList();

        Console.WriteLine("Fruits with 'e':");
        fruitsWithE.ForEach(f => Console.WriteLine(f));
    }


    public void ArrowBodyMethod() => ArrowBodyExample();

    public int Add(int a, int b) => a + b;
}
