using System;
using System.IO;

public class Program {
    static void Main(string[] args)
    {
        
    }
}

public partial class TestClass
{
    public int MethodWithLocalFunction(int x)
    {
        int LocalFunction(int y)
        {
            return y * y;
        }

        test("example", 42);
        return LocalFunction(x);
    }
}

public class Person
{
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

    public Vector2 Position { get; set; }

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

    public void Deconstruct(out int age, out string name, out Vector2 position)
    {
        age = Age;
        name = Name;
        position = Position;
    }

    public Vector2 GetPossition()
    {
        return Position;
    }
}

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
        else
        {
            Console.WriteLine($"Error: Unknown operator '{operatorSymbol}'.");
            return null;
        }
    }
}

public class Literals
{
    public void StringTest()
    {
        int a = 5;
        int b = 10;
        int c = 48;
        string normalString = "Hello from normal string.";
        string verbatimString = @"""Hello from verbatin string""";
        string multilineString = @"Hello
                                this
                                is
                                'multiline
                                verbatim
                                string'";

        string interpolatedString = $"Hello from interpolated string. Here are some variables: {b:X}, {a:N} , {c:C}";
        string interpolatedVerbatim = $@"
            Hello from interpolated verbatim string.
            Here are some variables: {a, 10:X}, {b, -10:N}, {c, 10:C}
        ";
        const int width = 10;

        string rawString = """
        This is a raw string literal.
        It can span multiple lines without needing escape sequences.
        Special characters like \n and \t are treated as normal characters.
        """;

        string rawStringInterpolated = $"""
        This is an interpolated raw string literal.
        It can also span multiple lines.
        Here are some variables: {a, 10:X}, {b, -10:N}, {c, width:C}
        """;
    }

    public void VarAndExprTest() {
        int a = 5;
        int b = 10;
        int c;
        c = (a + b) * (1 - (b - 2));

        int x_1, y_1 = 1;
        y_1++;
        ++y_1;
        const int c_1 = 1, c_2 = 2;
        x_1 = ((a + b) * (c - (a + (b * c)))) + ((a * (b + c)) - 3);

        object? d = null;
        c = d == null ? 1 : a;
    }
}

public class LoopsStmts {
    public void Loops() {
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

        for (int i = 0, j = 0; i < 10; i++, j += 2)
        {
            // ...
        }

        List<Person> people = [];
        {
            List<(string, string)> names = [
                ("John", "Doe"),
                ("Jane", "Smith"),
                ("Alice", "Johnson"),
                ("Bob", "Brown"),
                ("Charlie", "Davis"),
                ("Eve", "Wilson"),
                ("Frank", "Miller"),
                ("Grace", "Taylor"),
                ("Hank", "Anderson"),
                ("Ivy", "Thomas")
            ];
            int i;
            int j;
            for (i = 0, j = 20; i < 10; i++, j += 2)
            {
                (string name, string lastName) = names[i];
                people.Add(new Person($"{name} {lastName}", j));
            }
        }

        people = [
            new Person("John Doe", 30),
            new Person("Jane Smith", 25),
            new Person("Alice Johnson", 28)
        ];

        foreach (Person person in people)
        {
            Console.WriteLine($"Name: {person.Name}, Age: {person.Age}");
        }
    }
}

public class SwitchStmtsAndExprs {
    public record InventoryItem(string Code, int Quantity);
    public record Item(int Id, string Name);
    public static void CheckStatus(int code)
    {
        switch (code)
        {
            case 0:
                Console.WriteLine("Success.");
                break;

            case 100:
            case 101:
                Console.WriteLine("Informational.");
                break;
            case 200:
            case 201:
                Console.WriteLine("Success with data.");
                break;
            case 400:
            case 401:
                Console.WriteLine("Client Error.");
                break;
            case 500:
            case 501:
                Console.WriteLine("Server Error.");
                break;
            default:
                Console.WriteLine("Error or Unknown.");
                break;
        }
    }

    public static void HandleMessage(object payload)
    {
        switch (payload)
        {
            case null:
                Console.WriteLine("Payload is missing (null).");
                break;

            case string text:
                Console.WriteLine($"Received text message: '{text.ToUpper()}'");
                break;

            case int:
                Console.WriteLine("Received numeric data.");
                break;

            default:
                Console.WriteLine("Unrecognized object type.");
                break;
        }
    }

    public static void ProcessItem(object data)
    {
        switch (data)
        {
            case Item item when item.Name.StartsWith("Special"):
                Console.WriteLine($"Special category item ID: {item.Id}");
                break;

            case Item item when item.Id < 100:
                Console.WriteLine($"Common item ID: {item.Id}");
                break;

            case Item:
                Console.WriteLine("Standard item (No special conditions met).");
                break;

            default:
                Console.WriteLine("Data is not an Item.");
                break;
        }
    }


    public static string CategorizeData(object data)
    {
        string result = data switch
        {
            null => "Data Missing",
            int i when i < 0 => "Negative Count",
            int i when i >= 0 && i <= 10 => $"Small Positive Count ({i})",
            string s when s.Length > 20 => "Oversized Text",
            Vector2(0, 0) => "Origin Point",
            Vector2(var x, _) when x > 100 => "Far Right Point",
            string => "Standard Text",
            _ => $"Uncategorized Type: {data.GetType().Name}"
        };

        return result;
    }

    static string Classify(double measurement) => measurement switch
    {
        < -40.0 => "Too low",
        >= -40.0 and < 0 => "Low",
        >= 0 and < 10.0 => "Acceptable",
        >= 10.0 and < 20.0 => "High",
        >= 20.0 => "Too high",
        double.NaN => "Unknown",
    };

    public static decimal GetGroupTicketPrice(int visitorCount) => visitorCount switch
    {
        1 => 12.0m,
        2 => 20.0m,
        3 => 27.0m,
        4 => 32.0m,
        0 => 0.0m,
        _ => throw new ArgumentException($"Not supported number of visitors: {visitorCount}", nameof(visitorCount)),
    };

    static string GetCalendarSeason1(DateTime date) => date.Month switch
    {
        3 or 4 or 5 => "spring",
        6 or 7 or 8 => "summer",
        9 or 10 or 11 => "autumn",
        12 or 1 or 2 => "winter",
        _ => throw new ArgumentOutOfRangeException(nameof(date), $"Date with unexpected month: {date.Month}."),
    };


    static string GetCalendarSeason2(DateTime date) => date.Month switch
    {
        >= 3 and < 6 => "spring",
        >= 6 and < 9 => "summer",
        >= 9 and < 12 => "autumn",
        12 or (>= 1 and < 3) => "winter",
        _ => throw new ArgumentOutOfRangeException(nameof(date), $"Date with unexpected month: {date.Month}."),
    };
}

public class ExceptionHandlingDemo
{
    public static void ProcessData(string filePath, int operationType)
    {
        StreamReader fileReader = null;

        try
        {
            fileReader = new StreamReader(filePath);

            if (operationType == 1)
            {
                throw new FileNotFoundException("The required configuration file was not found.");
            }
            else if (operationType == 2)
            {
                int result = 100 / (operationType - 2);
            }
        }
        catch (FileNotFoundException ex)
        {
            Console.WriteLine($"Caught Specific Error: {ex.Message}");
        }

        catch (DivideByZeroException ex)
        {
            Console.WriteLine($"Caught Math Error: {ex.Message}");
        }
        catch (IOException ex) when (ex.Message.Contains("access denied"))
        {
            Console.WriteLine($"Caught IO Access Error: {ex.Message}");
        }
        catch (IOException ex)
        {
            Console.WriteLine($"Caught General IO Error: {ex.Message}");
        }
        catch (Exception)
        {
            throw;
        }
        catch
        {
            throw;
        }
        finally
        {
            if (fileReader != null)
            {
                fileReader.Close();
            }
        }
    }
}

public partial class TestClass : TestBase
{
    
}