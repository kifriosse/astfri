// using System;
//

using CSharp.T.Test;

const int testGlobalVar = 1;

/*
    (local_declaration_statement [147, 8] - [147, 35]
        (modifier [147, 8] - [147, 13])
        (variable_declaration [147, 14] - [147, 34]
            type: (predefined_type [147, 14] - [147, 17])
            (variable_declarator [147, 18] - [147, 25]
                name: (identifier [147, 18] - [147, 21])
                (integer_literal [147, 24] - [147, 25]))
            (variable_declarator [147, 27] - [147, 34]
                name: (identifier [147, 27] - [147, 30])
                (integer_literal [147, 33] - [147, 34]))))

    (local_declaration_statement [146, 8] - [146, 21]
        (variable_declaration [146, 8] - [146, 20]
            type: (predefined_type [146, 8] - [146, 11])
            (variable_declarator [146, 12] - [146, 13]
                name: (identifier [146, 12] - [146, 13]))
            (variable_declarator [146, 15] - [146, 20]
                name: (identifier [146, 15] - [146, 16])
                (integer_literal [146, 19] - [146, 20]))))

    (field_declaration [118, 4] - [118, 22]
        (modifier [118, 4] - [118, 10])
        (variable_declaration [118, 11] - [118, 21]
            type: (predefined_type [118, 11] - [118, 14])
            (variable_declarator [118, 15] - [118, 21]
                name: (identifier [118, 15] - [118, 17])
                (integer_literal [118, 20] - [118, 21]))))
*/


// class Switch
// {
//     record Person(string Name, int Age);
//     record Address(string City, string Country);
//     record User(string Name, Address Address);
//
//     static void Main()
//     {
//         // 1. Basic type pattern matching
//         object value = 42;
//         switch (value)
//         {
//             case int i:
//                 Console.WriteLine($"Integer: {i}");
//                 break;
//             case string s:
//                 Console.WriteLine($"String: {s}");
//                 break;
//             case null:
//                 Console.WriteLine("Null");
//                 break;
//             default:
//                 Console.WriteLine("Unknown type");
//                 break;
//         }
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
//         int temp = 23;
//         string category = temp switch
//         {
//             < 0              => "Freezing",
//             >= 0 and < 18    => "Cold",
//             >= 18 and < 25   => "Warm",
//             >= 25 and < 35   => "Hot",
//             _                => "Extreme"
//         };
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
//     }
//
// }
//

file class TestCases
{

    public int Property { get; set; }
    public int _d = 1;

    public void Run()
    {
//         Property = 1;
//         Console.WriteLine(Property);
        int a = 5;
        int b = 10;
        int c;
        int? d = null; 
        c = (a + b) * (1 - (b - 2));

        string verbatinString = @"""Hello from verbatin string""";
        string multilineString = @"Hello
                                this
                                is
                                multiline
                                string";

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
//         // int j;
//         // int i;
        for (int i = 0, j = 0; i < 10; i++, j += 2)
        {
            // ...
        }
//      
        (int age, string name) tuple = (1, "");
        _ = tuple.name;
        _ = tuple.Item1;
        (int, string name, char) tuple1 = (1, "", 'c');
        _ = tuple1.Item2;
        List<Person> people = [];
        foreach (Person p in people)
        {
            // ...
        }
    }

    ~TestCases() {

    }

    public void ArrowBodyExample()
    {
//         List<string> fruits = new() { "apple", "banana", "cherry", "date" };
//
//         bool hasA = fruits.Any(fruit => fruit.Contains('a'));
//
//         Console.WriteLine($"List contains a fruit with 'a': {hasA}");
//
//         var fruitsWithE = fruits.Where(fruit => fruit.Contains('e')).ToList();
//
//         Console.WriteLine("Fruits with 'e':");
//         fruitsWithE.ForEach(f => Console.WriteLine(f));
    }


    public void ArrowBodyMethod() => ArrowBodyExample();

    public int Add(int a, int b) => a + b;
}
