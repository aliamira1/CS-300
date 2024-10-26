
//================================================================================
// Name        : ProjectTwo.cpp
// Author      : Almera Wardle
// Version     : 1.0
// Description : This program is a course management system that uses a hash 
//               table data structure to efficiently store, search, and 
//               display course information including prerequisites from a file.
//================================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

// Define the Course structure to store course data
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Define the HashTable class
class HashTable {
private:
    struct Node {
        Course course;
        Node* next;

        Node(Course aCourse) : course(aCourse), next(nullptr) {}
    };

    vector<Node*> nodes;
    unsigned int tableSize;

    unsigned int hash(const string& courseNumber) {
        unsigned int hashValue = 0;
        for (char ch : courseNumber) {
            hashValue = (hashValue * 31 + static_cast<unsigned char>(ch)) % tableSize;
        }
        return hashValue;
    }

public:
    // Default constructor
    HashTable(unsigned int size = 179) : tableSize(size) {
        if (size == 0) throw invalid_argument("Hash table size must be greater than 0");
        nodes.resize(tableSize, nullptr);
    }

    // Copy constructor
    HashTable(const HashTable& other) : tableSize(other.tableSize) {
        nodes.resize(tableSize, nullptr);
        for (const Node* node : other.nodes) {
            while (node != nullptr) {
                Insert(node->course);
                node = node->next;
            }
        }
    }

    // Assignment operator
    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            // Clear existing data
            clear();

            // Copy new data
            tableSize = other.tableSize;
            nodes.resize(tableSize, nullptr);
            for (const Node* node : other.nodes) {
                while (node != nullptr) {
                    Insert(node->course);
                    node = node->next;
                }
            }
        }
        return *this;
    }

    // Destructor
    ~HashTable() {
        clear();
    }

    // Clear all nodes
    void clear() {
        for (auto& node : nodes) {
            while (node != nullptr) {
                Node* temp = node;
                node = node->next;
                delete temp;
            }
            node = nullptr;
        }
    }

    // Insert a course object
    void Insert(const Course& course) {
        unsigned int index = hash(course.courseNumber);
        Node* newNode = new Node(course);

        // Insert at the head of the chain
        newNode->next = nodes[index];
        nodes[index] = newNode;
    }

    // Search for the specified course number
    Course* Search(const string& courseNumber) {
        unsigned int index = hash(courseNumber);
        Node* node = nodes[index];

        while (node != nullptr) {
            if (node->course.courseNumber == courseNumber) {
                return &node->course;
            }
            node = node->next;
        }
        return nullptr;
    }

    // Helper function for retrieving all course objects from the hash table
    vector<Course> GetAllCourses() const {
        vector<Course> courseList;
        for (const auto& node : nodes) {
            Node* current = node;
            while (current != nullptr) {
                courseList.push_back(current->course);
                current = current->next;
            }
        }
        return courseList;
    }

    // Helper function to trim whitespace
    static string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        size_t last = str.find_last_not_of(" \t\n\r");
        if (first == string::npos) return "";
        return str.substr(first, (last - first + 1));
    }
};

// Function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * Function to load course data from a file
 *
 * @param filename The name of the file to open
 * @param coursesTable The hash table to load course objects into
 */
bool loadCourseData(const string& filename, HashTable& coursesTable) {
    ifstream file(filename);

    // Check if file opens successfully
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filename << endl;
        return false;
    }

    HashTable courseExistenceTable; // Hash table to check for duplicate courses
    string line;
    int lineNumber = 0;

    // Read each line and validate the format
    while (getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;  // Skip empty lines

        stringstream ss(line);
        string courseNumber, courseTitle, prerequisite;
        vector<string> prerequisites;

        // Parse the line by commas
        if (!getline(ss, courseNumber, ',') || !getline(ss, courseTitle, ',')) {
            cout << "Error: Invalid format at line " << lineNumber << endl;
            continue;
        }

        // Trim whitespace
        courseNumber = HashTable::trim(courseNumber);
        courseTitle = HashTable::trim(courseTitle);

        // Validate course number and title
        if (courseNumber.empty() || courseTitle.empty()) {
            cout << "Error: Empty course number or title at line " << lineNumber << endl;
            continue;
        }

        // Collect and trim prerequisites
        while (getline(ss, prerequisite, ',')) {
            prerequisite = HashTable::trim(prerequisite);
            if (!prerequisite.empty()) {
                prerequisites.push_back(prerequisite);
            }
        }

        // Check for duplicate course numbers
        if (courseExistenceTable.Search(courseNumber)) {
            cout << "Warning: Duplicate course number " << courseNumber << " at line " << lineNumber << endl;
            continue;
        }

        // Create and store the course
        Course course{ courseNumber, courseTitle, prerequisites };
        coursesTable.Insert(course);
        courseExistenceTable.Insert(Course{ courseNumber, "", {} });
    }

    // Close the file
    file.close();
    return true;
}

// Function to display the menu options
void printMenu() {
    cout << "\nMenu Options:" << endl;
    cout << "1. Load data structure" << endl;
    cout << "2. Print course list" << endl;
    cout << "3. Print course information" << endl;
    cout << "9. Exit" << endl;
    cout << "\nEnter your choice: ";
}

// Function to get the user's choice
int getUserChoice() {
    int choice;
    while (!(cin >> choice)) {
        cout << "Invalid input. Please enter a number: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    return choice;
}

/**
 * Function to print the course list in alphanumeric order
 *
 * @param coursesTable The hash table to get course objects from
 */
void printCourseList(HashTable& coursesTable) {
    // Retrieve all courses from the hash table
    vector<Course> courseList = coursesTable.GetAllCourses();

    // Check that the list is not empty
    if (courseList.empty()) {
        cout << "\nNo courses available." << endl;
        return;
    }

    // Sort courses by course number
    sort(courseList.begin(), courseList.end(),
        [](const Course& a, const Course& b) {
            return a.courseNumber < b.courseNumber;
        });

    // Print the sorted course list
    cout << "\nCourse List (Sorted Alphanumerically):" << endl;
    cout << string(50, '-') << endl;
    for (const auto& course : courseList) {
        cout << course.courseNumber << ": " << course.courseTitle << endl;
    }
    cout << string(50, '-') << endl;
}

/**
 * Function to print details for a specific course
 *
 * @param hashTable The hash table that contains course objects
 * @param courseNumber The course number to search for
 */
void printCourseInfo(HashTable& hashTable, const string& courseNumber) {
    // Search for the course in the hash table
    Course* course = hashTable.Search(courseNumber);

    if (course != nullptr) {
        // Print course number and title
        cout << "\nCourse Details:" << endl;
        cout << string(50, '-') << endl;
        cout << "Course Number: " << course->courseNumber << endl;
        cout << "Course Title: " << course->courseTitle << endl;
        // Print prerequisites
        cout << "Prerequisites: ";
        if (course->prerequisites.empty()) {
            cout << "None" << endl;
        }
        else {
            cout << endl;
            for (const auto& prereq : course->prerequisites) {
                cout << "  - " << prereq << endl;
            }
        }
        cout << string(50, '-') << endl;
    }
    else {
        cout << "Course '" << courseNumber << "' not found!" << endl;
    }
}

int main() {
    HashTable hashTable;
    bool dataLoaded = false;

    cout << "\nWelcome to the course planner." << endl;

    while (true) {
        try {
            printMenu();
            int choice = getUserChoice();

            switch (choice) {
            case 1: { // Load course data from the file
                string filename;
                cout << "Enter the full path to the file: ";
                getline(cin, filename);
                filename = HashTable::trim(filename);

                if (filename.empty()) {
                    cout << "Error: Filename cannot be empty." << endl;
                    break;
                }

                if (loadCourseData(filename, hashTable)) {
                    cout << "Data loaded successfully." << endl;
                    dataLoaded = true;
                }
                else {
                    cout << "Failed to load data." << endl;
                }
                break;
            }

            case 2: // Print sorted course list
                if (!dataLoaded) {
                    cout << "Please load data first (Option 1)." << endl;
                    break;
                }
                printCourseList(hashTable);
                break;

            case 3: { // Print the speicified course's details
                if (!dataLoaded) {
                    cout << "Please load data first (Option 1)." << endl;
                    break;
                }
                cout << "Enter course number: ";
                string courseNumber;
                getline(cin, courseNumber);
                courseNumber = HashTable::trim(courseNumber);

                if (courseNumber.empty()) {
                    cout << "Error: Course number cannot be empty." << endl;
                    break;
                }

                printCourseInfo(hashTable, courseNumber);
                break;
            }

            case 9: // Exit the program
                cout << "Thank you for using the course planner!" << endl;
                return 0;

            default: // Invalid user input
                cout << "Invalid option. Please try again." << endl;
                break;
            }
        }
        catch (const exception& e) { // Error occurred
            cout << "An error occurred: " << e.what() << endl;
            cout << "Please try again." << endl;
        }
    }
}