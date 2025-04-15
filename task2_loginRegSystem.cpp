#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>  // For getch() functionality
#include <filesystem>
#include <algorithm>

using namespace std;

// Structure to store user information
struct User {
    string username;
    string password;
    string fullName;
    string email;
};

// Function prototypes
void displayMenu();
void registerUser();
bool loginUser();
bool usernameExists(const string& username);
void viewUserProfile(const string& username);
void displayHeader();
void displayFooter();
string getPasswordInput();
void createDataDirectory();

// Global variables
const string DATA_DIR = "user_data";

int main() {
    // Create data directory if it doesn't exist
    createDataDirectory();
    
    int choice;
    bool exitProgram = false;
    
    while (!exitProgram) {
        system("cls"); // Clear screen (for Windows)
        displayHeader();
        displayMenu();
        displayFooter();
        
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    cout << "\nLogin successful!" << endl;
                } else {
                    cout << "\nLogin failed. Invalid username or password." << endl;
                }
                break;
            case 3:
                exitProgram = true;
                cout << "\nExiting program. Thank you for using our system!" << endl;
                break;
            default:
                cout << "\nInvalid choice. Please try again." << endl;
        }
        
        if (!exitProgram) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    }
    
    return 0;
}

// Function to create data directory if it doesn't exist
void createDataDirectory() {
    if (!filesystem::exists(DATA_DIR)) {
        filesystem::create_directory(DATA_DIR);
    }
}

// Function to display the program header
void displayHeader() {
    cout << "=======================================================" << endl;
    cout << "           LOGIN AND REGISTRATION SYSTEM               " << endl;
    cout << "=======================================================" << endl;
}

// Function to display the program footer
void displayFooter() {
    cout << "=======================================================" << endl;
    cout << "       (C) 2025 - Your Secure Login System             " << endl;
    cout << "=======================================================" << endl;
}

// Function to display the main menu
void displayMenu() {
    cout << "\nMAIN MENU\n" << endl;
    cout << "1. Register New User" << endl;
    cout << "2. Login to Existing Account" << endl;
    cout << "3. Exit" << endl;
}

// Function to securely get password input (masks with asterisks)
string getPasswordInput() {
    string password;
    char ch;
    
    cout << "Password (It must be atleast 6 charachters long): ";
    
    ch = _getch();
    while (ch != 13) { // 13 is Enter key
        if (ch == 8) { // 8 is Backspace key
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
        ch = _getch();
    }
    
    cout << endl;
    return password;
}

// Function to check if a username already exists
bool usernameExists(const string& username) {
    string filename = DATA_DIR + "/" + username + ".dat";
    ifstream file(filename);
    return file.good();
}

// Function to register a new user
void registerUser() {
    User newUser;
    
    system("cls"); // Clear screen
    displayHeader();
    cout << "\n             USER REGISTRATION                 \n" << endl;
    
    cout << "Enter username: ";
    getline(cin, newUser.username);
    
    // Check if username contains any spaces
    if (newUser.username.find(' ') != string::npos) {
        cout << "\nError: Username cannot contain spaces." << endl;
        return;
    }
    
    // Check if username already exists
    if (usernameExists(newUser.username)) {
        cout << "\nError: Username already exists. Please choose another username." << endl;
        return;
    }
    
    newUser.password = getPasswordInput();
    
    // Password validation
    if (newUser.password.length() < 6) {
        cout << "\nError: Password must be at least 6 characters long." << endl;
        return;
    }
    
    cout << "Enter full name: ";
    getline(cin, newUser.fullName);
    
    cout << "Enter email address: ";
    getline(cin, newUser.email);
    
    // Basic email validation (check for @ and .)
    if (newUser.email.find('@') == string::npos || newUser.email.find('.') == string::npos) {
        cout << "\nError: Invalid email format." << endl;
        return;
    }
    
    // Save user data to file
    string filename = DATA_DIR + "/" + newUser.username + ".dat";
    ofstream file(filename);
    
    if (file.is_open()) {
        file << "Username: " << newUser.username << endl;
        file << "Password: " << newUser.password << endl;
        file << "FullName: " << newUser.fullName << endl;
        file << "Email: " << newUser.email << endl;
        file.close();
        
        cout << "\nRegistration successful! You can now login with your credentials." << endl;
    } else {
        cout << "\nError: Unable to create user file." << endl;
    }
}

// Function to log in a user
bool loginUser() {
    string username;
    string password;
    
    system("cls"); // Clear screen
    displayHeader();
    cout << "\n                USER LOGIN                  \n" << endl;
    
    cout << "Enter username: ";
    getline(cin, username);
    
    password = getPasswordInput();
    
    // Check if user exists
    if (!usernameExists(username)) {
        return false;
    }
    
    // Read user data from file
    string filename = DATA_DIR + "/" + username + ".dat";
    ifstream file(filename);
    
    string line;
    string storedPassword;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.substr(0, 10) == "Password: ") {
                storedPassword = line.substr(10);
                break;
            }
        }
        file.close();
        
        // Check if password matches
        if (password == storedPassword) {
            viewUserProfile(username);
            return true;
        }
    }
    
    return false;
}

// Function to display user profile after successful login
void viewUserProfile(const string& username) {
    system("cls"); // Clear screen
    displayHeader();
    cout << "\n             USER PROFILE                 \n" << endl;
    
    string filename = DATA_DIR + "/" + username + ".dat";
    ifstream file(filename);
    
    if (file.is_open()) {
        string line;
        vector<string> profileData;
        
        while (getline(file, line)) {
            // Skip password line for security
            if (line.substr(0, 10) != "Password: ") {
                profileData.push_back(line);
            }
        }
        file.close();
        
        cout << "Welcome back!" << endl;
        cout << "\nYour Profile Information:" << endl;
        for (const string& data : profileData) {
            cout << data << endl;
        }
        
        cout << "\nLogin Time: " << __DATE__ << " " << __TIME__ << endl;
    } else {
        cout << "Error: Unable to retrieve user data." << endl;
    }
}
