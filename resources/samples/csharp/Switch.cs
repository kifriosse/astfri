namespace Switch;

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

        // 2. Switch expression with type patterns
        string Describe(object o) =>
            o switch
            {
                int i      => $"int: {i}",
                string s   => $"string: {s}",
                null       => "null",
                _          => "unknown"
            };
        Console.WriteLine(Describe("Hello"));

        // 3. Relational + logical patterns (C# 9+)
        int temp = 23;
        string category_ = temp switch
        {
            < 0              => "Freezing",
            >= 0  and < 18    => "Cold",
            >= 18 and < 25   => "Warm",
            >= 25 and < 35   => "Hot",
            _                => "Extreme"
        };

        int temp_ = 23;
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
}