#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "common.hpp"
#include "student.hpp"

using namespace std;

//file names 
string COMMANDS_FILE = "commands.txt";

int main()
{
    string command = "";
    vector<string> comVector;
    string value = "";

    Student st;

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
        }
        else if (comVector[0] == "show") {
            if (comVector[1] == "student") {
                st.showStudent();
            }
        }
        else if (comVector[0] == "edit") {
            if (comVector[1] == "student") {
                cout << "Enter student's ID: ";
                getline(cin, value);

                if (st.editStudent(0, value))
                    cout << "Student " << value << " successful." << endl;
                else
                    cout << "Student " << value << " unsuccessful." << endl;
            }
        }
        else if (comVector[0] == "exit") {
            cout << "Good bye." << endl;
            break;
        }
    }
}
