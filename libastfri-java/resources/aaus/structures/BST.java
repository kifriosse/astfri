package aaus_semestralka_1.pkg.structures;

import aaus_semestralka_1.pkg.funcinterfaces.InstanceMaker;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Scanner;
/**
 *
 * @author Adam-
 * @param <T>
 */
public class BST<T extends Comparable<T>>
{
    protected Node<T> root;
    
    public BST()
    {
        this.root = null;
    }
    
    public Node<T> getRoot()
    {
        return this.root;
    }
    
    public void setRoot(Node<T> newRoot)
    {
        if (newRoot != null)
        {
            this.root = newRoot;
        }
    }
   
    protected Node<T> insertNode(Node<T> node)
    {
        Node<T> currNode;
        
        if (this.root == null)
        {
            this.root = node;
            return this.getRoot();
        }
        else
        {
            currNode = this.getRoot(); 
            while (currNode != null)
            {
                if (node.getData().compareTo(currNode.getData()) < 0)
                {
                    if (currNode.getLeftSon() == null)
                    {
                        node.setParent(currNode);
                        currNode.setLeftSon(node);
                        return node;
                    }
                    else
                    {
                        currNode = currNode.getLeftSon();
                    }
                }
                else if (node.getData().compareTo(currNode.getData()) > 0)
                {
                    if (currNode.getRightSon() == null)
                    {
                        node.setParent(currNode);
                        currNode.setRightSon(node);
                        return node;
                    }
                    else
                    {
                        currNode = currNode.getRightSon();
                    }
                }
                else
                {
//                    System.out.println("Duplicate record.");
                    return null;
                }
            }
            return null;
        }
    }
    
    public T insert(T paData)
    {
        Node<T> node = new Node<>(paData);
        Node<T> newNode = this.insertNode(node);
        
        return newNode != null ? newNode.getData() : null;
    }
    
    protected Node<T> findNode(T paData)
    {
        Node<T> currNode;
        if (this.root == null)
        {
            //System.out.println("No records.");
            return null;
        }
        
        currNode = this.root;
        while (currNode != null)
        {
            if (currNode.compareTo(paData) == 0)
            {
                return currNode;
            }
            
            
            if (paData.compareTo(currNode.getData()) < 0)
            {
                currNode = currNode.getLeftSon();
            }
            else
            {
                currNode = currNode.getRightSon();
            }
        }
//        System.out.println("Record not found.");
        return null;
    }
    
    public T find(T data)
    {
        Node<T> node = this.findNode(data);
        return node != null ? node.getData() : null;
    }
    
    protected Node<T> deleteNode(Node<T> node)
    {
        Node<T> currNode = this.root;
        
        while (currNode != null)
        {
            if (node.getData().compareTo(currNode.getData()) < 0)
            {
                currNode = currNode.getLeftSon();
            }
            else if (node.getData().compareTo(currNode.getData()) > 0)
            {
                currNode = currNode.getRightSon();
            }
            else
            {
                break;
            }
        }
        
        if (currNode == null)
        {
//            System.out.println("Record not in tree.");
            return currNode;
        }
        
        Node<T> parent = currNode.getParent();
        
        if (currNode.getLeftSon() == null && currNode.getRightSon() == null)
        {
            if (parent == null)
            {
                this.root = null;
                return currNode;
            }
            if (parent.getLeftSon() == currNode)
            {
                parent.setLeftSon(null);
            }
            else
            {
                parent.setRightSon(null);
            }
            return currNode;
        }
        else if (currNode.getLeftSon() != null && currNode.getRightSon() == null)
        {
            if (parent == null)
            {
                this.root = currNode.getLeftSon();
                currNode.getLeftSon().setParent(null);
                return currNode;
            }
            if (parent.getLeftSon() == currNode)
            {
                parent.setLeftSon(currNode.getLeftSon());
            }
            else
            {
                parent.setRightSon(currNode.getLeftSon());
            }
            currNode.getLeftSon().setParent(parent);
            return currNode;
        }
        else if (currNode.getLeftSon() == null && currNode.getRightSon() != null)
        {
            if (parent == null)
            {
                this.root = currNode.getRightSon();
                currNode.getRightSon().setParent(null);
                return currNode;
            }
            if (parent.getLeftSon() == currNode)
            {
                parent.setLeftSon(currNode.getRightSon());
            }
            else
            {
                parent.setRightSon(currNode.getRightSon());
            }
            currNode.getRightSon().setParent(parent);
            return currNode;
        }
        else
        {
            Node<T> nextInOrder = currNode.getRightSon();
            while (nextInOrder.getLeftSon() != null)
            {
                nextInOrder = nextInOrder.getLeftSon();
            }
            
            currNode.setData(nextInOrder.getData());
            
            Node<T> nioParent = nextInOrder.getParent();
            Node<T> nioSon = nextInOrder.getRightSon();
            
            if (nioParent.getLeftSon() == nextInOrder)
            {
                nioParent.setLeftSon(nioSon);
            }
            else
            {
                nioParent.setRightSon(nioSon);
            }
            
            if (nioSon != null)
            {
                nioSon.setParent(nioParent);
            }
            return nextInOrder;
        }
    }
    
    public void delete(T paData)
    {
        Node<T> delNode = new Node<>(paData);
        this.deleteNode(delNode);
    }
    
    protected Node<T> getMinKeyNode()
    {
        if (this.root == null)
        {
            return null;
        }
        
        Node<T> currNode = this.root;
        
        while (currNode.getLeftSon() != null)
        {
            currNode = currNode.getLeftSon();
        }
        return currNode;
    }
    
    public T getMinKey()
    {
        Node<T> minKey = this.getMinKeyNode();
        return minKey == null ? null : minKey.getData();
    }
    
    protected Node<T> getMaxKeyNode()
    {
        if (this.root == null)
        {
            return null;
        }
        
        Node<T> currNode = this.root;
                
        while (currNode.getRightSon() != null)
        {
            currNode = currNode.getRightSon();
        }
        return currNode;
    }
    
    public T getMaxKey()
    {
        Node<T> maxKey = this.getMaxKeyNode();
        return maxKey  == null ? null : maxKey.getData();
    }
    
    public ArrayList<T> intervalSearch(T min, T max)
    {
        ArrayList<T> nodes = new ArrayList<>();
        if (this.root == null)
        {
            return nodes;
        }
        
        Node<T> currNode = this.findNode(min);
    
        if (currNode == null)
        {
            currNode = this.getMinKeyNode();
            while (currNode != null && currNode.getData().compareTo(min) < 0)
            {
                currNode = this.getInorderSuccessor(currNode);
            }
        }
        
        while (currNode != null && currNode.getData().compareTo(max) <= 0)
        {
            if (currNode.getData().compareTo(min) >= 0 && currNode.getData().compareTo(max) <= 0)
            {
                nodes.add(currNode.getData());
            }
            
            currNode = this.getInorderSuccessor(currNode);
        }
        
        return nodes;
    }
    
    private Node<T> getInorderSuccessor(Node<T> node)
    {
        if (node.getRightSon() != null)
        {
            Node<T> inoSuccessor = node.getRightSon();
            while (inoSuccessor.getLeftSon() != null)
            {
                inoSuccessor = inoSuccessor.getLeftSon();
            }
            return inoSuccessor;
        }
        else
        {
            Node<T> parent = node.getParent();
            Node<T> child = node;
            
            while (parent != null && child == parent.getRightSon()) 
            {
                child = parent;
                parent = parent.getParent();
            }
            return parent;
        }
    }
    
    public ArrayList<T> inorderTraversal() 
    {
        ArrayList<T> hlpArray = new ArrayList<>();
        if (this.root == null)
        {
            return hlpArray;
        }
        
        Node<T> currNode = this.root;
        
        while (currNode.getLeftSon() != null)
        {
            currNode = currNode.getLeftSon();
        }
        
        hlpArray.add(currNode.getData());
        
        while (this.getInorderSuccessor(currNode) != null)
        {
            currNode = this.getInorderSuccessor(currNode);
            hlpArray.add(currNode.getData());
        }
        
        return hlpArray;
    }
    
    public ArrayList<T> levelOrderTraveesal()
    {
        ArrayList<T> hlpArray = new ArrayList<>();
        if (this.root == null)
        {
            return hlpArray;
        }
        
        ArrayList<Node<T>> queue = new ArrayList<>();
        
        queue.add(this.root);
        
        while (!queue.isEmpty())
        {
            if (queue.getFirst().getLeftSon() != null)
            {
                queue.add(queue.getFirst().getLeftSon());
            }
            
            if (queue.getFirst().getRightSon() != null)
            {
                queue.add(queue.getFirst().getRightSon());
            }
            
            hlpArray.add(queue.getFirst().getData());
            queue.removeFirst();
        }
        
        return hlpArray;
    }
    
    public boolean exportToCSV(String filename)
    {
        if (!filename.toLowerCase().endsWith(".csv")) 
        {
            filename += ".csv";
        }
        String home = System.getProperty("user.home");
        Path download = Paths.get(home, "downloads", filename);
        
        String path = download.toString();
        
        ArrayList<T> hlpArray = this.levelOrderTraveesal();
        
        try
        {
            try (FileWriter fw = new FileWriter(path)) 
            {
                for (var d : hlpArray)
                {
                    fw.write(d.toString());
                    fw.write(System.lineSeparator());
                }
            }
            return true;
        }
        catch (IOException e)
        {
            System.out.println("Error while writing into file. " + e.getMessage());
        }
        
        return false;
    }
    
    public boolean importFromCSV(String path, InstanceMaker<T> factory)
    {
        ArrayList<String> values = new ArrayList<>();
        File input = new File(path);
        Scanner fileScanner;
        try 
        {
            fileScanner = new Scanner(input);
            Scanner lineScanner;

            while (fileScanner.hasNextLine())
            {
                values.clear();
                lineScanner = new Scanner(fileScanner.nextLine());
                lineScanner.useDelimiter(",");

                while(lineScanner.hasNext())
                {
                    values.add(lineScanner.next());
                }
                T newObject = factory.makeInstance(values);

                if (newObject != null)
                {
                    this.insert(newObject);
                }
                lineScanner.close();
            }
            fileScanner.close();
        } 
        catch (FileNotFoundException e) 
        {
            System.out.println("File not found. " + e.getMessage());
        }
        return false;
    }
}