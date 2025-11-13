package aaus_semestralka_1.pkg.structures;

/**
 *
 * @author Adam-
 * @param <T>
 */
public class Node<T extends Comparable<T>> 
{
    protected Node<T> leftSon;
    protected Node<T> rightSon;
    protected Node<T> parent;
    protected T data;
    
    public Node(T paData)
    {
        this.leftSon = null;
        this.rightSon = null;
        this.parent = null;
        this.data = paData;
    }
    
    public Node<T> getLeftSon()
    {
        return this.leftSon;
    }
    
    public Node<T> getRightSon()
    {
        
        return this.rightSon;
    }
    
    public Node<T> getParent()
    {
        return this.parent;
    }
    
    public T getData()
    {
        return this.data;
    }
    
    public void setLeftSon(Node<T> node)
    {
        this.leftSon = node;
    }
    
    public void setRightSon(Node<T> node)
    {
        this.rightSon = node;
    }
    
    public void setParent(Node<T> node)
    {
        this.parent = node;
    }
    
    public T setData(T newData)
    {
        return this.data = newData;
    }
    
    public int compareTo(T other)
    {
        return this.data.compareTo(other);
    }
}
