public class GotoTest
{
    public void TestGoto()
    {
        int x = 0;

        for (int i = 0; i < 5; i++)
        {
            if (i == 3)
            {
                goto Label1;
            }
        }

        Label1:
        x = 2;
        {
            Label2:
            x = 5;
            x = 1;
            x = 2;

            Label4:
            x = 3;
        }
    }

    public void TestGotoWithSwitch(int value)
    {
        switch (value)
        {
            case 1:
                goto TestLabel;
            case 2:
                TestLabel:
                goto case 3;
            case 3:
                goto default;
            default:
                break;
        }
    }
}