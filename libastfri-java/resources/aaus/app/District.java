package aaus_semestralka_1.pkg.app;

import aaus_semestralka_1.pkg.wrappers.TestsByDateAndTime;
import aaus_semestralka_1.pkg.structures.AVLT;

/**
 *
 * @author Adam-
 */
public class District implements Comparable<District>
{
    private int ID;
    private int regionID;
    private AVLT<TestsByDateAndTime> tests;
    private AVLT<TestsByDateAndTime> positiveTests;
    private AVLT<Workplace> workplaces;
    
    public District(int paID, int paRegionID)
    {
        this.ID = paID;
        this.regionID = paRegionID;
        this.tests = new AVLT<>();
        this.positiveTests = new AVLT<>();
        this.workplaces = new AVLT<>();
    }

    public int getID() 
    {
        return this.ID;
    }

    public int getRegionID() 
    {
        return this.regionID;
    }

    public AVLT<TestsByDateAndTime> getTests() 
    {
        return this.tests;
    }

    public AVLT<TestsByDateAndTime> getPositiveTests() 
    {
        return this.positiveTests;
    }

    public AVLT<Workplace> getWorkplaces() 
    {
        return this.workplaces;
    }

    @Override
    public int compareTo(District o) 
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