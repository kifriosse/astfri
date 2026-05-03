namespace csharp;

public class Fraction : IEquatable<Fraction> {
    private readonly long _numerator;
    private readonly long _denominator;

    public Fraction(long numerator, long denominator) {
        if (denominator == 0) 
            throw new ArgumentException("Denominator cannot be 0");
        
        long common = Gcd(Math.Abs(numerator), Math.Abs(denominator));
        
        _numerator = numerator / common;
        _denominator = denominator / common;
        
        if (_denominator >= 0) 
            return;
        
        _numerator = -_numerator;
        _denominator = -_denominator;
    }

    public Fraction(long numerator) {
        _numerator = numerator;
        _denominator = 1;
    }

    public double Value() {
        return _numerator / (double)_denominator;
    }

    public Fraction Add(Fraction other) {
        long lcm = Lcm(_denominator, other._denominator);
        long factor1 = lcm / _denominator;
        long factor2 = lcm / other._denominator;
        return new Fraction(factor1 * _numerator + factor2 * other._numerator, lcm);
    }
    
    public static Fraction operator -(Fraction fraction) => new(-fraction._numerator, fraction._denominator);

    public static Fraction operator +(Fraction first, Fraction second) => first.Add(second);

    public static Fraction operator -(Fraction first, Fraction second) => first.Add(-second);

    public static Fraction operator *(Fraction first, Fraction second) {
        long n = first._numerator * second._numerator;
        long d = first._denominator * second._denominator;
        return new Fraction(n, d);
    }

    public static Fraction operator *(Fraction fraction, long scalar) => 
        new(fraction._numerator * scalar, fraction._denominator);

    public static Fraction operator *(long scalar, Fraction f) => f * scalar;

    public static Fraction operator /(Fraction first, Fraction second) =>
        first * new Fraction(second._denominator, second._numerator);

    public static Fraction operator /(Fraction f, long scalar) => 
        scalar == 0 
            ? throw new DivideByZeroException() 
            : new Fraction(f._numerator, f._denominator * scalar);

    public static Fraction operator /(long scalar, Fraction f) =>
        f._numerator == 0 
            ? throw new DivideByZeroException() 
            : new Fraction(scalar * f._denominator, f._numerator);

    public static implicit operator Fraction(long number) => new(number);
    
    public static bool operator ==(Fraction a, Fraction b) => 
        a._numerator == b._numerator && a._denominator == b._denominator;
    
    public static bool operator !=(Fraction a, Fraction b) => !(a == b);
    
    public override bool Equals(object? obj) => obj is Fraction f && this == f;
    public bool Equals(Fraction other) => _numerator == other._numerator && _denominator == other._denominator;
    public override int GetHashCode() => HashCode.Combine(_numerator, _denominator);

    public void Deconstruct(out long n, out long d) {
        n = _numerator;
        d = _denominator;
    }

    public override string ToString() => 
        _denominator == 1 ? _numerator.ToString() : $"{_numerator}/{_denominator}";

    private static long Gcd(long a, long b)
    {
        while (b != 0)
        {
            long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    private static long Lcm(long a, long b) => 
        a == 0 || b == 0 ? 0 : Math.Abs(a * b) / Gcd(a, b);

}