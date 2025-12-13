public struct Vector2
{
    public float X { get; set; }
    public float Y { get; set; }

    public Vector2(float x, float y)
    {
        X = x;
        Y = y;
    }

    public float this[int i]
    {
        get => i switch
        {
            0 => X,
            1 => Y,
            _ => throw new ArgumentException($"Invalid index : {i}")
        };
        set
        {
            switch (i)
            {
                case 0:
                {
                    X = value;
                    break;
                }
                case 1:
                    Y = value;
                    break;
                default: 
                    throw new ArgumentException($"Invalid index : {i}");
            }
        }
    }

    public static Vector2 operator +(Vector2 first, Vector2 second) =>
        new(first.X + second.X, first.Y + second.Y);

    public static explicit operator int(Vector2 v) => (int)v.X;

    public void Deconstruct(out float x, out float y)
    {
        x = X;
        y = Y;
    }
}