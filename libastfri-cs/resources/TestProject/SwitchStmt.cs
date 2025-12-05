// using System;
//
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

        while(c < 10)
        {
            ++c;
            return;
        }

        do 
        {
            ++a;
            throw new NotImplementedException("");
        }
        while(a < 10);

        c = d == null ? 1 : a;

//
//         // Parenthesized binary expressions
//         int x = ((a + b) * (c - (a + (b * c)))) + ((a * (b + c)) - 3);
//
//         // Parenthesized expression inside if-pattern
//         if ((c + 1) > (a * 2))
//         {
//             Console.WriteLine("Pattern matched");
//         }
//         // int j;
//         // int i;
        for (int i = 0, j = 0; i < 10; ++i, j += 2)
        {
            // ...
        }
//
//         foreach (int i in new int[] {1,2,a,4})
//         {
//             // ...
//         }
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
