#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <map>

using namespace std;

struct Course {
    string name;
    float credits;
    string grade;
    float points;
};

struct Semester {
    int id;
    vector<Course> courses;
    float totalCredits;
    float totalGradePoints;
    float gpa;
};

// Function prototypes
void displayMenu();
void addSemester(vector<Semester>& semesters);
void removeSemester(vector<Semester>& semesters);
void displaySemesters(const vector<Semester>& semesters);
void manageSemester(vector<Semester>& semesters, int semesterIndex);
void addCourse(vector<Course>& courses);
void removeCourse(vector<Course>& courses);
void editCourse(vector<Course>& courses);
void displayCourses(const vector<Course>& courses);
float calculateSemesterGPA(vector<Course>& courses);
void calculateCGPA(const vector<Semester>& semesters);
float getGradePoint(const string& grade);
void clearScreen();
void waitForEnter();

int main() {
    vector<Semester> semesters;
    int choice;
    
    do {
        displayMenu();
        cout << "Enter your choice: ";
        
        // Input validation for menu choice
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        }
        
        switch (choice) {
            case 1:
                addSemester(semesters);
                break;
            case 2:
                removeSemester(semesters);
                break;
            case 3:
                displaySemesters(semesters);
                if (!semesters.empty()) {
                    int semIndex;
                    cout << "Enter semester number to manage (1-" << semesters.size() << "): ";
                    cin >> semIndex;
                    if (semIndex >= 1 && semIndex <= static_cast<int>(semesters.size())) {
                        manageSemester(semesters, semIndex - 1);
                    } else {
                        cout << "Invalid semester number." << endl;
                    }
                }
                break;
            case 4:
                if (!semesters.empty()) {
                    // Calculate GPA for each semester
                    for (auto& semester : semesters) {
                        semester.gpa = calculateSemesterGPA(semester.courses);
                    }
                    calculateCGPA(semesters);
                } else {
                    cout << "No semesters added yet. Please add at least one semester." << endl;
                }
                break;
            case 0:
                cout << "Exiting program. Thank you for using CGPA Calculator!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            waitForEnter();
        }
        
    } while (choice != 0);
    
    return 0;
}

void displayMenu() {
    clearScreen();
    cout << "===== CGPA CALCULATOR =====" << endl;
    cout << "1. Add Semester" << endl;
    cout << "2. Remove Semester" << endl;
    cout << "3. Manage Semester (Add/Remove/Edit Courses)" << endl;
    cout << "4. Calculate CGPA" << endl;
    cout << "0. Exit" << endl;
    cout << "===========================" << endl;
}

void addSemester(vector<Semester>& semesters) {
    Semester semester;
    semester.id = semesters.size() + 1;
    semester.totalCredits = 0;
    semester.totalGradePoints = 0;
    semester.gpa = 0;
    
    semesters.push_back(semester);
    cout << "Semester " << semester.id << " added successfully." << endl;
    
    // Prompt to add courses to the new semester
    char addCourseChoice;
    cout << "Do you want to add courses to this semester now? (y/n): ";
    cin >> addCourseChoice;
    
    if (tolower(addCourseChoice) == 'y') {
        manageSemester(semesters, semesters.size() - 1);
    }
}

void removeSemester(vector<Semester>& semesters) {
    if (semesters.empty()) {
        cout << "No semesters to remove." << endl;
        return;
    }
    
    displaySemesters(semesters);
    
    int semesterIndex;
    cout << "Enter semester number to remove (1-" << semesters.size() << "): ";
    cin >> semesterIndex;
    
    if (semesterIndex >= 1 && semesterIndex <= static_cast<int>(semesters.size())) {
        semesters.erase(semesters.begin() + semesterIndex - 1);
        
        // Updating semester IDs
        for (size_t i = 0; i < semesters.size(); i++) {
            semesters[i].id = i + 1;
        }
        
        cout << "Semester removed successfully." << endl;
    } else {
        cout << "Invalid semester number." << endl;
    }
}

void displaySemesters(const vector<Semester>& semesters) {
    if (semesters.empty()) {
        cout << "No semesters added yet." << endl;
        return;
    }
    
    cout << "===== SEMESTERS =====" << endl;
    for (const auto& semester : semesters) {
        cout << "Semester " << semester.id << " - Courses: " << semester.courses.size();
        if (semester.gpa > 0) {
            cout << " - GPA: " << fixed << setprecision(2) << semester.gpa;
        }
        cout << endl;
    }
    cout << "====================" << endl;
}

void manageSemester(vector<Semester>& semesters, int semesterIndex) {
    int choice;
    
    do {
        clearScreen();
        cout << "===== MANAGE SEMESTER " << semesters[semesterIndex].id << " =====" << endl;
        cout << "1. Add Course" << endl;
        cout << "2. Remove Course" << endl;
        cout << "3. Edit Course" << endl;
        cout << "4. Display Courses" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "===========================" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                addCourse(semesters[semesterIndex].courses);
                break;
            case 2:
                removeCourse(semesters[semesterIndex].courses);
                break;
            case 3:
                editCourse(semesters[semesterIndex].courses);
                break;
            case 4:
                displayCourses(semesters[semesterIndex].courses);
                break;
            case 0:

                semesters[semesterIndex].gpa = calculateSemesterGPA(semesters[semesterIndex].courses);
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            waitForEnter();
        }
        
    } while (choice != 0);
}

void addCourse(vector<Course>& courses) {
    Course course;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Enter course name: ";
    getline(cin, course.name);
    
    cout << "Enter course credits: ";
    while (!(cin >> course.credits) || course.credits <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a positive number for credits: ";
    }
    
    cout << "Enter course grade (A+, A, A-, B+, B, B-, C+, C, C-, D+, D, F): ";
    cin >> course.grade;
    

    transform(course.grade.begin(), course.grade.end(), course.grade.begin(), ::toupper);
    

    map<string, float> validGrades = {
        {"A+", 4.0}, {"A", 4.0}, {"A-", 3.7},
        {"B+", 3.3}, {"B", 3.0}, {"B-", 2.7},
        {"C+", 2.3}, {"C", 2.0}, {"C-", 1.7},
        {"D+", 1.3}, {"D", 1.0}, {"F", 0.0}
    };
    
    while (validGrades.find(course.grade) == validGrades.end()) {
        cout << "Invalid grade. Please enter a valid grade: ";
        cin >> course.grade;
        transform(course.grade.begin(), course.grade.end(), course.grade.begin(), ::toupper);
    }
    
    course.points = course.credits * getGradePoint(course.grade);
    courses.push_back(course);
    
    cout << "Course added successfully." << endl;
}

void removeCourse(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses to remove." << endl;
        return;
    }
    
    displayCourses(courses);
    
    int courseIndex;
    cout << "Enter course number to remove (1-" << courses.size() << "): ";
    cin >> courseIndex;
    
    if (courseIndex >= 1 && courseIndex <= static_cast<int>(courses.size())) {
        courses.erase(courses.begin() + courseIndex - 1);
        cout << "Course removed successfully." << endl;
    } else {
        cout << "Invalid course number." << endl;
    }
}

void editCourse(vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses to edit." << endl;
        return;
    }
    
    displayCourses(courses);
    
    int courseIndex;
    cout << "Enter course number to edit (1-" << courses.size() << "): ";
    cin >> courseIndex;
    
    if (courseIndex >= 1 && courseIndex <= static_cast<int>(courses.size())) {
        int field;
        Course& course = courses[courseIndex - 1];
        
        cout << "What would you like to edit?" << endl;
        cout << "1. Course Name" << endl;
        cout << "2. Credits" << endl;
        cout << "3. Grade" << endl;
        cout << "Enter your choice: ";
        cin >> field;

        // Validate grade
        map<string, float> validGrades = {
            {"A+", 4.0}, {"A", 4.0}, {"A-", 3.7},
            {"B+", 3.3}, {"B", 3.0}, {"B-", 2.7},
            {"C+", 2.3}, {"C", 2.0}, {"C-", 1.7},
            {"D+", 1.3}, {"D", 1.0}, {"F", 0.0}
        };
        
        switch (field) {
            case 1:
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter new course name: ";
                getline(cin, course.name);
                break;
            case 2:
                cout << "Enter new credits: ";
                while (!(cin >> course.credits) || course.credits <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a positive number for credits: ";
                }
                break;
            case 3:
                cout << "Enter new grade (A+, A, A-, B+, B, B-, C+, C, C-, D+, D, F): ";
                cin >> course.grade;
                transform(course.grade.begin(), course.grade.end(), course.grade.begin(), ::toupper);
                
                
                
                while (validGrades.find(course.grade) == validGrades.end()) {
                    cout << "Invalid grade. Please enter a valid grade: ";
                    cin >> course.grade;
                    transform(course.grade.begin(), course.grade.end(), course.grade.begin(), ::toupper);
                }
                break;
            default:
                cout << "Invalid choice." << endl;
                return;
        }
        
        // Update points
        course.points = course.credits * getGradePoint(course.grade);
        cout << "Course updated successfully." << endl;
    } else {
        cout << "Invalid course number." << endl;
    }
}

void displayCourses(const vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses added yet." << endl;
        return;
    }
    
    cout << "===== COURSES =====" << endl;
    cout << left << setw(5) << "No." << setw(30) << "Course Name" << setw(10) << "Credits" 
         << setw(10) << "Grade" << setw(10) << "Points" << endl;
    cout << string(65, '-') << endl;
    
    for (size_t i = 0; i < courses.size(); i++) {
        cout << left << setw(5) << (i + 1) << setw(30) << courses[i].name 
             << setw(10) << fixed << setprecision(1) << courses[i].credits 
             << setw(10) << courses[i].grade 
             << setw(10) << fixed << setprecision(2) << courses[i].points << endl;
    }
    cout << "===================" << endl;
}

float calculateSemesterGPA(vector<Course>& courses) {
    if (courses.empty()) {
        return 0.0;
    }
    
    float totalCredits = 0.0;
    float totalGradePoints = 0.0;
    
    for (auto& course : courses) {
        totalCredits += course.credits;
        course.points = course.credits * getGradePoint(course.grade);
        totalGradePoints += course.points;
    }
    
    return (totalCredits > 0) ? (totalGradePoints / totalCredits) : 0.0;
}

void calculateCGPA(const vector<Semester>& semesters) {
    if (semesters.empty()) {
        cout << "No semesters added yet." << endl;
        return;
    }
    
    clearScreen();
    cout << "===== CGPA CALCULATION =====" << endl;
    
    float totalCredits = 0.0;
    float totalGradePoints = 0.0;
    
    // Display semester-wise results
    for (const auto& semester : semesters) {
        float semesterCredits = 0.0;
        float semesterGradePoints = 0.0;
        
        cout << "Semester " << semester.id << " - GPA: " << fixed << setprecision(2) << semester.gpa << endl;
        cout << left << setw(30) << "Course Name" << setw(10) << "Credits" 
             << setw(10) << "Grade" << setw(10) << "Points" << endl;
        cout << string(60, '-') << endl;
        
        for (const auto& course : semester.courses) {
            cout << left << setw(30) << course.name 
                 << setw(10) << fixed << setprecision(1) << course.credits 
                 << setw(10) << course.grade 
                 << setw(10) << fixed << setprecision(2) << course.points << endl;
            
            semesterCredits += course.credits;
            semesterGradePoints += course.points;
        }
        
        cout << string(60, '-') << endl;
        cout << left << setw(30) << "Semester Total" 
             << setw(10) << fixed << setprecision(1) << semesterCredits 
             << setw(10) << "" 
             << setw(10) << fixed << setprecision(2) << semesterGradePoints << endl;
        cout << endl;
        
        totalCredits += semesterCredits;
        totalGradePoints += semesterGradePoints;
    }
    
    // Calculate and display CGPA
    float cgpa = (totalCredits > 0) ? (totalGradePoints / totalCredits) : 0.0;
    
    cout << string(60, '=') << endl;
    cout << "Total Credits: " << fixed << setprecision(1) << totalCredits << endl;
    cout << "Total Grade Points: " << fixed << setprecision(2) << totalGradePoints << endl;
    cout << "CUMULATIVE GPA (CGPA): " << fixed << setprecision(2) << cgpa << endl;
    cout << string(60, '=') << endl;
}

float getGradePoint(const string& grade) {
    map<string, float> gradePoints = {
        {"A+", 4.0}, {"A", 4.0}, {"A-", 3.5},
        {"B+", 3.3}, {"B", 3.0}, {"B-", 2.7},
        {"C+", 2.3}, {"C", 2.0}, {"C-", 1.7},
        {"D+", 1.3}, {"D", 1.0}, {"F", 0.0}
    };
    
    return gradePoints[grade];
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
