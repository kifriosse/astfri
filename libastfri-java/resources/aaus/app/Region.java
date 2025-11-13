package aaus_semestralka_1.pkg.app;

import aaus_semestralka_1.pkg.wrappers.TestsByDateAndTime;
import aaus_semestralka_1.pkg.structures.AVLT;

/**
 *
 * @author Adam-
 */
public class Region implements Comparable<Region>
{
    private int ID;
    private AVLT<District> districts;
    private AVLT<TestsByDateAndTime> tests;
    private AVLT<TestsByDateAndTime> positiveTests;
    
    public Region(int paID)
    {
        this.ID = paID;
        this.districts = new AVLT<>();
        this.tests = new AVLT<>();
        this.positiveTests = new AVLT<>();
    }

    public int getID() 
    {
        return this.ID;
    }

    public AVLT<District> getDistricts() 
    {
        return this.districts;
    }

    public AVLT<TestsByDateAndTime> getTests() 
    {
        return this.tests;
    }

    public AVLT<TestsByDateAndTime> getPositiveTests() 
    {
        return this.positiveTests;
    }

    @Override
    public int compareTo(Region o) 
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