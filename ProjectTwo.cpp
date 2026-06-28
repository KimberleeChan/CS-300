#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <limits>
#include <algorithm>

using namespace std;

// Remove extra spaces from the beginning and end of a string
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Convert a string to uppercase so searches are not case-sensitive
string toUpper(string str) {
    for (char& c : str) {
        c = toupper(c);
    }
    return str;
}

// Store information for a single course
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Store one course in the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// Binary search tree used to store the courses
class BinarySearchTree {
private:
    Node* root;

    void inOrder(Node* node);
    Node* search(Node* node, string courseNumber);

public:
    BinarySearchTree();
    void Insert(Course course);
    void PrintCourseList();
    void PrintCourse(string courseNumber);
};

// Start with an empty tree
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Add a course to the tree
void BinarySearchTree::Insert(Course course) {

    Node* newNode = new Node(course);

    if (root == nullptr) {
        root = newNode;
        return;
    }

    Node* current = root;

    while (true) {

        if (course.courseNumber < current->course.courseNumber) {

            if (current->left == nullptr) {
                current->left = newNode;
                return;
            }

            current = current->left;
        }
        else {

            if (current->right == nullptr) {
                current->right = newNode;
                return;
            }

            current = current->right;
        }
    }
}

// Print the courses in order
void BinarySearchTree::inOrder(Node* node) {

    if (node == nullptr) return;

    inOrder(node->left);

    cout << node->course.courseNumber << ", "
        << node->course.courseName << endl;

    inOrder(node->right);
}

// Display full course list
void BinarySearchTree::PrintCourseList() {

    cout << "Here is a sample schedule:" << endl << endl;
    inOrder(root);
}

// Search for a course in the tree
Node* BinarySearchTree::search(Node* node, string courseNumber) {

    courseNumber = toUpper(courseNumber);

    while (node != nullptr) {

        if (node->course.courseNumber == courseNumber) {
            return node;
        }

        if (courseNumber < node->course.courseNumber) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }

    return nullptr;
}

// Display one course and its prerequisites
void BinarySearchTree::PrintCourse(string courseNumber) {

    courseNumber = trim(toUpper(courseNumber));

    // Basic validation for course format
    if (courseNumber.length() < 4 ||
        !isalpha(courseNumber[0]) ||
        courseNumber.find_first_of("0123456789") == string::npos) {

        cout << "Invalid course format. Try something like CSCI100."
            << endl;
        return;
    }

    Node* result = search(root, courseNumber);

    if (result == nullptr) {
        cout << "Course '" << courseNumber << "' not found."
            << endl;
        return;
    }

    cout << result->course.courseNumber << ", "
        << result->course.courseName << endl;

    cout << "Prerequisites: ";

    if (result->course.prerequisites.empty()) {
        cout << "None" << endl;
    }
    else {
        for (size_t i = 0;
            i < result->course.prerequisites.size();
            i++) {

            cout << result->course.prerequisites[i];

            if (i < result->course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// Load file and store courses in the tree
void loadCourses(string fileName, BinarySearchTree* bst) {

    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file "
            << fileName << endl;
        return;
    }

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        string courseNumber;
        string courseName;
        string prereq;

        Course course;

        getline(ss, courseNumber, ',');
        course.courseNumber = toUpper(trim(courseNumber));

        getline(ss, courseName, ',');
        course.courseName = trim(courseName);

        while (getline(ss, prereq, ',')) {
            course.prerequisites.push_back(
                toUpper(trim(prereq))
            );
        }

        bst->Insert(course);
    }

    file.close();

    cout << "File loaded successfully." << endl;
}

// Main menu loop
int main() {

    BinarySearchTree bst;
    bool loaded = false;

    int choice = 0;
    string courseNumber;

    cout << "Welcome to the course planner." << endl
        << endl;

    while (choice != 9) {

        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;

        cout << endl
            << "What would you like to do? ";

        string input;
        getline(cin, input);
        input = trim(input);

        bool isNumber =
            !input.empty() &&
            all_of(input.begin(),
                input.end(),
                ::isdigit);

        if (!isNumber) {
            cout << input
                << " is not a valid option."
                << endl << endl;
            continue;
        }

        choice = stoi(input);

        switch (choice) {

        case 1:
            loadCourses(
                "CS 300 ABCU_Advising_Program_Input.csv",
                &bst
            );
            loaded = true;
            break;

        case 2:
            if (!loaded) {
                cout << "Please load the data structure first."
                    << endl;
                break;
            }
            bst.PrintCourseList();
            break;

        case 3:
            if (!loaded) {
                cout << "Please load the data structure first."
                    << endl;
                break;
            }

            cout << "What course do you want to know about? ";
            cin >> courseNumber;

            courseNumber = trim(courseNumber);
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            bst.PrintCourse(courseNumber);
            break;

        case 9:
            cout << "Thank you for using the course planner!"
                << endl;
            break;

        default:
            cout << choice
                << " is not a valid option."
                << endl;
            break;
        }

        cout << endl;
    }

    return 0;
}