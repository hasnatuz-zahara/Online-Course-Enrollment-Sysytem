#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

// =========ANSI Color Codes===========
#define RESET "\033[0m"
#define BRIGHT_GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define CYAN "\033[36m"
#define WHEAT "\033[1;33m"
#define GRAY "\033[1;30m"

// ==================== DATA STRUCTURES ====================
// Course Structure
struct Course {
    string courseId;
    string courseName;
    double credits;

    Course() : courseId(""), courseName(""), credits(0.0) {}
    Course(string id, string name, double cred)
        : courseId(id), courseName(name), credits(cred) {}
};

// Student Structure
struct EnrolledCourse {
    string courseId;
    bool isFromSortedArray;
};

struct Student {
    string studentId;
    string password;
    vector<EnrolledCourse> enrolledCourses;

    Student() : studentId(""), password("") {}
    Student(string id, string pass)
        : studentId(id), password(pass) {}
};

// Node for Linked List
struct Node {
    Course data;        //Course information
    Node* next;           // Next node pointer

    Node(Course course) : data(course), next(nullptr) {}
};

// ========UTILITY & DESIGN FUNCTIONS=======
void printHeader(string title) {
    cout << CYAN << "\n+";
    for(int i = 0; i < 48; i++) cout << "-";
    cout << "+" << RESET << "\n";
    cout << CYAN << "| ";
    int spaces = (46 - title.length()) / 2;
    for(int i = 0; i < spaces; i++) cout << " ";
    cout << title;
    for(int i = 0; i < 46 - spaces - title.length(); i++) cout << " ";
    cout << " |" << RESET << "\n";
    cout << CYAN << "+";
    for(int i = 0; i < 48; i++) cout << "-";
    cout << "+" << RESET << "\n";
}

void printMenuOption(int num, string option) {
    cout << CYAN << "| " << WHEAT << num << ". " << option;
    int spaces = 42 - option.length() - 3;
    for(int i = 0; i < spaces; i++) cout << " ";
    cout << CYAN << " |" << RESET << "\n";
}

void printFooter() {
    cout << CYAN << "+";
    for(int i = 0; i < 48; i++) cout << "-";
    cout << "+" << RESET << "\n";
}

void printPrompt(string prompt) {
    cout << "\n" << WHEAT << prompt << RESET;
}

void printSuccess(string message) {
    cout << BRIGHT_GREEN << message << RESET << endl;
}

void printError(string message) {
    cout << RED << message << RESET << endl;
}

void printContinue() {
    cout << GRAY << "Press any key to continue..." << RESET;
}

void clearScreen() {
    system("cls");
}

void waitForKey() {
    cin.ignore();
    cin.get();
}

// ==================== DATA STRUCTURE IMPLEMENTATIONS ====================
//  1. Sorted Array
class SortedArray {
private:
    vector<Course> courses;
public:
    void insertCourse(Course course) {
        auto it = lower_bound(courses.begin(), courses.end(), course,
                            [](const Course& a, const Course& b) {
                                return a.courseId < b.courseId;
                            });
        courses.insert(it, course);
    }
    void deleteCourse(string courseId) {
        auto it = find_if(courses.begin(), courses.end(),
                         [courseId](const Course& c) {
                             return c.courseId == courseId;
                         });
        if (it != courses.end()) {
            courses.erase(it);
            printSuccess("Course deleted successfully from sorted array!");
        } else {
            printError("Course not found in sorted array!");
        }
    }
    void searchCourse(string courseId) {
        auto it = find_if(courses.begin(), courses.end(),
                         [courseId](const Course& c) {
                             return c.courseId == courseId;
                         });
        if (it != courses.end()) {
            printSuccess("Course found in sorted array at position: " + to_string(it - courses.begin() + 1));
            cout << WHEAT << "Course ID: " << RESET << it->courseId << endl;
            cout << WHEAT << "Course Name: " << RESET << it->courseName << endl;
            cout << WHEAT << "Credits: " << RESET << fixed << setprecision(2) << it->credits << endl;
        } else {
            printError("Course not found in sorted array!");
        }
    }

    void displayCourses() {
        if (courses.empty()) {
            printError("No courses in sorted array!");
            return;
        }
        printHeader("SORTED ARRAY COURSES");

        for (size_t i = 0; i < courses.size(); i++) {
            cout << CYAN << "| " << WHEAT << (i + 1) << ". ID: " << courses[i].courseId
                 << ", Name: " << courses[i].courseName
                 << ", Credits: " << fixed << setprecision(2) << courses[i].credits;
            int spaces = 46 - to_string(i + 1).length() - 6 - courses[i].courseId.length()
                         - courses[i].courseName.length() - 5;
            for(int j = 0; j < spaces; j++) cout << " ";
            cout << CYAN << " |" << RESET << "\n";
        }
        printFooter();
    }

    vector<Course> getAllCourses() {
        return courses;
    }

    Course* findCourse(string courseId) {
        auto it = find_if(courses.begin(), courses.end(),
                         [courseId](const Course& c) {
                             return c.courseId == courseId;
                         });
        return (it != courses.end()) ? &(*it) : nullptr;
    }
};

// ======2. Linked List=======
class LinkedList {
private:
    Node* head;
public:
    LinkedList() : head(nullptr) {}

    void insertCourse(Course course) {
        Node* newNode = new Node(course);
        if (!head || head->data.courseId > course.courseId) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;
            while (current->next && current->next->data.courseId < course.courseId) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    void deleteCourse(string courseId) {
        if (!head) {
            printError("Linked list is empty!");
            return;
        }
        if (head->data.courseId == courseId) {
            Node* temp = head;
            head = head->next;
            delete temp;
            printSuccess("Course deleted successfully from linked list!");
            return;
        }
        Node* current = head;
        while (current->next && current->next->data.courseId != courseId) {
            current = current->next;
        }
        if (current->next) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            printSuccess("Course deleted successfully from linked list!");
        } else {
            printError("Course not found in linked list!");
        }
    }

    void searchCourse(string courseId) {
        Node* current = head;
        int position = 1;
        while (current) {
            if (current->data.courseId == courseId) {
                printSuccess("Course found in linked list at position: " + to_string(position));
                cout << WHEAT << "Course ID: " << RESET << current->data.courseId << endl;
                cout << WHEAT << "Course Name: " << RESET << current->data.courseName << endl;
                cout << WHEAT << "Credits: " << RESET << fixed << setprecision(2) << current->data.credits << endl;
                return;
            }
            current = current->next;
            position++;
        }
        printError("Course not found in linked list!");
    }

    void displayCourses() {
        if (!head) {
            printError("No courses in linked list!");
            return;
        }
        printHeader("LINKED LIST COURSES");
        Node* current = head;
        int count = 1;
        while (current) {
            cout << CYAN << "| " << WHEAT << count << ". ID: " << current->data.courseId
                 << ", Name: " << current->data.courseName
                 << ", Credits: " << fixed << setprecision(2) << current->data.credits;
            int spaces = 46 - to_string(count).length() - 6 - current->data.courseId.length()
                         - current->data.courseName.length() - 5;
            for(int j = 0; j < spaces; j++) cout << " ";
            cout << CYAN << " |" << RESET << "\n";
            current = current->next;
            count++;
        }
        printFooter();
    }

    vector<Course> getAllCourses() {
        vector<Course> courses;
        Node* current = head;
        while (current) {
            courses.push_back(current->data);
            current = current->next;
        }
        return courses;
    }
    Course* findCourse(string courseId) {
        Node* current = head;
        while (current) {
            if (current->data.courseId == courseId) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    ~LinkedList() {      //Destructor - memory leak prevent
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};



// ======= 3. Stack==========
class Stack {
private:
    vector<Course> courses;
public:
    void pushCourse(Course course) {
        courses.push_back(course);      //add on  top
    }

    void popCourse() {
        if (!courses.empty()) {
            courses.pop_back();       //remove from top
        }
    }

    Course* peekCourse() {
        if (!courses.empty()) {
            return &courses.back();    //return top element
        }
        return nullptr;
    }

    bool isEmpty() {
        return courses.empty();
    }

    void displayCourses() {
        if (courses.empty()) {
                //chcek if stack is empty
            printError("No courses in stack!");
            return;
        }
        printHeader("STACK COURSES (LIFO ORDER)");
        for (int i = courses.size() - 1; i >= 0; i--) {
            cout << CYAN << "| " << WHEAT << (courses.size() - i) << ". ID: " << courses[i].courseId
                 << ", Name: " << courses[i].courseName
                 << ", Credits: " << fixed << setprecision(2) << courses[i].credits;
            int spaces = 46 - to_string(courses.size() - i).length() - 6 - courses[i].courseId.length()
                         - courses[i].courseName.length() - 5;
            for(int j = 0; j < spaces; j++) cout << " ";
            cout << CYAN << " |" << RESET << "\n";
        }
        printFooter();
    }
    vector<Course> getAllCourses() {
        return courses;
    }

    Course* findCourse(string courseId) {
        for (auto it = courses.rbegin(); it != courses.rend(); ++it) {
            if (it->courseId == courseId) {
                return &(*it);
            }
        }
        return nullptr;
    }
};



//=======4. Queue===========
class Queue {
private:
    vector<Course> courses;
public:
    void enqueueCourse(Course course) {
        courses.push_back(course);        //add at last
    }

    void dequeueCourse() {
        if (!courses.empty()) {
            courses.erase(courses.begin());      //removes from front
            printSuccess("Course dequeued successfully from queue!");
        } else {
            printError("Queue is empty!");
        }
    }

    Course* frontCourse() {
        if (!courses.empty()) {
            return &courses.front();    //return front element
        }
        return nullptr;
    }

    bool isEmpty() {
        return courses.empty();
    }

    void displayCourses() {
        if (courses.empty()) {
            printError("No courses in queue!");
            return;
        }
        printHeader("QUEUE COURSES (FIFO ORDER)");
        for (size_t i = 0; i < courses.size(); i++) {
            cout << CYAN << "| " << WHEAT << (i + 1) << ". ID: " << courses[i].courseId
                 << ", Name: " << courses[i].courseName
                 << ", Credits: " << fixed << setprecision(2) << courses[i].credits;
            int spaces = 46 - to_string(i + 1).length() - 6 - courses[i].courseId.length()
                         - courses[i].courseName.length() - 5;
            for(int j = 0; j < spaces; j++) cout << " ";
            cout << CYAN << " |" << RESET << "\n";
        }
        printFooter();
    }

    vector<Course> getAllCourses() {
        return courses;
    }

    Course* findCourse(string courseId) {
        for (auto it = courses.begin(); it != courses.end(); ++it) {
            if (it->courseId == courseId) {
                return &(*it);
            }
        }
        return nullptr;
    }
};

// ==================== FILE MANAGEMENT ====================
class FileManager {
public:
    static bool saveCourses(const vector<Course>& courses, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        for (const auto& course : courses) {
            file << course.courseId << "," << course.courseName << ","
                 << fixed << setprecision(2) << course.credits << endl;
        }
        file.close();
        return true;
    }

    static vector<Course> loadCourses(const string& filename) {
        vector<Course> courses;
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, creditsStr;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, creditsStr);

            if (!id.empty()) {
                courses.push_back(Course(id, name, stod(creditsStr)));
            }
        }
        file.close();
        return courses;
    }

    static bool saveStudents(const vector<Student>& students, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        for (const auto& student : students) {
            file << student.studentId << "," << student.password << ",";
            for (size_t i = 0; i < student.enrolledCourses.size(); i++) {
                file << student.enrolledCourses[i].courseId << ":"
                     << (student.enrolledCourses[i].isFromSortedArray ? "1" : "0");
                if (i < student.enrolledCourses.size() - 1) file << ";";
            }
            file << endl;
        }
        file.close();
        return true;
    }

    static vector<Student> loadStudents(const string& filename) {
        vector<Student> students;
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string id, password, coursesStr;

            getline(ss, id, ',');
            getline(ss, password, ',');
            getline(ss, coursesStr);

            if (!id.empty()) {
                Student student(id, password);

                if (!coursesStr.empty()) {
                    stringstream coursesSS(coursesStr);
                    string courseEntry;
                    while (getline(coursesSS, courseEntry, ';')) {
                        size_t colonPos = courseEntry.find(':');
                        if (colonPos != string::npos) {
                            string courseId = courseEntry.substr(0, colonPos);
                            bool isFromSA = (courseEntry.substr(colonPos+1) == "1");
                            student.enrolledCourses.push_back({courseId, isFromSA});
                        }
                    }
                }
                students.push_back(student);
            }
        }
        file.close();
        return students;
    }
};

// ==================== MAIN SYSTEM CLASS ====================
class CourseManagementSystem {
private:
    SortedArray sortedArray;
    LinkedList linkedList;
    Stack stack;
    Queue queue;
    vector<Student> students;
    string currentStudentId;

public:
    CourseManagementSystem() {
        loadData();
    }

    ~CourseManagementSystem() {
        saveData();
    }

    void loadData() {
        vector<Course> courses = FileManager::loadCourses("courses.txt");
        for (const auto& course : courses) {
            sortedArray.insertCourse(course);
            linkedList.insertCourse(course);
            stack.pushCourse(course);
             queue.enqueueCourse(course);
        }
        students = FileManager::loadStudents("students.txt");
    }

   bool saveData() {
    FileManager::saveCourses(sortedArray.getAllCourses(), "courses.txt");
    FileManager::saveStudents(students, "students.txt");
    return true;
}

// ==================== MENU SYSTEM ====================
    void mainMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("ONLINE COURSE ENROLLMENT SYSTEM");
            printMenuOption(1, "Admin Panel");
            printMenuOption(2, "Student Login");
            printMenuOption(3, "Register as Student");
            printMenuOption(4, "Exit");
            printFooter();
            printPrompt("Enter your choice: ");
            cin >> choice;

            switch (choice) {
                case 1:
                    adminLogin();
                    break;
                case 2:
                    studentLogin();
                    break;
                case 3:
                    registerStudent();
                    break;
                case 4:
                    printSuccess("Exiting system...");
                    break;
                default:
                    printError("Invalid choice!");
                    printContinue();
                    waitForKey();
            }
        } while (choice != 4);
    }

    void adminLogin() {
        clearScreen();
        string id, password;
        printHeader("ADMIN LOGIN");
        printPrompt("Enter Admin ID: ");
        cin >> id;
        printPrompt("Enter Password: ");
        cin >> password;

        if (id == "admin" && password == "admin123") {
            printSuccess("Login successful!");
            waitForKey();
            adminPanel();
        } else {
            printError("Invalid credentials!");
            printContinue();
            waitForKey();
        }
    }
    void studentLogin() {
        clearScreen();
        string id, password;
        printHeader("STUDENT LOGIN");
        printPrompt("Enter Student ID: ");
        cin >> id;
        printPrompt("Enter Password: ");
        cin >> password;

        auto it = find_if(students.begin(), students.end(),
                         [id, password](const Student& s) {
                             return s.studentId == id && s.password == password;
                         });

        if (it != students.end()) {
            printSuccess("Login successful!");
            currentStudentId = id;
            waitForKey();
            studentPanel();
        } else {
            printError("Invalid credentials!");
            printContinue();
            waitForKey();
        }
    }

    void registerStudent() {
        clearScreen();
        string id, password;
        printHeader("STUDENT REGISTRATION");
        printPrompt("Enter Student ID: ");
        cin >> id;
        printPrompt("Enter Password: ");
        cin >> password;

        auto it = find_if(students.begin(), students.end(),
                         [id](const Student& s) {
                             return s.studentId == id;
                         });

        if (it != students.end()) {
            printError("Student ID already exists!");
        } else {
            students.push_back(Student(id, password));
            saveData();
            printSuccess("Registration successful!");
        }
        printContinue();
        waitForKey();
    }

    void adminPanel() {
        int choice;
        do {
            clearScreen();
            printHeader("ADMIN PANEL");
            printMenuOption(1, "Add Course");
            printMenuOption(2, "Delete Course");
            printMenuOption(3, "View All Courses");
            printMenuOption(4, "Search Course");
            printMenuOption(5, "View All Students");
            printMenuOption(6, "Delete Student");
            printMenuOption(7, "Logout");
            printFooter();
            printPrompt("Enter your choice: ");
            cin >> choice;

            switch (choice) {
                case 1:
                    addCourseDataStructureChoice();
                    break;
                case 2:
                    deleteCourseDataStructureChoice();
                    break;
                case 3:
                    viewAllCoursesDataStructureChoice();
                    break;
                case 4:
                    searchCourseDataStructureChoice();
                    break;
                case 5:
                    viewAllStudents();
                    break;
                case 6:
                    deleteStudent();
                    break;
                case 7:
                    printSuccess("Logging out...");
                    break;
                default:
                    printError("Invalid choice!");
                    printContinue();
                    waitForKey();
            }
        } while (choice != 7);
    }

    void studentPanel() {
        int choice;
        do {
            clearScreen();
            printHeader("STUDENT PANEL");
            printMenuOption(1, "View All Courses");
            printMenuOption(2, "Search Course");
            printMenuOption(3, "Enroll Course");
            printMenuOption(4, "Drop Course");
            printMenuOption(5, "View My Courses");
            printMenuOption(6, "Logout");
            printFooter();
            printPrompt("Enter your choice: ");
            cin >> choice;

            switch (choice) {
                case 1:
                    viewAllCoursesDataStructureChoice();
                    break;
                case 2:
                    searchCourseDataStructureChoice();
                    break;
                case 3:
                    enrollCourseDataStructureChoice();
                    break;
                case 4:
                    dropCourse();
                    break;
                case 5:
                    viewMyCoursesDataStructureChoice();
                    break;
                case 6:
                    printSuccess("Logging out...");
                    break;
                default:
                    printError("Invalid choice!");
                    printContinue();
                    waitForKey();
            }
        } while (choice != 6);
    }

    void addCourseDataStructureChoice() {
        clearScreen();
        int choice;
        printHeader("ADD COURSE");
        printMenuOption(1, "Sorted Array");
        printMenuOption(2, "Linked List");
        printMenuOption(3, "Stack");
         printMenuOption(4, "Queue");
        printFooter();
        printPrompt("Enter choice: ");
        cin >> choice;

        switch (choice) {
            case 1:
                addCourseSortedArray();
                break;
            case 2:
                addCourseLinkedList();
                break;
            case 3:
                addCourseStack();
                break;
                case 4:
            addCourseQueue();
            break;
            default:
                printError("Invalid choice!");
                printContinue();
                waitForKey();
        }
    }

    void addCourseSortedArray() {
        clearScreen();
        string id, name;
        double credits;

        printHeader("ADD COURSE (SORTED ARRAY)");
        printPrompt("Enter Course ID: ");
        cin >> id;
        cin.ignore();
        printPrompt("Enter Course Name: ");
        getline(cin, name);
        printPrompt("Enter Credits : ");
        while (!(cin >> credits)) {
            printError("Invalid input! Please enter a number: ");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Course course(id, name, credits);
        sortedArray.insertCourse(course);
        saveData();
        printSuccess("Course added successfully to sorted array!");
        printContinue();
        waitForKey();
    }

    void addCourseLinkedList() {
        clearScreen();
        string id, name;
        double credits;

        printHeader("ADD COURSE (LINKED LIST)");
        printPrompt("Enter Course ID: ");
        cin >> id;
        cin.ignore();
        printPrompt("Enter Course Name: ");
        getline(cin, name);
        printPrompt("Enter Credits : ");
        while (!(cin >> credits)) {
            printError("Invalid input! Please enter a number: ");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Course course(id, name, credits);
        linkedList.insertCourse(course);
        saveData();
        printSuccess("Course added successfully to linked list!");
        printContinue();
        waitForKey();
    }

    void addCourseStack() {
        clearScreen();
        string id, name;
        double credits;

        printHeader("ADD COURSE (STACK)");
        printPrompt("Enter Course ID: ");
        cin >> id;
        cin.ignore();
        printPrompt("Enter Course Name: ");
        getline(cin, name);
        printPrompt("Enter Credits: ");
        while (!(cin >> credits)) {
            printError("Invalid input! Please enter a number: ");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Course course(id, name, credits);
        stack.pushCourse(course);
        saveData();
        printSuccess("Course pushed successfully to stack!");
        printContinue();
        waitForKey();
    }

    void addCourseQueue() {
    clearScreen();
    string id, name;
    double credits;

    printHeader("ADD COURSE (QUEUE)");
    printPrompt("Enter Course ID: ");
    cin >> id;
    cin.ignore();
    printPrompt("Enter Course Name: ");
    getline(cin, name);
    printPrompt("Enter Credits : ");
    while (!(cin >> credits)) {
        printError("Invalid input! Please enter a number: ");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Course course(id, name, credits);
    queue.enqueueCourse(course);
    saveData();
    printSuccess("Course enqueued successfully to queue!");
    printContinue();
    waitForKey();
}





    void deleteCourseDataStructureChoice() {
        clearScreen();
        int choice;
        printHeader("DELETE COURSE");
        printMenuOption(1, "Sorted Array");
        printMenuOption(2, "Linked List");
        printMenuOption(3, "Stack");
         printMenuOption(4, "Queue");
        printFooter();
        printPrompt("Enter choice: ");
        cin >> choice;

        switch (choice) {
            case 1:
                deleteCourseSortedArray();
                break;
            case 2:
                deleteCourseLinkedList();
                break;
            case 3:
                deleteCourseStack();
                break;
                case 4:
            deleteCourseQueue();
            break;
            default:
                printError("Invalid choice!");
                printContinue();
                waitForKey();
        }
    }

    void deleteCourseSortedArray() {
        clearScreen();
        string id;
        printHeader("DELETE COURSE (SORTED ARRAY)");
        printPrompt("Enter Course ID to delete: ");
        cin >> id;

        sortedArray.deleteCourse(id);
        saveData();
        printContinue();
        waitForKey();
    }

    void deleteCourseLinkedList() {
        clearScreen();
        string id;
        printHeader("DELETE COURSE (LINKED LIST)");
        printPrompt("Enter Course ID to delete: ");
        cin >> id;

        linkedList.deleteCourse(id);
        saveData();
        printContinue();
        waitForKey();
    }

    void deleteCourseStack() {
    clearScreen();
    printHeader("DELETE COURSE (STACK)");

    if (stack.isEmpty()) {
        printError("Stack is empty! No courses to delete.");
    } else {
        Course* course = stack.peekCourse();
        if (course) {
            printSuccess("Deleting last added course: " + course->courseName);
            stack.popCourse();
            saveData();
        }
    }

    printContinue();
    waitForKey();
}


void deleteCourseQueue() {
    clearScreen();
    printHeader("DELETE COURSE (QUEUE)");

    if (queue.isEmpty()) {
        printError("Queue is empty! No courses to delete.");
    } else {
        Course* course = queue.frontCourse();
        if (course) {
            printSuccess("Deleting first course in queue: " + course->courseName);
            queue.dequeueCourse();
            saveData();
        }
    }

    printContinue();
    waitForKey();
}





    void viewAllCoursesDataStructureChoice() {
        clearScreen();
        int choice;
        printHeader("VIEW ALL COURSES");
        printMenuOption(1, "Sorted Array");
        printMenuOption(2, "Linked List");
        printMenuOption(3, "Stack");
        printMenuOption(4, "Queue");
        printFooter();
        printPrompt("Enter choice: ");
        cin >> choice;

        switch (choice) {
            case 1:
                viewAllCoursesSortedArray();
                break;
            case 2:
                viewAllCoursesLinkedList();
                break;
            case 3:
                viewAllCoursesStack();
                break;
                case 4:
            viewAllCoursesQueue();
            break;
            default:
                printError("Invalid choice!");
                printContinue();
                waitForKey();
        }
    }

    void viewAllCoursesSortedArray() {
        clearScreen();
        sortedArray.displayCourses();
        printContinue();
        waitForKey();
    }

    void viewAllCoursesLinkedList() {
        clearScreen();
        linkedList.displayCourses();
        printContinue();
        waitForKey();
    }

    void viewAllCoursesStack() {
        clearScreen();
        stack.displayCourses();
        printContinue();
        waitForKey();
    }

    void viewAllCoursesQueue() {
    clearScreen();
    queue.displayCourses();
    printContinue();
    waitForKey();
}


    void viewAllStudents() {
    clearScreen();
    printHeader("ALL STUDENTS (SORTED BY ID)");

    if (students.empty()) {
        printError("No students registered!");
    } else {
        // Create a sorted array of student pointers
        vector<const Student*> sortedStudents;
        for (const auto& student : students) {
            sortedStudents.push_back(&student);
        }

        // Sort using insertion sort (to demonstrate sorted array approach)
        for (size_t i = 1; i < sortedStudents.size(); i++) {
            const Student* key = sortedStudents[i];
            int j = i - 1;

            // Move elements that are greater than key to one position ahead
            while (j >= 0 && sortedStudents[j]->studentId > key->studentId) {
                sortedStudents[j + 1] = sortedStudents[j];
                j = j - 1;
            }
            sortedStudents[j + 1] = key;
        }

        // Display the sorted students
        for (size_t i = 0; i < sortedStudents.size(); i++) {
            cout << CYAN << "| " << WHEAT << (i + 1) << ". ID: " << sortedStudents[i]->studentId
                 << ", Enrolled Courses: " << sortedStudents[i]->enrolledCourses.size();
            int spaces = 46 - to_string(i + 1).length() - 6 - sortedStudents[i]->studentId.length()
                       - to_string(sortedStudents[i]->enrolledCourses.size()).length() - 21;
            for(int j = 0; j < spaces; j++) cout << " ";
            cout << CYAN << " |" << RESET << "\n";
        }
        printFooter();
    }
    printContinue();
    waitForKey();
}

    void searchCourseDataStructureChoice() {
        clearScreen();
        int choice;
        printHeader("SEARCH COURSE");
        printMenuOption(1, "Sorted Array");
        printMenuOption(2, "Linked List");
        printMenuOption(3, "Stack");
        printMenuOption(4, "Queue");
        printFooter();
        printPrompt("Enter choice: ");
        cin >> choice;

        switch (choice) {
            case 1:
                searchCourseSortedArray();
                break;
            case 2:
                searchCourseLinkedList();
                break;
            case 3:
                searchCourseStack();
                break;
                case 4:
            searchCourseQueue();
            break;
            default:
                printError("Invalid choice!");
                printContinue();
                waitForKey();
        }
    }

    void searchCourseSortedArray() {
        clearScreen();
        string id;
        printHeader("SEARCH COURSE (SORTED ARRAY)");
        printPrompt("Enter Course ID to search: ");
        cin >> id;

        sortedArray.searchCourse(id);
        printContinue();
        waitForKey();
    }

    void searchCourseLinkedList() {
        clearScreen();
        string id;
        printHeader("SEARCH COURSE (LINKED LIST)");
        printPrompt("Enter Course ID to search: ");
        cin >> id;

        linkedList.searchCourse(id);
        printContinue();
        waitForKey();
    }

    void searchCourseStack() {
        clearScreen();
        string id;
        printHeader("SEARCH COURSE (STACK)");
        printPrompt("Enter Course ID to search: ");
        cin >> id;

        Course* course = stack.findCourse(id);
        if (course) {
            printSuccess("Course found in stack!");
            cout << WHEAT << "Course ID: " << RESET << course->courseId << endl;
            cout << WHEAT << "Course Name: " << RESET << course->courseName << endl;
            cout << WHEAT << "Credits: " << RESET << fixed << setprecision(2) << course->credits << endl;
        } else {
            printError("Course not found in stack!");
        }
        printContinue();
        waitForKey();
    }

    void searchCourseQueue() {
    clearScreen();
    string id;
    printHeader("SEARCH COURSE (QUEUE)");
    printPrompt("Enter Course ID to search: ");
    cin >> id;

    Course* course = queue.findCourse(id);
    if (course) {
        printSuccess("Course found in queue!");
        cout << WHEAT << "Course ID: " << RESET << course->courseId << endl;
        cout << WHEAT << "Course Name: " << RESET << course->courseName << endl;
        cout << WHEAT << "Credits: " << RESET << fixed << setprecision(2) << course->credits << endl;
    } else {
        printError("Course not found in queue!");
    }
    printContinue();
    waitForKey();
}


    void enrollCourseDataStructureChoice() {
        clearScreen();
        int choice;
        printHeader("ENROLL COURSE");
        printMenuOption(1, "Sorted Array");
        printMenuOption(2, "Linked List");
        printMenuOption(3, "Stack");
        printMenuOption(4, "Queue");
        printFooter();
        printPrompt("Enter choice: ");
        cin >> choice;

        switch (choice) {
            case 1:
                enrollCourseSortedArray();
                break;
            case 2:
                enrollCourseLinkedList();
                break;
            case 3:
                enrollCourseStack();
                break;
                case 4:
            enrollCourseQueue();
            break;
            default:
                printError("Invalid choice!");
                printContinue();
                waitForKey();
        }
    }

    void enrollCourseSortedArray() {
        clearScreen();
        string courseId;
        printHeader("ENROLL COURSE (SORTED ARRAY)");
        printPrompt("Enter Course ID to enroll: ");
        cin >> courseId;

        Course* course = sortedArray.findCourse(courseId);

        if (course) {
            auto it = find_if(students.begin(), students.end(),
                             [this](const Student& s) {
                                 return s.studentId == currentStudentId;
                             });

            if (it != students.end()) {
                auto enrolledIt = find_if(it->enrolledCourses.begin(), it->enrolledCourses.end(),
                                         [courseId](const EnrolledCourse& ec) {
                                             return ec.courseId == courseId;
                                         });
                if (enrolledIt != it->enrolledCourses.end()) {
                    printError("Already enrolled in this course!");
                } else {
                    it->enrolledCourses.push_back({courseId, true});
                    saveData();
                    printSuccess("Successfully enrolled in course: " + course->courseName);
                }
            }
        } else {
            printError("Course not found in Sorted Array!");
        }

        printContinue();
        waitForKey();
    }

    void enrollCourseLinkedList() {
        clearScreen();
        string courseId;
        printHeader("ENROLL COURSE (LINKED LIST)");
        printPrompt("Enter Course ID to enroll: ");
        cin >> courseId;

        Course* course = linkedList.findCourse(courseId);

        if (course) {
            auto it = find_if(students.begin(), students.end(),
                             [this](const Student& s) {
                                 return s.studentId == currentStudentId;
                             });

            if (it != students.end()) {
                auto enrolledIt = find_if(it->enrolledCourses.begin(), it->enrolledCourses.end(),
                                         [courseId](const EnrolledCourse& ec) {
                                             return ec.courseId == courseId;
                                         });
                if (enrolledIt != it->enrolledCourses.end()) {
                    printError("Already enrolled in this course!");
                } else {
                    it->enrolledCourses.push_back({courseId, false});
                    saveData();
                    printSuccess("Successfully enrolled in course: " + course->courseName);
                }
            }
        } else {
            printError("Course not found in Linked List!");
        }

        printContinue();
        waitForKey();
    }

    void enrollCourseStack() {
        clearScreen();
        string courseId;
        printHeader("ENROLL COURSE (STACK)");
        printPrompt("Enter Course ID to enroll: ");
        cin >> courseId;

        Course* course = stack.findCourse(courseId);

        if (course) {
            auto it = find_if(students.begin(), students.end(),
                             [this](const Student& s) {
                                 return s.studentId == currentStudentId;
                             });

            if (it != students.end()) {
                auto enrolledIt = find_if(it->enrolledCourses.begin(), it->enrolledCourses.end(),
                                         [courseId](const EnrolledCourse& ec) {
                                             return ec.courseId == courseId;
                                         });
                if (enrolledIt != it->enrolledCourses.end()) {
                    printError("Already enrolled in this course!");
                } else {
                    it->enrolledCourses.push_back({courseId, false});
                    saveData();
                    printSuccess("Successfully enrolled in course: " + course->courseName);
                }
            }
        } else {
            printError("Course not found in Stack!");
        }

        printContinue();
        waitForKey();
    }


    void enrollCourseQueue() {
    clearScreen();
    string courseId;
    printHeader("ENROLL COURSE (QUEUE)");
    printPrompt("Enter Course ID to enroll: ");
    cin >> courseId;

    Course* course = queue.findCourse(courseId);

    if (course) {
        auto it = find_if(students.begin(), students.end(),
                         [this](const Student& s) {
                             return s.studentId == currentStudentId;
                         });

        if (it != students.end()) {
            auto enrolledIt = find_if(it->enrolledCourses.begin(), it->enrolledCourses.end(),
                                     [courseId](const EnrolledCourse& ec) {
                                         return ec.courseId == courseId;
                                     });
            if (enrolledIt != it->enrolledCourses.end()) {
                printError("Already enrolled in this course!");
            } else {
                it->enrolledCourses.push_back({courseId, false});
                saveData();
                printSuccess("Successfully enrolled in course: " + course->courseName);
            }
        }
    } else {
        printError("Course not found in Queue!");
    }

    printContinue();
    waitForKey();
}








    void deleteStudent() {
        clearScreen();
        string id;
        printHeader("DELETE STUDENT");
        printPrompt("Enter Student ID to delete: ");
        cin >> id;

        auto it = find_if(students.begin(), students.end(),
                         [id](const Student& s) {
                             return s.studentId == id;
                         });

        if (it != students.end()) {
            students.erase(it);
            saveData();
            printSuccess("Student deleted successfully!");
        } else {
            printError("Student not found!");
        }

        printContinue();
        waitForKey();
    }

    void dropCourse() {
        clearScreen();
        string courseId;
        printHeader("DROP COURSE");
        printPrompt("Enter Course ID to drop: ");
        cin >> courseId;

        auto it = find_if(students.begin(), students.end(),
                         [this](const Student& s) {
                             return s.studentId == currentStudentId;
                         });

        if (it != students.end()) {
            auto enrolledIt = find_if(it->enrolledCourses.begin(), it->enrolledCourses.end(),
                                     [courseId](const EnrolledCourse& ec) {
                                         return ec.courseId == courseId;
                                     });
            if (enrolledIt != it->enrolledCourses.end()) {
                it->enrolledCourses.erase(enrolledIt);
                saveData();
                printSuccess("Successfully dropped course!");
            } else {
                printError("You are not enrolled in this course!");
            }
        }

        printContinue();
        waitForKey();
    }

    void viewMyCoursesDataStructureChoice() {
        clearScreen();
        int choice;
        printHeader("VIEW MY COURSES");
        printMenuOption(1, "Sorted Array");
        printMenuOption(2, "Linked List");
        printMenuOption(3, "Stack");
        printMenuOption(4, "Queue");
        printFooter();
        printPrompt("Enter choice: ");
        cin >> choice;

        switch (choice) {
            case 1:
                viewMyCoursesSortedArray();
                break;
            case 2:
                viewMyCoursesLinkedList();
                break;
            case 3:
                viewMyCoursesStack();
                break;
                 case 4:
            viewMyCoursesQueue();
            break;
            default:
                printError("Invalid choice!");
                printContinue();
                waitForKey();
        }
    }

    void viewMyCoursesSortedArray() {
        clearScreen();
        printHeader("MY COURSES (SORTED ARRAY)");

        auto it = find_if(students.begin(), students.end(),
                         [this](const Student& s) {
                             return s.studentId == currentStudentId;
                         });

        if (it != students.end()) {
            if (it->enrolledCourses.empty()) {
                printError("No courses enrolled!");
            } else {
                int count = 1;
                for (const auto& enrolledCourse : it->enrolledCourses) {
                    if (enrolledCourse.isFromSortedArray) {
                        Course* course = sortedArray.findCourse(enrolledCourse.courseId);
                        if (course) {
                            cout << CYAN << "| " << WHEAT << count << ". " << course->courseId
                                 << " - " << course->courseName;
                            int spaces = 46 - to_string(count).length() - 4 - course->courseId.length()
                                       - course->courseName.length();
                            for(int j = 0; j < spaces; j++) cout << " ";
                            cout << CYAN << " |" << RESET << "\n";
                            count++;
                        }
                    }
                }
                if (count == 1) {
                    printError("No courses enrolled via Sorted Array!");
                } else {
                    printFooter();
                }
            }
        }
        printContinue();
        waitForKey();
    }

    void viewMyCoursesLinkedList() {
        clearScreen();
        printHeader("MY COURSES (LINKED LIST)");

        auto it = find_if(students.begin(), students.end(),
                         [this](const Student& s) {
                             return s.studentId == currentStudentId;
                         });

        if (it != students.end()) {
            if (it->enrolledCourses.empty()) {
                printError("No courses enrolled!");
            } else {
                int count = 1;
                for (const auto& enrolledCourse : it->enrolledCourses) {
                    if (!enrolledCourse.isFromSortedArray) {
                        Course* course = linkedList.findCourse(enrolledCourse.courseId);
                        if (course) {
                            cout << CYAN << "| " << WHEAT << count << ". " << course->courseId
                                 << " - " << course->courseName;
                            int spaces = 46 - to_string(count).length() - 4 - course->courseId.length()
                                       - course->courseName.length();
                            for(int j = 0; j < spaces; j++) cout << " ";
                            cout << CYAN << " |" << RESET << "\n";
                            count++;
                        }
                    }
                }
                if (count == 1) {
                    printError("No courses enrolled via Linked List!");
                } else {
                    printFooter();
                }
            }
        }
        printContinue();
        waitForKey();
    }

    void viewMyCoursesStack() {
        clearScreen();
        printHeader("MY COURSES (STACK)");

        auto it = find_if(students.begin(), students.end(),
                         [this](const Student& s) {
                             return s.studentId == currentStudentId;
                         });

        if (it != students.end()) {
            if (it->enrolledCourses.empty()) {
                printError("No courses enrolled!");
            } else {
                int count = 1;
                for (const auto& enrolledCourse : it->enrolledCourses) {
                    Course* course = stack.findCourse(enrolledCourse.courseId);
                    if (course && !enrolledCourse.isFromSortedArray) {
                        cout << CYAN << "| " << WHEAT << count << ". " << course->courseId
                             << " - " << course->courseName;
                        int spaces = 46 - to_string(count).length() - 4 - course->courseId.length()
                                   - course->courseName.length();
                        for(int j = 0; j < spaces; j++) cout << " ";
                        cout << CYAN << " |" << RESET << "\n";
                        count++;
                    }
                }
                if (count == 1) {
                    printError("No courses enrolled via Stack!");
                } else {
                    printFooter();
                }
            }
        }
        printContinue();
        waitForKey();
    }



    void viewMyCoursesQueue() {
    clearScreen();
    printHeader("MY COURSES (QUEUE)");

    auto it = find_if(students.begin(), students.end(),
                     [this](const Student& s) {
                         return s.studentId == currentStudentId;
                     });

    if (it != students.end()) {
        if (it->enrolledCourses.empty()) {
            printError("No courses enrolled!");
        } else {
            int count = 1;
            for (const auto& enrolledCourse : it->enrolledCourses) {
                Course* course = queue.findCourse(enrolledCourse.courseId);
                if (course && !enrolledCourse.isFromSortedArray) {
                    cout << CYAN << "| " << WHEAT << count << ". " << course->courseId
                         << " - " << course->courseName;
                    int spaces = 46 - to_string(count).length() - 4 - course->courseId.length()
                               - course->courseName.length();
                    for(int j = 0; j < spaces; j++) cout << " ";
                    cout << CYAN << " |" << RESET << "\n";
                    count++;
                }
            }
            if (count == 1) {
                printError("No courses enrolled via Queue!");
            } else {
                printFooter();
            }
        }
    }
    printContinue();
    waitForKey();
}

};
// ==================== MAIN FUNCTION ====================
int main() {
    CourseManagementSystem system;
    system.mainMenu();
    return 0;
}
