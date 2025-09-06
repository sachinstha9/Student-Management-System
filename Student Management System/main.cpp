#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits> 

#include "common.hpp"
#include "filemodification.hpp"
#include "student.hpp"
#include "course.hpp"
#include "teacher.hpp"
#include "admin.hpp"

using namespace std;

struct User {
    int type = 0;
    string id = "";

    User(int t = 4, const string& i = "") : type(t), id(i) {}
};

User setUserType(string STUDENT_FILE, string TEACHER_FILE, string ADMIN_FILE) {
    User u;
    int type;
    string id, password;
    vector<vector<string>> userDetails;

    auto getInput = [](const string& prompt, string& value) -> bool {
        cout << prompt;
        getline(cin, value);
        value = trim(value);
        return !value.empty();
        };

    auto askRetry = []() -> bool {
        char res;
        cout << "Do you want to try again? (y/n): ";
        cin >> res;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return tolower(res) == 'y';
        };

    // Login type
    while (true) {
        cout << "Login as (1=Admin, 2=Teacher, 3=Student): ";
        cin >> type;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (type >= 1 && type <= 3) break;
        if (!askRetry()) return u;
    }

    // ID
    while (true) {
        if (!getInput("Enter Id: ", id)) {
            if (!askRetry()) return u;
            continue;
        }
        string FILE = (type == 1) ? ADMIN_FILE : (type == 2) ? TEACHER_FILE : STUDENT_FILE;
        userDetails = readTxtFile(FILE, 0, id);
        if (!userDetails.empty()) break;
        cout << "Id is not registered.\n";
        if (!askRetry()) return u;
    }

    // Password
    while (true) {
        if (!getInput("Enter Password: ", password)) {
            if (!askRetry()) return u;
            continue;
        }
        if (password == userDetails[0][3]) break;
        cout << "Password doesn't match.\n";
        if (!askRetry()) return u;
    }

    u.type = type;
    u.id = id;
    return u;
}

int main()
{
    User u;
    Student s;
    Course c;
    Teacher t;
    Admin a;

    string COURSE_FILE = "COURSEDETAILS.txt",
        STUDENT_FILE = "STUDENTDETAILS.txt",
        TEACHER_FILE = "TEACHERDETAILS.txt",
        ADMIN_FILE = "ADMINDETAILS.txt",
        GRADE_FILE = "GRADE.txt",
        COMMAND_FILE = "COMMANDS.txt";
    string command;
    vector<string> comVector;
    vector<vector<string>> defCommands = readTxtFile(COMMAND_FILE);

    cout << "Student Management System" << endl;

    while (true) {
        if (u.id.empty()) {
            u = setUserType(STUDENT_FILE, TEACHER_FILE, ADMIN_FILE);
           continue;
        }

        cout << ">> ";
        getline(cin, command);
        command = trim(command);
        comVector = split(command);

        if (!checkCommandValidity(command, defCommands, u.type)) continue;

        if (comVector[0] == "help") {
            printTable(defCommands);
        }
        else if (comVector[0] == "exit") {
            cout << "Good day." << endl;
            break;
        }
        else if (comVector[0] == "logout") {
            if (!u.id.empty()) {
                cout << "Login first." << endl;
                continue;
            }

            u.id = "";
            u.type = 0;
            cout << "Logout successful." << endl;
        }
        else if (comVector[0] == "add") {
            if (comVector[1] == "student" && (u.type == 1 || u.type == 2) && comVector.size() == 2) {
                if (s.add())
                    cout << "New student registered successfully." << endl;
                else
                    cout << "New student register unsuccessful." << endl;
            }
            else if (comVector[1] == "teacher" && u.type == 1 && comVector.size() == 2) {
                if (t.add())
                    cout << "New teacher registered successfully." << endl;
                else
                    cout << "New teacher register unsuccessful." << endl;
            }
            else if (comVector[1] == "course" && (u.type == 1 || u.type == 2) && comVector.size() == 2) {
                if (c.add())
                    cout << "New course added successfully." << endl;
                else
                    cout << "New course add unsuccessful." << endl;
            }
            else if (comVector[1] == "admin" && u.type == 1) {
                if (a.add())
                    cout << "New admin registered succesfully." << endl;
                else
                    cout << "New admin register unsuccessful." << endl;
            }
        }
        else if (comVector[0] == "delete") {
            if (comVector[1] == "student" && (u.type == 1 || u.type == 2) && comVector.size() == 2) {
                if (s.del())
                    cout << "Student deleted successful." << endl;
                else
                    cout << "Student delete unsuccessful." << endl;
            }
            else if (comVector[1] == "teacher" && u.type == 1 && comVector.size() == 2) {
                if (t.del())
                    cout << "Teacher deleted successfully." << endl;
                else
                    cout << "Teacher delete unsuccessful." << endl;
            }
            else if (comVector[1] == "course" && (u.type == 1 || u.type == 2) && comVector.size() == 2) {
                if (c.del())
                    cout << "Course deleted successfully." << endl;
                else
                    cout << "Course delete unsuccessful." << endl;
            }
        }
        else if (comVector[0] == "show") {
            if (comVector[1] == "enroll-course") {
                vector<string> enrolledCourse;

                if (u.type == 1 || u.type == 2) enrolledCourse = c.getEnrolledCourses();
                else if (u.type == 3) enrolledCourse = c.getEnrolledCourses(u.id);

                if (!enrolledCourse.empty()) {
                    cout << "Courses you are enrolled in are: " << endl;
                    for (auto c : enrolledCourse) {
                        cout << c << endl;
                    }
                }
            }
            else if (comVector[1] == "report") {
                if (u.type == 1 || u.type == 2) c.generateReport();
                else if (u.type == 3) c.generateReport(u.id);
            }
            else if (comVector[1] == "teacher" && (u.type == 1 || u.type == 2 || u.type == 3)) {
                t.show();
            }
            else if (comVector[1] == "course" && (u.type == 1 || u.type == 2 || u.type == 3)) {
                c.show();
            }
            else if (comVector[1] == "student" && (u.type == 1 || u.type == 2 || u.type == 3)) {
                s.show();
            }
        }
        else if (comVector[0] == "edit") {
            if (comVector[1] == "student" && (u.type == 1 || u.type == 2 || u.type == 3) && comVector.size() == 2) {
                if (s.edit())
                    cout << "Student details edit successful." << endl;
                else
                    cout << "Student details edit unsuccessful." << endl;
            }
            else if (comVector[1] == "teacher" && (u.type == 1 || u.type == 2) && comVector.size() == 2) {
                if (t.edit())
                    cout << "Teacher details edit successful." << endl;
                else
                    cout << "Teacher details edit unsuccessful." << endl;
            }
            else if (comVector[1] == "course" && (u.type == 1 || u.type == 2) && comVector.size() == 2) {
                if (c.edit())
                    cout << "Course details edit successfully." << endl;
                else
                    cout << "Course details edit unsuccessful." << endl;
            }
            else if (comVector[1] == "mark" && (u.type == 1 || u.type == 2) && comVector.size() == 2) {
                string id, code;
                int markType, mark;
                vector<string> markVec;

                cout << "Enter student id: ";
                getline(cin, id);
                id = trim(id);

                cout << "Enter course code: ";
                getline(cin, code);
                code = trim(code);

                cout << "Enter mark type (1 - internal, 2 - final):";
                cin >> markType;

                if (cin.fail())
                    cout << "Field value is not valid." << endl;

                cout << "Enter mark: ";
                cin >> mark;

                if (cin.fail())
                    cout << "Field value is not valid." << endl;

                markVec = c.getMarks(id, code);

                if (markVec.size() == 2) {
                    if (markVec[0] == "-" && markVec[1] == "-") {
                        cout << "Student " << id << " not enrolled in course " << code << "." << endl;
                    }
                    else {
                        if (c.editMark(id, code, markType, to_string(mark)))
                            cout << "Mark successfully edited." << endl;
                        else
                            cout << "Mark edit unsuccessful." << endl;
                    }
                }
                else if (markVec.size() == 1) {
                    cout << markVec[0] << endl;
                }
            }
        }
        else if (comVector[0] == "enroll") {
            if (comVector[1] == "course") {
                if (u.type == 1 || u.type == 2)
                    if (c.enroll())
                        cout << "Enroll Successful." << endl;
                    else
                        cout << "Enroll Unsuccessful." << endl;
                else if (u.type == 3)
                    if (c.enroll(u.id))
                        cout << "Enroll Successful." << endl;
                    else
                        cout << "Enroll Unsuccessful." << endl;
            }
        }
        else if (comVector[0] == "disenroll") {
            if (comVector[1] == "course") {
                if (u.type == 1 || u.type == 2)
                    if (c.disenroll())
                        cout << "Disenroll Successful." << endl;
                    else
                        cout << "Disenroll Unsuccessful." << endl;
                else if (u.type == 3)
                    if (c.disenroll(u.id))
                        cout << "Disenroll Successful." << endl;
                    else
                        cout << "Disenroll Unsuccessful." << endl;
            }
        }
        else if (comVector[0] == "assign") {
            if (comVector[1] == "teacher" && u.type == 1) {
                if (c.assignTeacher())
                    cout << "Teacher assign successful." << endl;
                else
                    cout << "Teacher assign unsuccessful." << endl;
            }
        }
        else if (comVector[0] == "disassign") {
            if (comVector[1] == "teacher" && u.type == 1) {
                if (c.disassignTeacher())
                    cout << "Teacher disassign successful." << endl;
                else
                    cout << "Teacher disassign unsuccessful." << endl;
            }
        }
    }
}
