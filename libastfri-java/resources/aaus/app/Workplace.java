package aaus_semestralka_1.pkg.app;

import aaus_semestralka_1.pkg.wrappers.TestsByDateAndTime;
import aaus_semestralka_1.pkg.structures.AVLT;

/**
 *
 * @author Adam-
 */
public class Workplace implements Comparable<Workplace>
{
    
    private int ID;
    private int districtID;
    private AVLT<TestsByDateAndTime> tests;
    
    public Workplace(int paID, int paDistrictID)
    {
        this.ID = paID;
        this.districtID = paDistrictID;
        this.tests = new AVLT<>();
    }

    public int getID() 
    {
        return this.ID;
    }

    public int getDistrictID() 
    {
        return this.districtID;
    }

    public AVLT<TestsByDateAndTime> getTests() 
    {
        return this.tests;
    }

    @Override
    public int compareTo(Workplace o) 
    {
        if (this.ID > o.getID())
        {
            return 1;
        }
        else if (this.ID < o.getID())
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }   
}