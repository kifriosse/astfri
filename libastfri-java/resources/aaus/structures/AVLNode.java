package aaus_semestralka_1.pkg.structures;

/**
 *
 * @author Adam-
 * @param <T>
 */
public class AVLNode<T extends Comparable<T>> extends Node<T>
{
    private int balanceFactor;
    
    public AVLNode(T value)
    {
        super(value);
        this.balanceFactor = 0;
    }
    
    public int getBalanceFactor()
    {
        return this.balanceFactor;
    }
    
    @Override
    public AVLNode<T> getLeftSon()
    {
        return (AVLNode<T>) this.leftSon;
    }
    
    @Override
    public AVLNode<T> getRightSon()
    {
        return (AVLNode<T>) this.rightSon;
    }
    
    @Override
    public AVLNode<T> getParent()
    {
        return (AVLNode<T>) this.parent;
    }
    
    public void setBalanceFactor(int paBalanceFactor)
    {
        this.balanceFactor = paBalanceFactor;
    }
    
    public void setLeftSon(AVLNode<T> avlNode)
    {
        this.leftSon = avlNode;
    }
    
    public void setRightSon(AVLNode<T> avlNode)
    {
        this.rightSon = avlNode;
    }
    
    public void setParent(AVLNode<T> avlNode)
    {
        this.parent = avlNode;
    }
}