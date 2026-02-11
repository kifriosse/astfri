using System;
using CSharp.T.Test;

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
