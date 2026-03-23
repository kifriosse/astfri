// global using System.Numerics;
// global using static System.Math;
// global using Cons = global::System.Console;
// using System.Drawing;
// using System.Runtime.ExceptionServices;
// using global::System.Security.Cryptography.X509Certificates;
// using Generics = System.Collections.Generic;

public class Vector : IComparable<Vector>, IEquatable<Vector> 
{
    float[] elements;

    public int Size => elements.Length;

    public Vector(int n)
    {
        elements = new float[n];
    }
    public Vector(params float[] elements) : this(elements.Length)
    {
        elements.CopyTo(this.elements);
    }

    public Vector(Vector other)
    {
        other.elements.CopyTo(elements);
    }

    public float this[uint index]
    {
        get {
            if (index >= elements.Length)
                throw new IndexOutOfRangeException(
                    $"Index: '{index}' out of range for vector of size: '{elements.Length}"
                );
            return elements[index];
        }
        set
        {
            if (index >= elements.Length)
                throw new IndexOutOfRangeException(
                    $"Index: '{index}' out of range for vector of size: '{elements.Length}"
                );
            elements[index] = value;
        }
    }

    public static Vector operator+ (Vector first, Vector second)
    {
        return Sum(first, second, false);
    }

    public static Vector operator- (Vector first, Vector second)
    {
        return Sum(first, second, true);
    }

    public static Vector operator* (float scalar, Vector vec)
    {
        Vector result = new(vec.Size);
        for (uint i = 0; i < result.Size; ++i)
        {
            result[i] = vec[i] * scalar;
        }
        return result;
    }

    public static Vector operator* (Vector vec, float scalar) => scalar * vec; 

    public static Vector operator/ (Vector vec, float scalar)
    {
        if (scalar == 0) 
            throw new DivideByZeroException("Cannot divide a vector by zero.");
        return 1f / scalar * vec;
    }

    public float DotProduct(Vector other)
    {
        if (Size != other.Size)
            throw new ArgumentException("Both vectors need to be have same size");
        float result = 0;
        for (uint i = 0; i < Size; ++i)
        {
            result += this[i] * other[i];
        }
        return result;
    }

    public int CompareTo(Vector? other)
    {
        return other == null 
            ? 1
            : elements.SequenceCompareTo(other.elements);
    }

    public bool Equals(Vector? other)
    {
        return other == null 
            ? false
            : elements.SequenceEqual(other.elements);
    }

    private static Vector Sum(Vector first, Vector second, bool subtract)
    {
        if (first.Size != second.Size)
            throw new ArgumentException("Both vectors need to be have same size");
        Vector result = new(first.Size);
        for (uint i = 0; i < result.Size; ++i)
        {
            result[i] = first[i] + (subtract ? -second[i] : second[i]);
        }
        return result;
    }
}

public class Vector2 : Vector
{
    public float X
    {
        get => this[0];
        set => this[0] = value;
    }
    public float Y
    {
        get => this[1];
        set => this[1] = value;
    }

    public Vector2(float x = 0, float y = 0) : base(x, y)
    {
    }

    public Vector2(float x) : this(x, x)
    {
    }

    public void Deconstruct(out float x, out float y)
    {
        x = this.X;
        y = this.Y;
    }
}

public class Vector3 : Vector
{
    public float X
    {
        get => this[0];
        set => this[0] = value;
    }
    public float Y
    {
        get => this[1];
        set => this[1] = value;
    }

    public float Z
    {
        get => this[2];
        set => this[2] = value;
    }

    public Vector3(float x = 0, float y = 0, float z = 0) : base(x, y, z)
    {
    }

    public Vector3(float val) : this(val, val, val)
    {
    }

    public void Deconstruct(out float x, out float y, out float z)
    {
        x = this.X;
        y = this.Y;
        z = this.Z;
    }
}