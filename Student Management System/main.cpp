#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "common.hpp"
#include "student.hpp"
#include "course.hpp"

using namespace std;

//file names 
string COMMANDS_FILE = "commands.txt";

int main()
{
    string command = "";
    vector<string> comVector;
    string value = "";

    Student st;
    Course c;

    vector<vector<string>> defCommands = readTxtFile(COMMANDS_FILE);

    cout << "== POKENO PRIMARY SOUTH SCHOOL ==" << endl;
    cout << "-- Student Management System --\n" << endl;
   
    cout << "Type 'help' to see valid commands." << endl;

    while (true) {
        cout << ">> ";
        getline(cin, command);
        command = trim(command);
        comVector = splitBySpace(command);

        if (!checkCommandValidity(command, defCommands)) {
            cout << "Command invalid. Type 'help' to see valid commands." << endl;
            continue;
        }

        if (comVector[0] == "help") {
            printTable(defCommands);
        }
        else if (comVector[0] == "add") {
            if (comVector[1] == "student") {
                if (st.addStudent())
                    cout << "New student register successful." << endl;
                else
                    cout << "Register unsuccessful." << endl;
            }
            else if (comVector[1] == "course") {
                if (c.addCourse())
                    cout << "New course add successful." << endl;
                else
                    cout << "New course add unsuccessful." << endl;
            }
        }
        else if (comVector[0] == "show") {
            if (comVector[1] == "student")
                st.showStudent();
            else if (comVector[1] == "course")
                c.showCourse();
        }
        else if (comVector[0] == "edit") {
            if (comVector[1] == "student") {
                cout << "Enter student's ID: ";
                getline(cin, value);
                value = trim(value);

                if (st.editStudent(0, value))
                    cout << "Student " << value << " edit successful." << endl;
                else
                    cout << "Student " << value << " edit unsuccessful." << endl;

                value.clear();
            }
            else if (comVector[1] == "course") {
                cout << "Enter course code: ";
                getline(cin, value);
                value = trim(value);

                if (c.editCourse(0, value))
                    cout << "Course " << value << " edit successful." << endl;
                else
                    cout << "Course " << value << " edit unsuccessful." << endl;

                value.clear();
            }
        }
        else if (comVector[0] == "delete") {
            if (comVector[1] == "student") {
                cout << "Enter student's ID: ";
                getline(cin, value);
                value = trim(value);

                if (st.deleteStudent(value))
                    cout << "Student " << value << " delete successful." << endl;
                else
                    cout << "Student " << value << " delete unsuccessful." << endl;

                value.clear();
            } else if (comVector[1] == "course") {
                cout << "Enter course code: ";
                getline(cin, value);
                value = trim(value);

                if (c.deleteCourse(value))
                    cout << "Course " << value << " delete successful." << endl;
                else
                    cout << "Course " << value << " delete unsuccessful." << endl;

                value.clear();
            }
        } 
        else if (comVector[0] == "enroll") {
            if (comVector[1] == "student") {
                cout << "Enter student's ID: ";
                getline(cin, value);
                value = trim(value);

                if (st.enrollStudent(value)) {
                    return 0;
                }
            }

        }
        else if (comVector[0] == "exit") {
            cout << "Good bye." << endl;
            break;
        }
    }
}

