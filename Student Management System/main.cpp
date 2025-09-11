#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>
#include <unordered_map>
#include <functional>

#include "common.hpp"
#include "filemodification.hpp"
#include "student.hpp"
#include "course.hpp"
#include "teacher.hpp"
#include "admin.hpp"

using namespace std;

// User types
enum class UserType { None = 0, Admin = 1, Teacher = 2, Student = 3 };

// User info
struct User {
    UserType type = UserType::None;
    string id = "";
    bool isLoggedIn() const { return !id.empty(); } // check login status
};

// File names
const string COURSE_FILE = "COURSEDETAILS.txt";
const string STUDENT_FILE = "STUDENTDETAILS.txt";
const string TEACHER_FILE = "TEACHERDETAILS.txt";
const string ADMIN_FILE = "ADMINDETAILS.txt";
const string GRADE_FILE = "GRADE.txt";
const string COMMAND_FILE = "COMMANDS.txt";

// Handles login
class UserManager {
public:
    static User login() {
        User u;
        int type;
        string id, password;
        vector<vector<string>> userDetails;

        // Lambda to get input
        auto getInput = [](const string& prompt, string& value) -> bool {
            cout << prompt;
            getline(cin, value);
            value = trim(value);
            return !value.empty();
            };

        // Lambda to ask retry
        auto askRetry = []() -> bool {
            char res;
            cout << "Do you want to try again? (y/n): ";
            cin >> res;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return tolower(res) == 'y';
            };

        // Select user type
        while (true) {
            cout << "Login as (1=Admin, 2=Teacher, 3=Student): ";
            if (!(cin >> type)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                if (!askRetry()) return u;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (type >= 1 && type <= 3) break;
            cout << "Invalid choice. Enter 1, 2, or 3.\n";
            if (!askRetry()) return u;
        }

        // Enter ID
        while (true) {
            if (!getInput("Enter Id: ", id)) {
                if (!askRetry()) return u;
                continue;
            }
            string FILE = (type == 1) ? ADMIN_FILE : (type == 2) ? TEACHER_FILE : STUDENT_FILE;
            userDetails = readTxtFile(FILE, 0, id); // read user from file
            if (!userDetails.empty()) break;
            cout << "Id is not registered.\n";
            if (!askRetry()) return u;
        }

        // Enter password
        while (true) {
            if (!getInput("Enter Password: ", password)) {
                if (!askRetry()) return u;
                continue;
            }
            if (password == userDetails[0][3]) break;
            cout << "Password doesn't match.\n";
            if (!askRetry()) return u;
        }

        u.type = static_cast<UserType>(type);
        u.id = id;
        return u;
    }
};

// Handles commands based on user type
class CommandHandler {
    User& user;
    Student s;
    Course c;
    Teacher t;
    Admin a;
    vector<vector<string>> defCommands;

public:
    CommandHandler(User& u, const vector<vector<string>>& cmds) : user(u), defCommands(cmds) {}

    // Execute command
    void execute(const vector<string>& comVector, const string& rawCommand) {
        if (!checkCommandValidity(rawCommand, defCommands, static_cast<int>(user.type))) return;

        string cmd = comVector[0];

        if (cmd == "help") printTable(defCommands); // show commands
        else if (cmd == "exit") { cout << "Good day." << endl; exit(0); }
        else if (cmd == "login") handleLogin();
        else if (cmd == "logout") handleLogout();
        else if (cmd == "add") handleAdd(comVector);
        else if (cmd == "delete") handleDelete(comVector);
        else if (cmd == "show") handleShow(comVector);
        else if (cmd == "edit") handleEdit(comVector);
        else if (cmd == "enroll") handleEnroll(comVector);
        else if (cmd == "disenroll") handleDisenroll(comVector);
        else if (cmd == "assign") handleAssign(comVector);
        else if (cmd == "disassign") handleDisassign(comVector);
        else cout << "Unknown command.\n";
    }

private:
    void handleLogin() { if (!user.isLoggedIn()) user = UserManager::login(); else cout << "Already logged in.\n"; }
    void handleLogout() { user.id = ""; user.type = UserType::None; cout << "Logout successful.\n"; }

    // Add entities
    void handleAdd(const vector<string>& comVector) {
        if (comVector.size() < 2) return;
        string target = comVector[1];
        if (target == "student") cout << (s.add() ? "New student registered." : "Student register failed.") << endl;
        else if (target == "teacher") cout << (t.add() ? "New teacher registered." : "Teacher register failed.") << endl;
        else if (target == "course") cout << (c.add() ? "New course added." : "Course add failed.") << endl;
        else if (target == "admin" && user.type == UserType::Admin) cout << (a.add() ? "New admin registered." : "Admin register failed.") << endl;
    }

    // Delete entities
    void handleDelete(const vector<string>& comVector) {
        if (comVector.size() < 2) return;
        string target = comVector[1];
        if (target == "student") cout << (s.del() ? "Student deleted." : "Delete failed.") << endl;
        else if (target == "teacher") cout << (t.del() ? "Teacher deleted." : "Delete failed.") << endl;
        else if (target == "course") cout << (c.del() ? "Course deleted." : "Delete failed.") << endl;
    }

    // Show entities or reports
    void handleShow(const vector<string>& comVector) {
        if (comVector.size() < 2) return;
        string target = comVector[1];
        if (target == "enroll-course") {
            vector<string> enrolledCourse;
            if (user.type == UserType::Admin || user.type == UserType::Teacher) enrolledCourse = c.getEnrolledCourses();
            else if (user.type == UserType::Student) enrolledCourse = c.getEnrolledCourses(user.id);
            if (!enrolledCourse.empty()) {
                cout << "Enrolled courses:\n";
                for (auto& c : enrolledCourse) cout << c << endl;
            }
        }
        else if (target == "report") {
            if (user.type == UserType::Admin || user.type == UserType::Teacher) c.generateReport();
            else if (user.type == UserType::Student) c.generateReport(user.id);
        }
        else if (target == "teacher") t.show();
        else if (target == "course") c.show();
        else if (target == "student") s.show();
    }

    // Edit entities or marks
    void handleEdit(const vector<string>& comVector) {
        if (comVector.size() < 2) return;
        string target = comVector[1];
        if (target == "student") cout << (s.edit() ? "Student edited." : "Edit failed.") << endl;
        else if (target == "teacher") cout << (t.edit() ? "Teacher edited." : "Edit failed.") << endl;
        else if (target == "course") cout << (c.edit() ? "Course edited." : "Edit failed.") << endl;
        else if (target == "mark") { // edit mark
            string id, code; int markType, mark;
            cout << "Enter student id: "; getline(cin, id); id = trim(id);
            cout << "Enter course code: "; getline(cin, code); code = trim(code);
            cout << "Enter mark type (1 - internal, 2 - final): "; cin >> markType;
            cout << "Enter mark: "; cin >> mark; cin.ignore();
            if (c.editMark(id, code, markType, to_string(mark))) cout << "Mark edited.\n";
            else cout << "Mark edit failed.\n";
        }
    }

    void handleEnroll(const vector<string>& comVector) {
        if (comVector.size() < 2 || comVector[1] != "course") return;
        bool success = (user.type == UserType::Student) ? c.enroll(user.id) : c.enroll();
        cout << (success ? "Enroll Successful." : "Enroll Failed.") << endl;
    }

    void handleDisenroll(const vector<string>& comVector) {
        if (comVector.size() < 2 || comVector[1] != "course") return;
        bool success = (user.type == UserType::Student) ? c.disenroll(user.id) : c.disenroll();
        cout << (success ? "Disenroll Successful." : "Disenroll Failed.") << endl;
    }

    void handleAssign(const vector<string>& comVector) {
        if (comVector.size() < 2) return;
        if (comVector[1] == "teacher") cout << (c.assignTeacher() ? "Teacher assigned." : "Assign failed.") << endl;
    }

    void handleDisassign(const vector<string>& comVector) {
        if (comVector.size() < 2) return;
        if (comVector[1] == "teacher") cout << (c.disassignTeacher() ? "Teacher disassigned." : "Disassign failed.") << endl;
    }
};

// Main program loop
int main() {
    User u;
    vector<vector<string>> defCommands = readTxtFile(COMMAND_FILE);

    cout << "***** POKENO SOUTH PRIMARY SCHOOL *****" << endl;
    cout << "----- Student Management System -----\n" << endl;
    cout << "Type 'help' to see all the commands." << endl;

    CommandHandler handler(u, defCommands);
    string command;
    vector<string> comVector;

    // Command loop
    while (true) {
        cout << ">> ";
        getline(cin, command);
        command = trim(command);
        comVector = split(command); // split command into words
        if (!comVector.empty()) handler.execute(comVector, command);
    }
}
