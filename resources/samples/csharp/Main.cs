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
                                string'";

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

        int x_1, y_1 = 1;
        const int c_1 = 1, c_2 = 2;
        x_1 = ((a + b) * (c - (a + (b * c)))) + ((a * (b + c)) - 3);

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
    }
}