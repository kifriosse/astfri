

public class GenericClass<T> : IComparable<GenericClass<T>> where T : IComparable<T>, IEquatable<T>
{
    private T _value;

    public GenericClass(T value)
    {
        _value = value;
    }

    public T GetValue()
    {
        return _value;
    }

    public void SetValue(T value)
    {
        _value = value;
    }

    public int CompareTo(GenericClass<T>? other)
    {
        if (other is null) 
            return 1;
        return _value.CompareTo(other._value);
    }
}

public class MultiGenericClass<T1, T2> 
    where T1 : IComparable<T1> 
    where T2 : IEquatable<T2>
{
    
}

// Súbor 1
partial class A : IDisposable
{
    public void Dispose()
    {
        throw new NotImplementedException();
    }
}

// Súbor 2
partial class A : IComparable, IDisposable
{

    public int Method<B, C>(B b)
    {
        return 1;
    }


    public int Method(int b)
    {
        return 1;
    }

    public int CompareTo(object? obj)
    {
        throw new NotImplementedException();
    }
}



public interface IGenericInterface<T>
{
    T GetItem();
    void SetItem(T item);
}
