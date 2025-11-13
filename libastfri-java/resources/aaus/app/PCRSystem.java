package aaus_semestralka_1.pkg.app;

import aaus_semestralka_1.pkg.funcinterfaces.InstanceMaker;
import aaus_semestralka_1.pkg.wrappers.IntervalsOrderedBySize;
import aaus_semestralka_1.pkg.wrappers.TestsByDateAndTime;
import aaus_semestralka_1.pkg.wrappers.TestsByValue;
import aaus_semestralka_1.pkg.structures.AVLT;
import java.io.File;
import java.io.FileNotFoundException;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Scanner;

/**
 *
 * @author Adam-
 */
public class PCRSystem 
{
    private AVLT<Person> patients;
    private AVLT<PCRTest> allTests;
    private AVLT<TestsByDateAndTime> testsOrderedByDateAndTime;
    private AVLT<TestsByDateAndTime> positiveTests;
    private AVLT<Region> regions;
    private AVLT<District> allDistricts;
    private AVLT<Workplace> allWorkplaces;

    public PCRSystem() 
    {
        this.patients = new AVLT<>();
        this.allTests = new AVLT<>();
        this.testsOrderedByDateAndTime = new AVLT<>();
        this.positiveTests = new AVLT<>();
        this.regions = new AVLT<>();
        this.allDistricts = new AVLT<>();
        this.allWorkplaces = new AVLT<>();
    }
    
    private boolean addTests(PCRTest test, Person p, District d, Region r, Workplace w)
    {
        if (this.allTests.insert(test) == null)
        {
            return false;
        }
        this.testsOrderedByDateAndTime.insert(new TestsByDateAndTime(test));
        p.getTests().insert(test);
        d.getTests().insert(new TestsByDateAndTime(test));
        r.getTests().insert(new TestsByDateAndTime(test));
        w.getTests().insert(new TestsByDateAndTime(test));

        if (test.isTestResult())
        {
            this.positiveTests.insert(new TestsByDateAndTime(test));
            d.getPositiveTests().insert(new TestsByDateAndTime(test));
            r.getPositiveTests().insert(new TestsByDateAndTime(test));
        }
        return true;
    }
    
    // Method 1 add test into System
    public String addPCRTestIntoSystem(PCRTest test)
    {
        String output;
        Person p = new Person(null, null, null, test.getUniquePatientNumber());
        Person foundPatient = this.patients.find(p);
        if (foundPatient != null)
        {
            Workplace w = new Workplace(test.getUniqueWorkplaceCode(), test.getDistrictCode());
            District d = new District(test.getDistrictCode(), test.getRegionCode());
            Region r = new Region(test.getRegionCode());

            Region foundRegion = this.regions.find(r);
            if (foundRegion == null)
            {
                this.regions.insert(r);
                foundRegion = r;
            }
            
            District foundDistrict = this.allDistricts.find(d);
            if (foundDistrict != null)
            {
                if (foundDistrict.getRegionID() != foundRegion.getID())
                {
                    output = "Wrong combination for district and region.";
                    return output;
                }
            }
            else
            {
                foundRegion.getDistricts().insert(d);
                this.allDistricts.insert(d);
                foundDistrict = d;
            }
            
            Workplace foundWorkplace = this.allWorkplaces.find(w);
            if (foundWorkplace != null)
            {
                if (foundWorkplace.getDistrictID() != foundDistrict.getID())
                {
                    output = "Wrong combination for workplace and district.";
                    return output;
                }
            }
            else
            {
                foundDistrict.getWorkplaces().insert(w);
                this.allWorkplaces.insert(w);
                foundWorkplace = w;
            }

            if (this.addTests(test, foundPatient, foundDistrict, foundRegion, foundWorkplace))
            {
                output = "Added test:\n" + test.toStringForOutput();
                return output;
            }
            return "Test not added.";
        }
        return "Patient not found.";
    }
    
    // Method 2 find test results for patient and print his data
    public String findTestResultForPatient(String upn, int pcrCode)
    {
        String output;
        Person p = this.patients.find(new Person(null, null, null, upn));
        if (p != null)
        {
            output = "Not found.";
            PCRTest t = p.getTests().find(new PCRTest(null, null, pcrCode, 0, 0, 0, true, 0, null)); 
            if (t != null)
            {
                output = p.toStringForOutput();
                output += "\n" + t.toStringForOutput();
            }
            return output;
        }
        output = "Not found.";
        return output;
    }
    
    // Method 3 print all tests for patient with specified unique patient number
    public String printAllTestsForPatient(String upn)
    {
        String output;
        Person p = new Person(null, null, null, upn);
        Person found = this.patients.find(p);
        if (found != null)
        {
            output = found.toStringForOutput()+ "\n----------------------------------------------------------------";
            ArrayList<PCRTest> tests = found.getTests().inorderTraversal();
            if (tests.isEmpty())
            {
                output += "\nNo tests for patient.";
                return output;
            }
                output += "\nAll tests:";
            for (var t : tests)
            {
                output += "\n" + t.toStringForOutput()+ "\n----------------------------------------------------------------";
            }
            return output;
        }
        output = "Patient not found in system";
        return output;
    }
    
    // Method 4 print all positive tests for specified district in specified time range
    public String printAllPositiveTestsForDistrictInTimeRange(LocalDateTime from, LocalDateTime to, int districtCode)
    {
        String output = "";
        PCRTest t1 = new PCRTest(from, null, 0, 0, 0, 0, true, 0, null);
        PCRTest t2 = new PCRTest(to, null, 0, 0, 0, 0, true, 0, null);
        District d = this.allDistricts.find(new District(districtCode, 0));
        
        if (d != null)
        {
            ArrayList<TestsByDateAndTime> tests = d.getPositiveTests().intervalSearch(new TestsByDateAndTime(t1), new TestsByDateAndTime(t2));
            for (var t : tests)
            {
                output += "\n" + t.getTest().toStringForOutput()+ "\n-------------------------------------------------";
            }
            return output;
        }
        output = "District not found.";
        return output;
    }
    
    // Method 5 print all tests for specified district in specified time range
    public String printAllTestsForDistrictInTimeRange(LocalDateTime from, LocalDateTime to, int districtCode)
    {
        String output = "";
        PCRTest t1 = new PCRTest(from, null, 0, 0, 0, 0, true, 0, null);
        PCRTest t2 = new PCRTest(to, null, 0, 0, 0, 0, true, 0, null);
        District d = this.allDistricts.find(new District(districtCode, 0));
        
        if (d != null)
        {
            ArrayList<TestsByDateAndTime> tests = d.getTests().intervalSearch(new TestsByDateAndTime(t1), new TestsByDateAndTime(t2));
            
            for (var t : tests)
            {
                output += "\n" + t.getTest().toStringForOutput()+ "\n-------------------------------------------------";
            }
            return output;
        }
        output = "District not found.";
        return output;
    }
    
    // Method 6 print all positive tests for specified region in specified time range
    public String printAllPositiveTestsForRegionInTimeRange(LocalDateTime from, LocalDateTime to, int regionCode)
    {
        String output;
        PCRTest t1 = new PCRTest(from, null, 0, 0, 0, 0, true, 0, null);
        PCRTest t2 = new PCRTest(to, null, 0, 0, 0, 0, true, 0, null);
        Region r = this.regions.find(new Region(regionCode));
        
        if (r != null)
        {
            ArrayList<TestsByDateAndTime> tests = r.getPositiveTests().intervalSearch(new TestsByDateAndTime(t1), new TestsByDateAndTime(t2));
            output = "";
            
            for (var t : tests)
            {
                output += "\n" + t.getTest().toStringForOutput()+ "\n-------------------------------------------------";
            }
            return output;
        }
        output = "Region not found.";
        return output;
    }
    
    // Method 7 print all tests for specified region in specified time range
    public String printAllTestsForRegionInTimeRange(LocalDateTime from, LocalDateTime to, int regionCode)
    {
        String output = "";
        PCRTest t1 = new PCRTest(from, null, 0, 0, 0, 0, true, 0, null);
        PCRTest t2 = new PCRTest(to, null, 0, 0, 0, 0, true, 0, null);
        Region r = this.regions.find(new Region(regionCode));
        
        if (r != null)
        {
            ArrayList<TestsByDateAndTime> tests = r.getTests().intervalSearch(new TestsByDateAndTime(t1), new TestsByDateAndTime(t2));
            
            for (var t : tests)
            {
                output += "\n" + t.getTest().toStringForOutput()+ "\n-------------------------------------------------";
            }
            return output;
        }
        output = "Region not found.";
        return output;
    }
    
    // Method 8 print all positive tests in specified time range
    public String printAllPositiveTestsInTimeRange(LocalDateTime from, LocalDateTime to)
    {
        String output = "";
        PCRTest t1 = new PCRTest(from, null, 0, 0, 0, 0, true, 0, null);
        PCRTest t2 = new PCRTest(to, null, 0, 0, 0, 0, true, 0, null);
        ArrayList<TestsByDateAndTime> tests = this.positiveTests.intervalSearch(new TestsByDateAndTime(t1), new TestsByDateAndTime(t2));
        
        if (tests.isEmpty())
        {
            output = "No positive tests in specified time.";
            return output;
        }
        
        for (var t : tests)
        {
            output += "\n" + t.getTest().toStringForOutput()+ "\n-------------------------------------------------";
        }
        return output;
    }
    
    // Method 9 print all tests in specified time range
    public String printAllTestsInTimeRange(LocalDateTime from, LocalDateTime to)
    {
        String output = "";
        PCRTest t1 = new PCRTest(from, null, 0, 0, 0, 0, true, 0, null);
        PCRTest t2 = new PCRTest(to, null, 0, 0, 0, 0, true, 0, null);
        ArrayList<TestsByDateAndTime> tests = this.testsOrderedByDateAndTime.intervalSearch(new TestsByDateAndTime(t1), new TestsByDateAndTime(t2));
        
        if (tests.isEmpty())
        {
            output = "No positive tests in specified time.";
            return output;
        }
        
        for (var t : tests)
        {
            output += "\n" + t.getTest().toStringForOutput()+ "\n-------------------------------------------------";
        }
        return output;
    }
    
    // Method 10 print all sick people for specified district in specified time range
    public String printSickPeopleInDistrict(int districtCode, LocalDateTime date, int numOfDays)
    {
        String output = "";
        TestsByDateAndTime t1 = new TestsByDateAndTime(new PCRTest(date, null, 0, 0, 0, 0, true, 0, null));
        TestsByDateAndTime t2 = new TestsByDateAndTime(new PCRTest(date.minusDays(numOfDays), null, 0, 0, 0, 0, true, 0, null));
        District d = this.allDistricts.find(new District(districtCode, 0));
        
        if (d != null)
        {
            ArrayList<TestsByDateAndTime> tests = d.getPositiveTests().intervalSearch(t2, t1);
            if (tests.isEmpty())
            {
                output = "No sick people in specified district and time.";
                return output;
            }
            
            for (var t : tests)
            {
                String upn = t.getTest().getUniquePatientNumber();
                Person p = patients.find(new Person (null, null, null, upn));
                if (p != null)
                {
                    output += "\n" + p.toStringForOutput()+ "\n-------------------------------------------------" + "\n" + 
                            t.getTest().toStringForOutput()+ "\n-------------------------------------------------";
                }
            }
            return output;
        }
        output = "District not found.";
        return output;
    }
    
    // Method 11 print all sick people for specified district in specified time range from sickest
    public String printSickPeopleInDistrictByTestValue(int districtCode, LocalDateTime date, int numOfDays)
    {
        String output = "";
        TestsByDateAndTime t1 = new TestsByDateAndTime(new PCRTest(date, null, 0, 0, 0, 0, true, 0, null));
        TestsByDateAndTime t2 = new TestsByDateAndTime(new PCRTest(date.minusDays(numOfDays), null, 0, 0, 0, 0, true, 0, null));
        District d = this.allDistricts.find(new District(districtCode, 0));
        
        if (d != null)
        {
            ArrayList<TestsByDateAndTime> tests = d.getPositiveTests().intervalSearch(t2, t1);

            AVLT<TestsByValue> avl = new AVLT<>();

            for (var t : tests)
            {
                avl.insert(new TestsByValue(t.getTest()));
            }

            if (avl.getRoot() == null)
            {
                output = "No sick people in specified district and time.";
                return output;
            }
            ArrayList<TestsByValue> results = avl.inorderTraversal();

            for (int i = results.size() - 1; i > -1; i--)
            {
                String upn = results.get(i).getTest().getUniquePatientNumber();
                Person p = patients.find(new Person (null, null, null, upn));
                if (p != null)
                {
                    output += "\n" + p.toStringForOutput()+ "\n-------------------------------------------------\n" + 
                            results.get(i).getTest().toStringForOutput()+ "\n-------------------------------------------------";
                }
            }
            return output;
        }
        output = "District not found.";
        return output;
    }
    
    // Method 12 print all sick people for specified region in specified time range
    public String printSickPeopleInRegion(int regionCode, LocalDateTime date, int numOfDays)
    {
        String output = "";
        TestsByDateAndTime t1 = new TestsByDateAndTime(new PCRTest(date, null, 0, 0, 0, 0, true, 0, null));
        TestsByDateAndTime t2 = new TestsByDateAndTime(new PCRTest(date.minusDays(numOfDays), null, 0, 0, 0, 0, true, 0, null));
        Region r = this.regions.find(new Region(regionCode));
        
        if (r != null)
        {
            ArrayList<TestsByDateAndTime> tests = r.getPositiveTests().intervalSearch(t2, t1);
            if (tests.isEmpty())
            {
                output = "No sick people in specified region and time.";
                return output;
            }
            
            for (var t : tests)
            {
                String upn = t.getTest().getUniquePatientNumber();
                Person p = patients.find(new Person (null, null, null, upn));
                if (p != null)
                {
                    output += "\n" + p.toStringForOutput()+ "\n-------------------------------------------------" + "\n" + 
                            t.getTest().toStringForOutput()+ "\n-------------------------------------------------";
                }
            }
            return output;
        }
        output = "Region not found.";
        return output;
    }
    
    // Method 13 print all sick people in specified time range
    public String printSickPeople(LocalDateTime date, int numOfDays)
    {
        String output = "";
        TestsByDateAndTime t1 = new TestsByDateAndTime(new PCRTest(date, null, 0, 0, 0, 0, true, 0, null));
        TestsByDateAndTime t2 = new TestsByDateAndTime(new PCRTest(date.minusDays(numOfDays), null, 0, 0, 0, 0, true, 0, null));
        ArrayList<TestsByDateAndTime> tests = this.positiveTests.intervalSearch(t2, t1);
        
        if (tests.isEmpty())
        {
            output = "No sick people in specified time.";
            return output;
        }
        
        for (var t : tests)
        {
            String upn = t.getTest().getUniquePatientNumber();
            Person p = patients.find(new Person (null, null, null, upn));
            if (p != null)
            {
                output += "\n" + p.toStringForOutput()+ "\n-------------------------------------------------" + "\n" + 
                        t.getTest().toStringForOutput()+ "\n-------------------------------------------------";
            }
        }
        return output;
    }
    
    // Method 14
    public String printSickestPersonInEveryDistrict(LocalDateTime date, int numOfDays)
    {
        String output = "";
        ArrayList<Person> result = new ArrayList<>();
        TestsByDateAndTime t1 = new TestsByDateAndTime(new PCRTest(date, null, 0, 0, 0, 0, true, 0, null));
        TestsByDateAndTime t2 = new TestsByDateAndTime(new PCRTest(date.minusDays(numOfDays), null, 0, 0, 0, 0, true, 0, null));
        
        ArrayList<District> districts = this.allDistricts.inorderTraversal();
        
        for (var d : districts)
        {
            ArrayList<TestsByDateAndTime> tests = d.getPositiveTests().intervalSearch(t2, t1);
            if (tests.isEmpty())
            {
//                System.out.println("No sick people in district: " + d.getID());
                continue;
            }
            
            TestsByDateAndTime max = tests.getFirst();
            for (var t : tests)
            {
                if (Double.compare(t.getTest().getTestValue(), max.getTest().getTestValue()) > 0)
                {
                    max = t;
                }
            }
            result.add(this.patients.find(new Person(null, null, null, max.getTest().getUniquePatientNumber())));
        }
        
        for (var r : result)
        {
            output += "\n"+ r.toStringForOutput()+ "\n----------------------------------------------";
        }
        return output;
    }
    
    // Method 15
    public String printDistrictsOrderedByNumOfSick(LocalDateTime date, int numOfDays)
    {
        String output = "";
        TestsByDateAndTime t1 = new TestsByDateAndTime(new PCRTest(date, null, 0, 0, 0, 0, true, 0, null));
        TestsByDateAndTime t2 = new TestsByDateAndTime(new PCRTest(date.minusDays(numOfDays), null, 0, 0, 0, 0, true, 0, null));
        
        ArrayList<District> districts = this.allDistricts.inorderTraversal();
        AVLT<IntervalsOrderedBySize> avl = new AVLT<>();
        
        for (var d : districts)
        {
            ArrayList<TestsByDateAndTime> tests = d.getPositiveTests().intervalSearch(t2, t1);
            if (tests.isEmpty())
            {
                //System.out.println("No sick people in district: " + d.getID());
                continue;
            }
            avl.insert(new IntervalsOrderedBySize(tests, d.getID()));
        }
        
        ArrayList<IntervalsOrderedBySize> result = avl.inorderTraversal();
        
        for (int i = result.size() - 1; i > -1; i--)
        {
            output += "\n" + result.get(i).getTests().size() + ", in district: " + result.get(i).getId();
        }
        return output;
    }
    
    // Method 16
    public String printRegionsOrderedByNumOfSick(LocalDateTime date, int numOfDays)
    {
        String output = "";
        TestsByDateAndTime t1 = new TestsByDateAndTime(new PCRTest(date, null, 0, 0, 0, 0, true, 0, null));
        TestsByDateAndTime t2 = new TestsByDateAndTime(new PCRTest(date.minusDays(numOfDays), null, 0, 0, 0, 0, true, 0, null));
        
        ArrayList<Region> regs = this.regions.inorderTraversal();
        AVLT<IntervalsOrderedBySize> avl = new AVLT<>();
        
        for (var r : regs)
        {
            ArrayList<TestsByDateAndTime> tests = r.getPositiveTests().intervalSearch(t2, t1);
            if (tests.isEmpty())
            {
//                System.out.println("No sick people in region: " + r.getID());
                continue;
            }
            avl.insert(new IntervalsOrderedBySize(tests, r.getID()));
        }
        
        ArrayList<IntervalsOrderedBySize> result = avl.inorderTraversal();
        
        for (int i = result.size() - 1; i > -1; i--)
        {
            output += "\n" + result.get(i).getTests().size() + ", in region: " + result.get(i).getId();
        }
        return output;
    }
    
    // Method 17
    public String printAllTestsFromWorkplace(LocalDateTime from, LocalDateTime to, int workplaceID)
    {
        String output = "";
        TestsByDateAndTime t1 = new TestsByDateAndTime(new PCRTest(from, null, 0, 0, 0, 0, true, 0, null));
        TestsByDateAndTime t2 = new TestsByDateAndTime(new PCRTest(to, null, 0, 0, 0, 0, true, 0, null));
        
        Workplace w = this.allWorkplaces.find(new Workplace(workplaceID, 0));
        if (w == null)
        {
            output = "Specified workplace is not in system.";
            return output;
        }
        
        ArrayList<TestsByDateAndTime> tests = w.getTests().intervalSearch(t1, t2);
        
        if (tests.isEmpty())
        {
            output = "No tests in specified time range.";
            return output;
        }
        
        for (var t : tests)
        {
            output += "\n" + t.getTest().toStringForOutput()+ "\n-----------------------------------------";
        }
        return output;
    }
    
    // Method 18 find PCR test specified by it's code
    public String findPCRTest(int pcrCode)
    {
        String output;
        PCRTest t = new PCRTest(null, null, pcrCode, 0, 0, 0, true, 0.0, null);
        PCRTest foundTest = this.allTests.find(t);
        
        if (foundTest != null)
        {
            output = foundTest.toStringForOutput();
            return output;
        }
        output = "Test not found";
        
        return output;
    }
    
    // Method 19 add patient into system
    public String addPatientIntoSystem(Person patient)
    {
        String output;
        Person p = this.patients.insert(patient);
        if (p != null)
        {
            output = "Patient added: \n" + p.toStringForOutput();
            return output;
        }
        output = "Patient already in system";
        return output;
    }
    
    //Method 20 PCR test delete
    public String deletePCRTest(int pcrCode)
    {
        String output;
        PCRTest t = new PCRTest(null, null, pcrCode, 0, 0, 0, true, 0.0, null);
        PCRTest foundTest = this.allTests.find(t);
        if (foundTest != null)
        {
            this.allTests.delete(foundTest);
            this.testsOrderedByDateAndTime.delete(new TestsByDateAndTime(foundTest));
            
            Person p = this.patients.find(new Person(null, null, null, foundTest.getUniquePatientNumber()));
            Region r = this.regions.find(new Region(foundTest.getRegionCode()));
            District d = this.allDistricts.find(new District(foundTest.getDistrictCode(), foundTest.getRegionCode()));
            Workplace w = this.allWorkplaces.find(new Workplace(foundTest.getUniqueWorkplaceCode(), foundTest.getDistrictCode()));
            
            p.getTests().delete(foundTest);
            r.getTests().delete(new TestsByDateAndTime(foundTest));
            d.getTests().delete(new TestsByDateAndTime(foundTest));
            w.getTests().delete(new TestsByDateAndTime(foundTest));
            
            
            if (foundTest.isTestResult())
            {
                this.positiveTests.delete(new TestsByDateAndTime(foundTest));
                r.getPositiveTests().delete(new TestsByDateAndTime(foundTest));
                d.getPositiveTests().delete(new TestsByDateAndTime(foundTest));
            }
            output = "Test deleted";
            return output;
        }
        output = "Test not found";
        return output;
    }
    
    // Method 21 delete patient from system
    public String deletePatient(String upn)
    {
        String output;
        Person p = new Person(null, null, null, upn);
        Person foundPatient = this.patients.find(p);
        
        if (foundPatient != null)
        {
            ArrayList<PCRTest> tests = foundPatient.getTests().inorderTraversal();
            for (var t : tests)
            {
                this.deletePCRTest(t.getPCRtestCode());
            }
            this.patients.delete(foundPatient);
            output = "Patient deleted";
            return output;
        }
        output = "Patient not found";
        return output;
    }
    
    public String exportData(String fnForPatients, String fnForTests)
    {
        String outputP = "";
        String outputT = "";
        String fullOutput = "";
        
        if (this.patients.exportToCSV(fnForPatients))
        {
            outputP += "Patients data exported successfully";
        }
        else
        {
            outputP += "Error while exporting patients data";
        }
        
        if (this.allTests.exportToCSV(fnForTests))
        {
            outputT = "Tests data exported successfully";
        }
        else
        {
            outputP += "Error while exporting tests data";
        }
        
        fullOutput += outputP + "\n" + outputT;
        return fullOutput;
    }
    
    public void importData(String pathToPatientsData, String pathToTestsData)
    {
        this.importPatients(pathToPatientsData);
        this.importTests(pathToTestsData);
    }
    
    private void importPatients(String path)
    {
        InstanceMaker<Person> factory = (values)->
        {
            if (values.size() == 4)
            {
                DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
                LocalDate date = LocalDate.parse(values.get(2), formatter);
                Person p = new Person(values.get(0), values.get(1), date, values.get(3));
                return p;
            }
            
          return null;  
        };
        
        this.patients.importFromCSV(path, factory);
    }
    
    private void importTests(String path)
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
                
                if (values.size() == 9)
                {
                    LocalDateTime time = LocalDateTime.parse(values.get(0), DateTimeFormatter.ISO_LOCAL_DATE_TIME);
                    
                    int pcrTestCode = Integer.parseInt(values.get(2));
                    int workplaceCode = Integer.parseInt(values.get(3));
                    int districtCode = Integer.parseInt(values.get(4));
                    int regionCode = Integer.parseInt(values.get(5));
                    boolean testResult = Boolean.parseBoolean(values.get(6));
                    double testValue = Double.parseDouble(values.get(7));
                    PCRTest t = new PCRTest(time, values.get(1), pcrTestCode, workplaceCode, districtCode, regionCode, testResult, testValue, values.get(8));
                    this.addPCRTestIntoSystem(t);
                }
                
                lineScanner.close();
            }
            fileScanner.close();
        } 
        catch (FileNotFoundException e) 
        {
            System.out.println("File not found. " + e.getMessage());
        }
    }
}