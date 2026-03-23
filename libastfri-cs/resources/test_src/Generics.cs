

public class GenericClass<T> where T : IComparable<T>, IEquatable<T>
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
