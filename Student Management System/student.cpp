#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "student.hpp"
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

bool Student::add() {
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

    string todayDate = getTodayDate();;
    todayDate.erase(std::remove(todayDate.begin(), todayDate.end(), '-'), todayDate.end());
    size_t totalNumOfS = readTxtFile(STUDENT_FILE).size();

    id = todayDate + to_string(totalNumOfS + 1);

    vector<vector<string>> grade = readTxtFile(GRADE_FILE);
    vector<string> newStudentGrade = { id };
    for (int i = 0; i < grade[0].size() - 1; i++) {
        newStudentGrade.push_back("- -");
    }

    return writeTxtFile(STUDENT_FILE, { {id, name, email, password} }) && writeTxtFile(GRADE_FILE, { newStudentGrade });
}

bool Student::del() {
    string temp;

    cout << "Enter student id: ";
    getline(cin, temp);

    if (!temp.empty()) {
        if (readTxtFile(STUDENT_FILE, 0, temp).empty()) {
            cout << "Student " << temp << " doesn't exists." << endl;
            return false;
        }
        else {
            return deleteRowTxtFile(STUDENT_FILE, 0, temp);
        }
    }
    else {
        cout << "Field cann't be empty." << endl;
        return false;
    }
}

bool Student::edit() {
    string temp;

    cout << "Enter student id: ";
    getline(cin, temp);

    if (!temp.empty()) {
        if (readTxtFile(STUDENT_FILE, 0, temp).empty()) {
            cout << "Student " << temp << " doesn't exists." << endl;
            return false;
        }
        else {
            while (true) {
                getInput("Name: ", name);
                break;
            }

            while (true) {
                getInput("Email: ", email);

                if (isValidEmail(email) || email.empty()) break;
                cout << "Email not valid.\n";
                if (!askRetry()) return false;
            }

            while (true) {
                getInput("Password (must be 6 characters long): ", password);

                if (password.size() >= 6 || password.empty()) break;
                cout << "Password not valid.\n";
                if (!askRetry()) return false;
            }

            return updateTxtFile(STUDENT_FILE, 0, temp, { temp, name, email, password });
        }
    }
    else {
        cout << "Field cann't be empty." << endl;
        return false;
    }
}

void Student::show() {
    vector<vector<string>> studentDetails = readTxtFile(STUDENT_FILE);

    for (int i = 0; i < studentDetails.size(); i++) {
        studentDetails[i] = { studentDetails[i][0], studentDetails[i][1], studentDetails[i][2] };
    }

    printTable(studentDetails);
}