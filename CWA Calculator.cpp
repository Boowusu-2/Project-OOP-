#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <Shlobj.h> // For Windows-specific function
#pragma comment(lib, "Shell32.lib") // For linking with the Shell32 library

using namespace std;

class Course
{
private:
    int creditHours;
    float grade;
    string courseName;

public:
    Course(int creditHours, const string& courseName, float grade)
        : creditHours(creditHours), courseName(courseName), grade(grade) {}

    double getCreditHours() const
    {
        return creditHours;
    }

    float getGrade() const
    {
        return grade;
    }

    string getCourseName() const
    {
        return courseName;
    }
};

class Transcript
{
private:
    vector<Course> courses;
    string studentName;
    int studentID;
    int studentPassword;

public:
    // Function to set student information
    void setStudentInfo(const string& name, int id, int password)
    {
        studentName = name;
        studentID = id;
        studentPassword = password;
    }

    // Function to add a course to the transcript
    void addCourse(const Course& course)
    {
        courses.push_back(course);
    }

    // Function to calculate the previous CWA for a specific semester
    double calcPreviousCWA(int semester) const
    {
        double previousCWA = 0.0;

        if (semester > 1)
        {
            cout << "Enter your previous CWA for Semester " << semester - 1 << ": ";
            cin >> previousCWA;
        }

        return previousCWA;
    }

    // Function to calculate the previous total credit hours for a specific semester
    double calcPreviousTotalCreditHours(int semester) const
    {
        double ptotalCreditHours = 0.0;

        if (semester > 1)
        {
            cout << "Enter the total Credit hours for Semester " << semester - 1 << ": ";
            cin >> ptotalCreditHours;
        }

        return ptotalCreditHours;
    }

    // Function to calculate the cumulative weighted average for a specific semester
    double calcCWA(int semester, double previousCWA, double ptotalCreditHours) const
    {
        double totalCreditHours = 0.0;
        double weightedAverage = 0.0;

        if (semester == 2)
        {
            cout << "Enter your previous CWA for Semester 1: ";
            cin >> previousCWA;
            cout << "Enter the total Credit hours for Semester 1: ";
            cin >> ptotalCreditHours;
        }

        for (const Course& course : courses)
        {
            double creditHours = course.getCreditHours();
            double grade = course.getGrade();

            totalCreditHours += creditHours;
            weightedAverage += (creditHours * grade);
        }

        if (semester == 2)
        {
            totalCreditHours += ptotalCreditHours;
            weightedAverage += (previousCWA * ptotalCreditHours);
        }

        if (totalCreditHours == 0.0)
        {
            return 0.0;
        }

        return weightedAverage / totalCreditHours;
    }

    // Function to display the current transcript in table format
    void displayTranscript() const
    {
        cout << "Transcript for " << studentName << " (ID: " << studentID << "):" << endl << endl;
        cout << setw(60) << left << "Course";
        cout << setw(48) << left << "Credit Hours";
        cout << setw(24) << left << "Grade" << endl;

        for (const Course& course : courses)
        {
            cout << setw(60) << left << course.getCourseName();
            cout << setw(48) << left << fixed << setprecision(2) << course.getCreditHours();
            cout << setw(24) << left << fixed << setprecision(2) << course.getGrade() << endl;
        }
    }

    // Function to save the transcript to a file
    void saveToFile(const string& filename) const
    {
        ofstream outputFile(filename);

        if (!outputFile.is_open())
        {
            cout << "Error opening the file. Transcript not saved." << endl;
            return;
        }

        outputFile << "Transcript for " << studentName << " (ID: " << studentID << "):" << endl;
        outputFile << "Year: " << currentYear << endl;
        outputFile << endl;
        outputFile << setw(60) << left << "Course";
        outputFile << setw(48) << left << "Credit Hours";
        outputFile << setw(24) << left << "Grade" << endl;

        for (const Course& course : courses)
        {
            outputFile << setw(60) << left << course.getCourseName();
            outputFile << setw(48) << left << fixed << setprecision(2) << course.getCreditHours();
            outputFile << setw(24) << left << fixed << setprecision(2) << course.getGrade() << endl;
        }

        outputFile.close();

        cout << "Transcript saved to " << filename << endl;
    }

    // Function to display the saved transcript from a file
    void displaySavedTranscript(const string& filename) const
    {
        ifstream inputFile(filename);

        if (!inputFile.is_open())
        {
            cout << "Error opening the file. Cannot display saved transcript." << endl;
            return;
        }

        cout << "Saved Transcript Contents:" << endl;
        string line;
        while (getline(inputFile, line))
        {
            cout << line << endl;
        }

        inputFile.close();
    }

    // Function to get the Documents folder path for saving the transcript
    string getDocumentsFolderPath() const
    {
        char path[MAX_PATH];
        if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, path) == S_OK)
        {
            return string(path);
        }
        return "";
    }

    void setCurrentYear(int year)
    {
        currentYear = year;
    }

private:
    int currentYear;
};

// Function to get input data from the user and add courses to the transcript
void getInputData(Transcript& transcript)
{
    int numCourses;
    cout << "Enter the number of courses: ";
    cin >> numCourses;

    cin.ignore(); // Clear the input buffer

    for (int i = 0; i < numCourses; i++)
    {
        cout << "Course #" << i + 1 << endl;

        string name;
        double creditHours;
        double grade;

        cout << "Course Name: ";
        getline(cin, name);

        cout << "Credit Hours: ";
        cin >> creditHours;

        cout << "Grade: ";
        cin >> grade;

        cin.ignore(); // Clear the input buffer

        transcript.addCourse(Course(creditHours, name, grade));
    }
}

// Function to save new user info to a file
void saveNewUserInfo(const string& filename, const string& name, int id, int password)
{
    ofstream outputFile(filename, ios::app);

    if (!outputFile.is_open())
    {
        cout << "Error opening the file. User info not saved." << endl;
        return;
    }

    outputFile << name << "," << id << "," << password << endl;
    outputFile.close();
}

// Function to check if a user exists in the file
bool isExistingUser(const string& filename, int id, int password)
{
    ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        cout << "Error opening the file. Cannot check for existing users." << endl;
        return false;
    }

    string line;
    while (getline(inputFile, line))
    {
        string name;
        int savedID;
        int savedPassword;
        stringstream ss(line);

        getline(ss, name, ',');
        ss >> savedID;
        ss.ignore(); // Clear the comma
        ss >> savedPassword;

        if (id == savedID && password == savedPassword)
        {
            inputFile.close();
            return true;
        }
    }

    inputFile.close();
    return false;
}

int main()
{
    string savedDataFile = "savedData.txt";
    string studentName;
    int studentID;
    int studentPassword;
    int semester;
    int currentYear;

    cout << "______________________________________________________" << endl;
    cout << "           Welcome to the Transcript System          " << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "Are you a new user? (1 for yes, 0 for no): " << flush;
    bool isNewUser;
    cin >> isNewUser;

    if (isNewUser)
    {
        cin.ignore(); // Clear the input buffer

        cout << "Enter your name: ";
        getline(cin, studentName);

        cout << "Enter your student ID: ";
        cin >> studentID;

        cout << "Create a password: ";
        cin >> studentPassword;

        // Save new user info to file
        saveNewUserInfo(savedDataFile, studentName, studentID, studentPassword);
    }
    else
    {
        cout << "Enter your student ID: ";
        cin >> studentID;

        cout << "Enter your password: ";
        cin >> studentPassword;

        // Check if the user exists in the file
        if (!isExistingUser(savedDataFile, studentID, studentPassword))
        {
            cout << "User not found or incorrect credentials. Exiting application." << endl;
            exit(0);
        }
        else
        {
            // TODO: Load the existing student information and transcript data here if needed
        }
    }

    Transcript transcript;
    transcript.setStudentInfo(studentName, studentID, studentPassword);

    cout << "Enter your current year (1 for the first year, 2 for the second, and so on): ";
    cin >> currentYear;

    transcript.setCurrentYear(currentYear);

    getInputData(transcript);

    cout << "Choose the semester to calculate CWA (1 or 2): ";
    cin >> semester;

    // Get previous CWA and previous total credit hours
    double previousCWA = transcript.calcPreviousCWA(semester);
    double ptotalCreditHours = transcript.calcPreviousTotalCreditHours(semester);

    // Calculating cumulative weighted average for the chosen semester
    double CWA = transcript.calcCWA(semester, previousCWA, ptotalCreditHours);

    string filename;
    cout << "Enter filename to save transcript: ";

    string documentsPath = transcript.getDocumentsFolderPath();
    cin.ignore(); // Clear the input buffer
    getline(cin, filename);

    string fullPath = documentsPath + "\\" + filename + ".txt";

    transcript.saveToFile(fullPath);

    cout << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "Transcript saved to " << fullPath << endl;
    cout << "------------------------------------------------------" << endl;

    // Displaying the transcript in table format
    transcript.displayTranscript();

    cout << "------------------------------------------------------" << endl;
    cout << "Cumulative Weighted Average: " << fixed << setprecision(2) << CWA << endl;
    cout << "------------------------------------------------------" << endl;

    // Call the function to display the saved transcript
    transcript.displaySavedTranscript(fullPath);

    return 0;
}
