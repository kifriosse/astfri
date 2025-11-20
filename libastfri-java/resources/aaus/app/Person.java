package aaus_semestralka_1.pkg.app;

import aaus_semestralka_1.pkg.structures.AVLT;
import java.time.LocalDate;

/**
 *
 * @author Adam-
 */
public class Person implements Comparable<Person>
{
    private String name;
    private String lastName;
    private LocalDate birthdayDate;
    private String uniquePatientNumber;
    private final AVLT<PCRTest> tests;
    
    public Person(String paName, String paLastName, LocalDate paBD, String paUPN)
    {
        this.name = paName;
        this.lastName = paLastName;
        this.birthdayDate = paBD;
        this.uniquePatientNumber = paUPN;
        this.tests = new AVLT<>();
    }
    
    public String getName()
    {
        return this.name;
    }

    public void setName(String newName)
    {
        this.name = newName;
    }
    
    public String getLastName() 
    {
        return this.lastName;
    }

    public void setLastName(String newLastName) 
    {
        this.lastName = newLastName;
    }

    public LocalDate getBirthdayDate() 
    {
        return this.birthdayDate;
    }

    public void setBirthdayDate(LocalDate newBirthdayDate) 
    {
        this.birthdayDate = newBirthdayDate;
    }

    public String getUniquePatientNumber() 
    {
        return this.uniquePatientNumber;
    }

    public void setUniquePatientNumber(String newUniquePatientNumber) 
    {
        this.uniquePatientNumber = newUniquePatientNumber;
    }

    public AVLT<PCRTest> getTests() 
    {
        return this.tests;
    }
    
    @Override
    public String toString()
    {
        String string = this.name + "," + this.lastName + "," + this.birthdayDate + "," + this.uniquePatientNumber;
        return string;
    }
    
    public String toStringForOutput()
    {
        String string = "Name: " + this.name + "\nLastname: " + this.lastName + "\nBirthday: " + this.birthdayDate + "\nUnique Patient Number:" + this.uniquePatientNumber;
        return string;
    }
    
    @Override
    public int compareTo(Person o) 
    {
        
        if (this.uniquePatientNumber.compareTo(o.getUniquePatientNumber()) > 0)
        {
            return 1;
        }
        else if (this.uniquePatientNumber.compareTo(o.getUniquePatientNumber()) < 0)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
}