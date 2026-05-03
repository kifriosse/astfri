using System.ComponentModel;

namespace csharp;

public class Matrix {
    private Fraction[,] _matrix;

    public (int, int) Size => (_matrix.GetLength(0), _matrix.GetLength(1));
    public int SizeX => _matrix.GetLength(0);
    public int SizeY => _matrix.GetLength(1);

    public Matrix(Fraction[,] matrix) {
        _matrix = matrix;
    }
    
    public Fraction this[int x, int y] {
        get {
            if (y >= _matrix.GetLength(0) || x >= _matrix.GetLength(1)) 
                throw new IndexOutOfRangeException($"Indexes x: {x} y: {y} are out of range.");
            return _matrix[y, x];
        }
        set {
            if (y >= _matrix.GetLength(0) || x >= _matrix.GetLength(1)) 
                throw new IndexOutOfRangeException($"Indexes x: {x} y: {y} are out of range.");
            _matrix[y, x] = value;
        }
    }

    public Matrix Add(Matrix other) {
        if (Size == other.Size)
            throw new ArgumentException("Input matrices have to have the same size");
        
        Matrix result = new Fraction[SizeY, other.SizeX];
        for (int i = 0; i < result.SizeY; ++i) {
            for (int j = 0; j < result.SizeY; ++j) {
                result[i, j] =  this[i, j] + other[i, j];
            }
        }
        return result;
    }

    public Matrix Multiply(Matrix other) {
        if (SizeX != other.SizeY)
            throw new ArgumentException("Input matrices have incorrect size for multiplication");
        
        Matrix result = new Fraction[other.SizeY, SizeX];
        for (int i = 0; i < result.SizeY; i++) {
            for (int j = 0; j < result.SizeX; j++) {
                Fraction sum = new(0);
                for (int k = 0; k < SizeX; k++) {
                    sum += this[i, k] * other[k, j];
                }
                result[i, j] = sum;
            }
        }
        return result;
    }

    public static Matrix operator +(Matrix first, Matrix second) {
        return first.Add(second);
    }

    public static Matrix operator *(Matrix first, Matrix second) {
        return first.Multiply(second);
    }
    
    public static implicit operator Matrix(Fraction[,] matrix)  => new (matrix);
}