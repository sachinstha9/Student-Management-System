#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>   

#include "teacher.hpp"
#include "filemodification.hpp"
#include "common.hpp"

using namespace std;

static auto getInput = [](const string& prompt, string& value) -> bool {
    cout << prompt;
    getline(cin, value);
    value = trim(value);
    return !value.empty();
    };

static auto askRetry = []() -> bool {
    char res;
    cout << "Do you want to try again? (y/n): ";
    cin >> res;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return tolower(res) == 'y';
    };


bool Teacher::add() {
    while (true) {
        if (!getInput("Name: ", name)) {
            if (!askRetry()) return false;
            continue;
        }

        if (!name.empty()) break;
        cout << "Field cannot be empty.\n";
        if (!askRetry()) return false;

    }

    while (true) {
        if (!getInput("Email: ", email)) {
            if (!askRetry()) return false;
            continue;
        }

        if (isValidEmail(email)) break;
        cout << "Email not valid.\n";
        if (!askRetry()) return false;
    }

    while (true) {
        if (!getInput("Password (must be 6 characters long): ", password)) {
            if (!askRetry()) return false;
            continue;
        }

        if (password.size() >= 6) break;
        cout << "Password not valid.\n";
        if (!askRetry()) return false;
    }

    srand(static_cast<unsigned int>(time(0)));
    string todayDate = getTodayDate();;
    todayDate.erase(std::remove(todayDate.begin(), todayDate.end(), '-'), todayDate.end());
    size_t totalNumOfS = readTxtFile(TEACHER_FILE).size();

    id = "T" + to_string(rand() % 100 + 1) + to_string(totalNumOfS + 1) + todayDate;

    return writeTxtFile(TEACHER_FILE, { {id, name, email, password} });
}

bool Teacher::del() {
    string temp;

    cout << "Enter teacher id: ";
    getline(cin, temp);

    if (!temp.empty()) {
        if (readTxtFile(TEACHER_FILE, 0, temp).empty()) {
            cout << "Teacher " << temp << " doesn't exists." << endl;
            return false;
        }
        else {
            return deleteRowTxtFile(TEACHER_FILE, 0, temp);
        }
    }
    else {
        cout << "Field cann't be empty." << endl;
        return false;
    }
}

bool Teacher::edit() {
    string temp;

    cout << "Enter teacher id: ";
    getline(cin, temp);

    if (!temp.empty()) {
        if (readTxtFile(TEACHER_FILE, 0, temp).empty()) {
            cout << "Teacher " << temp << " doesn't exists." << endl;
            return false;
        }
        else {
            while (true) {
                getInput("Name: ", name);
                break;
            }

            while (true) {
                getInput("Email: ", email);

                if (isValidEmail(email)) break;
                cout << "Email not valid.\n";
                if (!askRetry()) return false;
            }

            while (true) {
                getInput("Password (must be 6 characters long): ", password);

                if (password.size() >= 6) break;
                cout << "Password not valid.\n";
                if (!askRetry()) return false;
            }

            return updateTxtFile(TEACHER_FILE, 0, temp, { temp, name, email, password });
        }
    }
    else {
        cout << "Field cann't be empty." << endl;
        return false;
    }
}

void Teacher::show() {
    vector<vector<string>> teacherDetails = readTxtFile(TEACHER_FILE);

    if(teacherDetails.size() <= 1) {
        cout << "No teacher to show." << endl;
        return;
    }

    for (int i = 0; i < teacherDetails.size(); i++) {
        teacherDetails[i] = { teacherDetails[i][0], teacherDetails[i][1], teacherDetails[i][2] };
    }

    printTable(teacherDetails);
}