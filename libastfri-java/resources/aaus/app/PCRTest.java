package aaus_semestralka_1.pkg.app;

import java.time.LocalDateTime;

/**
 *
 * @author Adam-
 */
public class PCRTest implements Comparable<PCRTest>
{
    private LocalDateTime dateAndTimeOfTest;
    private String uniquePatientNumber;
    private int PCRtestCode;
    private int uniqueWorkplaceCode;
    private int districtCode;
    private int regionCode;
    private boolean testResult;
    private double testValue;
    private String note;

    public PCRTest(LocalDateTime paDateAndTimeOfTest, String paUniquePatientNumber, 
                    int paPCRtestCode, int paUniqueWorkplaceCode, 
                    int paDistrictCode, int paRegionCode, boolean paTestResult, 
                    double paTestValue, String paNote) 
    {
        this.dateAndTimeOfTest = paDateAndTimeOfTest;
        this.uniquePatientNumber = paUniquePatientNumber;
        this.PCRtestCode = paPCRtestCode;
        this.uniqueWorkplaceCode = paUniqueWorkplaceCode;
        this.districtCode = paDistrictCode;
        this.regionCode = paRegionCode;
        this.testResult = paTestResult;
        this.testValue = paTestValue;
        this.note = paNote;
    }

    public LocalDateTime getDateAndTimeOfTest() 
    {
        return this.dateAndTimeOfTest;
    }

    public void setDateAndTimeOfTest(LocalDateTime dateAndTimeOfTest) 
    {
        this.dateAndTimeOfTest = dateAndTimeOfTest;
    }

    public String getUniquePatientNumber() 
    {
        return this.uniquePatientNumber;
    }

    public void setUniquePatientNumber(String uniquePatientNumber) 
    {
        this.uniquePatientNumber = uniquePatientNumber;
    }

    public int getPCRtestCode() 
    {
        return this.PCRtestCode;
    }

    public void setPCRtestCode(int PCRtestCode) 
    {
        this.PCRtestCode = PCRtestCode;
    }

    public int getUniqueWorkplaceCode() 
    {
        return this.uniqueWorkplaceCode;
    }

    public void setUniqueWorkplaceCode(int uniqueWorkplaceCode) 
    {
        this.uniqueWorkplaceCode = uniqueWorkplaceCode;
    }

    public int getDistrictCode() 
    {
        return this.districtCode;
    }

    public void setDistrictCode(int districtCode) 
    {
        this.districtCode = districtCode;
    }

    public int getRegionCode() 
    {
        return this.regionCode;
    }

    public void setRegionCode(int regionCode) 
    {
        this.regionCode = regionCode;
    }

    public boolean isTestResult() 
    {
        return this.testResult;
    }

    public void setTestResult(boolean testResult) 
    {
        this.testResult = testResult;
    }

    public double getTestValue() 
    {
        return this.testValue;
    }

    public void setTestValue(double testValue) 
    {
        this.testValue = testValue;
    }

    public String getNote() 
    {
        return this.note;
    }

    public void setNote(String note) 
    {
        this.note = note;
    }
    
    @Override
    public String toString()
    {
        String string = this.dateAndTimeOfTest + "," + this.uniquePatientNumber + "," + this.PCRtestCode + "," + this.uniqueWorkplaceCode + 
                "," + this.districtCode + "," + this.regionCode + "," + this.testResult + "," + this.testValue + "," + this.note;
        return string;
    }
    
    public String toStringForOutput()
    {
        String string = "Date and time of the test: " + this.dateAndTimeOfTest + "\nUnique patient number: " + this.uniquePatientNumber + "\nPCRTestCode: " + this.PCRtestCode + "\n"
                + "Unique workplace code: " + this.uniqueWorkplaceCode + "\nDistrict code: " + this.districtCode + "\nRegion code: " + this.regionCode + "\nTest result: " + this.testResult
                + "\nTest value: " + this.testValue + "\nNote: " + this.note;
        return string;
    }

    @Override
    public int compareTo(PCRTest o) 
    {
        if (this.PCRtestCode > o.getPCRtestCode())
        {
            return 1;
        }
        else if (this.PCRtestCode < o.getPCRtestCode())
        {
            return -1;
        }
        else
        {
//            if (this.dateAndTimeOfTest.compareTo(o.getDateAndTimeOfTest()) > 0)
//            {
//                return 1;
//            }
//            else if (this.dateAndTimeOfTest.compareTo(o.getDateAndTimeOfTest()) < 0)
//            {
//                return -1;
//            }
//            else
//            {
                return 0;
//            }
        }
    }
}