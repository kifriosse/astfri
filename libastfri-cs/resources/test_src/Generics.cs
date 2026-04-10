

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

public interface IGenericInterface<T>
{
    T GetItem();
    void SetItem(T item);
}
