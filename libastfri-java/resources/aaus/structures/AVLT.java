package aaus_semestralka_1.pkg.structures;

import java.util.ArrayList;

/**
 *
 * @author Adam-
 * @param <T>
 */
public class AVLT<T extends Comparable<T>> extends BST<T>
{
    public AVLT()
    {
        super();
    }
    
    @Override
    public AVLNode<T> getRoot()
    {
        return (AVLNode<T>) this.root;
    }
    
    public void setRoot(AVLNode<T> newRoot)
    {
        this.root = newRoot;
    }
    
    private void balanceTreeAfterInsert(AVLNode<T> node)
    {
        if (node.getBalanceFactor() == -2)
        {
            AVLNode<T> leftSon = node.getLeftSon();
            AVLNode<T> RSOLS = leftSon.getRightSon();
            if (leftSon.getBalanceFactor() == 1)
            {
                this.leftRotation(leftSon);
                this.rightRotation(node);
                if (RSOLS.getBalanceFactor() == 1)
                {
                    node.setBalanceFactor(0);
                    leftSon.setBalanceFactor(-1);
                }
                else if (RSOLS.getBalanceFactor() == -1)
                {
                    node.setBalanceFactor(1);
                    leftSon.setBalanceFactor(0);
                }
                else
                {
                    node.setBalanceFactor(0);
                    leftSon.setBalanceFactor(0);
                }
                RSOLS.setBalanceFactor(0);
            }
            else
            {
                this.rightRotation(node);
                node.setBalanceFactor(0);
                leftSon.setBalanceFactor(0);
            }
        }
        else if (node.getBalanceFactor() == 2)
        {
            AVLNode<T> rightSon = node.getRightSon();
            AVLNode<T> LSORS = rightSon.getLeftSon();
            if (rightSon.getBalanceFactor() == -1)
            {
                this.rightRotation(rightSon);
                this.leftRotation(node);
                if (LSORS.getBalanceFactor() == 1)
                {
                    node.setBalanceFactor(-1);
                    rightSon.setBalanceFactor(0);
                }
                else if (LSORS.getBalanceFactor() == -1)
                {
                    node.setBalanceFactor(0);
                    rightSon.setBalanceFactor(1);
                }
                else
                {
                    node.setBalanceFactor(0);
                    rightSon.setBalanceFactor(0);
                }
                LSORS.setBalanceFactor(0);
            }
            else
            {
                this.leftRotation(node);
                node.setBalanceFactor(0);
                rightSon.setBalanceFactor(0);
            }
        }
    }
    
    private AVLNode<T> balanceTreeAfterDelete(AVLNode<T> node)
    {
        if (node.getBalanceFactor() == -2)
        {
            AVLNode<T> leftSon = node.getLeftSon();
            AVLNode<T> RSOLS = leftSon.getRightSon();
            if (node.getLeftSon().getBalanceFactor() == 1)
            {
                this.leftRotation(node.getLeftSon());
                this.rightRotation(node);
                
                if (RSOLS.getBalanceFactor() == 1)
                {
                    node.setBalanceFactor(0);
                    leftSon.setBalanceFactor(-1);
                }
                else if (RSOLS.getBalanceFactor() == -1)
                {
                    node.setBalanceFactor(1);
                    leftSon.setBalanceFactor(0);
                }
                else
                {
                    node.setBalanceFactor(0);
                    leftSon.setBalanceFactor(0);
                }
                RSOLS.setBalanceFactor(0);
                return RSOLS;
            }
            else
            {
                this.rightRotation(node);
                if (leftSon.getBalanceFactor() == -1)
                {
                    node.setBalanceFactor(0);
                    leftSon.setBalanceFactor(0);
                    return leftSon;
                }
                else if (leftSon.getBalanceFactor() == 0)
                {
                    node.setBalanceFactor(-1);
                    leftSon.setBalanceFactor(1);
                    return leftSon;
                }
            }
        }
        else if (node.getBalanceFactor() == 2)
        {
            AVLNode<T> rightSon = node.getRightSon();
            AVLNode<T> LSORS = rightSon.getLeftSon();
            if (rightSon.getBalanceFactor() == -1)
            {
                this.rightRotation(rightSon);
                this.leftRotation(node);
                if (LSORS.getBalanceFactor() == 1)
                {
                    node.setBalanceFactor(-1);
                    rightSon.setBalanceFactor(0);
                }
                else if (LSORS.getBalanceFactor() == -1)
                {
                    node.setBalanceFactor(0);
                    rightSon.setBalanceFactor(1);
                }
                else
                {
                    node.setBalanceFactor(0);
                    rightSon.setBalanceFactor(0);
                }
                LSORS.setBalanceFactor(0);
                return LSORS;
            }
            else
            {
                this.leftRotation(node);
                if (rightSon.getBalanceFactor() == 1)
                {
                    node.setBalanceFactor(0);
                    rightSon.setBalanceFactor(0);
                    return rightSon;
                }
                else if (rightSon.getBalanceFactor() == 0)
                {
                    node.setBalanceFactor(1);
                    rightSon.setBalanceFactor(-1);
                    return rightSon;
                }
            }
        }
        return null;
    }
    
    private void updateBalanceAfterInsert(AVLNode<T> node)
    {
        AVLNode<T> parent = node.getParent();
        while (parent != null)
        {
            if (parent.getLeftSon() == node)
            {
                parent.setBalanceFactor(parent.getBalanceFactor() - 1);
            }
            else
            {
                parent.setBalanceFactor(parent.getBalanceFactor() + 1);
            }
            
            if (parent.getBalanceFactor() == 0)
            {
                return;
            }
            
            if (parent.getBalanceFactor() == 2 || parent.getBalanceFactor() == -2)
            {
                this.balanceTreeAfterInsert(parent);
                return;
            }
            
            node = node.getParent();
            parent = node.getParent();
        }
    }
    
    @Override
    protected AVLNode<T> insertNode(Node<T> node)
    {
        Node<T> newNode = super.insertNode(node);
        AVLNode<T> newAVLN = (AVLNode<T>) newNode;
        return newAVLN;
    }
    
    @Override
    public T insert(T paData)
    {
        AVLNode<T> newNode = new AVLNode<>(paData);
        if (this.insertNode(newNode) != null)
        {
            this.updateBalanceAfterInsert(newNode);
            return newNode.getData();
        }
        return null;
    }
    
    @Override
    public T find(T paData)
    {
        return super.find(paData);
    }
    
    private void updateBalanceAfterDelete(AVLNode<T> node)
    {
        AVLNode<T> parent = node.getParent();

        while (parent != null)
        {
            if (parent.getData().compareTo(node.getData()) > 0)
            {
                parent.setBalanceFactor(parent.getBalanceFactor() + 1);
            }
            else
            {
                parent.setBalanceFactor(parent.getBalanceFactor() - 1);
            }

            if (parent.getBalanceFactor() == 1 || parent.getBalanceFactor() == -1)
            {
                return;
            }

            if (parent.getBalanceFactor() == 2 || parent.getBalanceFactor() == -2)
            {
                AVLNode<T> hlpNode = this.balanceTreeAfterDelete(parent);

                if (hlpNode.getBalanceFactor() == 0)
                {
                    node = hlpNode;
                    parent = node.getParent();
                    if (parent == null)
                    {
                        return;
                    }
                    continue;
                }
                else
                {
                    return;
                }
            }

            node = parent;
            parent = node.getParent();
        }
    }

    @Override
    protected AVLNode<T> deleteNode(Node<T> node)
    {
        Node<T> delNode = super.deleteNode(node);
        return (AVLNode<T>) delNode;
    }
    
    @Override
    public void delete(T paData)
    {
        Node<T> node = new Node<>(paData);
        AVLNode<T> delNode = deleteNode(node);
        if (delNode != null)
        {
            this.updateBalanceAfterDelete(delNode);
        }
    }
    
    @Override
    public ArrayList<T> intervalSearch(T min, T max)
    {
        return super.intervalSearch(min, max);
    }
    
    @Override
    public T getMinKey()
    {
        return super.getMinKey();
    }
    
    @Override
    public T getMaxKey()
    {
        return super.getMaxKey();
    }

    private void rightRotation(AVLNode<T> node)
    {
        if (node.getLeftSon() == null)
        {
            System.out.println("It is not possible to perform right rotation.");
        }
        else
        {
            AVLNode<T> hlpPNode = node.getParent(); 
            AVLNode<T> hlpLSNode = node.getLeftSon();
            AVLNode<T> hlpRSOLSNode = hlpLSNode.getRightSon();
            
            node.setLeftSon(hlpRSOLSNode);
            if (hlpRSOLSNode != null)
            {
                hlpRSOLSNode.setParent(node);
            }
            
            hlpLSNode.setRightSon(node);
            node.setParent(hlpLSNode);
            hlpLSNode.setParent(hlpPNode);
            
            if (hlpPNode == null)
            {
                this.root = hlpLSNode;
            }
            else
            {
                if (hlpPNode.getLeftSon() == node)
                {
                    hlpPNode.setLeftSon(hlpLSNode);
                }
                else
                {
                    hlpPNode.setRightSon(hlpLSNode);
                }
            }
        }
    }
    
    private void leftRotation(AVLNode<T> node)
    {
        if (node.getRightSon() == null)
        {
            System.out.println("It is not possible to perform left rotation.");
        }
        else
        {
            AVLNode<T> hlpPNode = node.getParent();
            AVLNode<T> hlpRSNode = node.getRightSon();
            AVLNode<T> hlpLSORSNode = hlpRSNode.getLeftSon();
            
            node.setRightSon(hlpLSORSNode);
            if (hlpLSORSNode != null)
            {
                hlpLSORSNode.setParent(node);
            }
            
            hlpRSNode.setLeftSon(node);
            node.setParent(hlpRSNode);
            hlpRSNode.setParent(hlpPNode);
            
            if (hlpPNode == null)
            {
                this.root = hlpRSNode;
            }
            else
            {
                if (hlpPNode.getLeftSon() == node)
                {
                    hlpPNode.setLeftSon(hlpRSNode);
                }
                else
                {
                    hlpPNode.setRightSon(hlpRSNode);
                }
            }
        }
    }
}