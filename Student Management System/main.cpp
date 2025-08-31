#include <iostream>
#include <string>
#include <vector>

#include "common.hpp"

using namespace std;

//file names 
string COMMANDS_FILE = "commands.txt";

int main()
{
    string command = "";
    vector<string> comVector;

    vector<vector<string>> defCommands = readTxtFile(COMMANDS_FILE);

    cout << "== POKENO PRIMARY SOUTH SCHOOL ==" << endl;
    cout << "-- Student Management System --\n" << endl;
   
    cout << "Type 'help' to see valid commands." << endl;

    while (true) {
        cout << ">> ";
        getline(cin, command);
        command = trim(command);
        comVector = splitBySpace(command);

        checkCommandValidity(command, defCommands);

        if (comVector[0] == "help") {
            printTable(defCommands);
        }
        else if (comVector[0] == "exit") {
            cout << "Good bye." << endl;
            break;
        }
    }
}
