#include <iostream>
#include <string>
#include <vector>
<<<<<<< HEAD
#include <algorithm>
#include <cstdlib>
#include <ctime>   

#include "teacher.hpp"
#include "filemodification.hpp"
=======
#include <algorithm> 
#include <cctype>   

#include "teacher.hpp"
>>>>>>> d3e046512ab807f351ee6c8daeceb103ccf7fd40
#include "common.hpp"

using namespace std;

<<<<<<< HEAD
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
=======
vector<string> Teacher::askDetails() {
    cout << "Fill out the details to register new teacher.\n" << endl;

    while (true) {
        cout << "Name*: ";
        getline(cin, name);
        name = trim(name);

        if (name.empty()) {
            cout << "This field cannot be empty." << endl;
            cout << "Press enter to type again or type 'q' to quit." << endl;

            getline(cin, res);
            if (res == "q") {
                return {};
            }
        }
        else {
            break;
        }
    }

    while (true) {
        cout << "Course*: ";
        getline(cin, course);
        course = trim(course);

        if (course.empty()) {
            cout << "This field cannot be empty." << endl;
            cout << "Press enter to type again or type 'q' to quit." << endl;

            getline(cin, res);
            if (res == "q") {
                return {};
            }
            continue;
        }

        vector<string> coursesVec = splitBySpace(course);
        bool valid = true;

        for (const auto& c : coursesVec) {
            if (countRowsInFile(COURSE_DETAILS, 0, c) != 1) {
                cout << c << " doesn't exist." << endl;
                valid = false;
            }
        }

        if (!valid) {
            cout << "Press enter to type again or type 'q' to quit." << endl;
            getline(cin, res);
            if (res == "q") {
                return {};
            }
        }
        else {
            break;
        }
    }

    joinedDate = getTodayDate();

    string todayDate = joinedDate;
    todayDate.erase(std::remove(todayDate.begin(), todayDate.end(), '-'), todayDate.end());
    int totalNumOfS = countRowsInFile(TEACHER_DETAILS);

    id = todayDate + to_string(totalNumOfS + 1);

    return { id, name, course, joinedDate };
}

bool Teacher::addTeacher() {
    return writeTxtFile(TEACHER_DETAILS, { askDetails() });
}

bool Teacher::inputMarks(string teacherId) {
    string studentId, course, courseTeacher, markType, mark;

    vector<string> courseDet, teacherDetails = readTxtFile(TEACHER_DETAILS, 0, teacherId)[0];

    if (teacherDetails.empty()) {
        cout << "Teacher doesn't exists." << endl;
        return false;
    }

    while (true) {
        cout << "Enter student's id*: ";
        getline(cin, studentId);
        studentId = trim(studentId);

        if (studentId.empty()) {
            cout << "This field cannot be empty." << endl;
            cout << "Press enter to type again or type 'q' to quit." << endl;

            getline(cin, res);
        }
        else {
            if (readTxtFile(STUDENT_DETAILS, 0, studentId)[0].empty()) {
                cout << "This field value is invalid (Must be register student)." << endl;
                cout << "Press enter to type again or type 'q' to quit." << endl;

                getline(cin, res);
            }
            else
                break;
        }


        if (res == "q") {
            res = "";
            return false;
        }
    }


    while (true) {
        cout << "Enter course code*: ";
        getline(cin, course);

        if (course.empty()) {
            cout << "This field cannot be empty." << endl;
            cout << "Press enter to type again or type 'q' to quit." << endl;

            getline(cin, res);
        }
        else {
            courseDet = readTxtFile(COURSE_DETAILS, 0, course)[0];

            if (courseDet.empty()) {
                cout << "This field value is invalid (Must be register course)." << endl;
                cout << "Press enter to type again or type 'q' to quit." << endl;

                getline(cin, res);
            } else
                break;
        }
        

        if (res == "q") {
            res = "";
            return false;
        }
    }

    if (courseDet[3] != teacherId) {
        cout << courseDet[3] << "|" << teacherId << endl;
        cout << "You are not permitted to change the marks for this course." << endl;
        cout << "Marks can be only changed by respected teacher of the course." << endl;
        return false;
    }

    while (true) {
        cout << "\nMark type" << endl;
        cout << "0 - internal marks" << endl;
        cout << "1 - final marks" << endl;
        cout << "2 - total marks" << endl;
        cout << "Enter the marks type: ";
        getline(cin, markType);
        markType = trim(markType);

        if (markType.empty()) {
            cout << "This field cannot be empty." << endl;
            cout << "Press enter to type again or type 'q' to quit." << endl;

            getline(cin, res);
        }
        else if(markType != "0" && markType != "1" && markType != "2") {
            cout << "This field valid is not valid. (Must be between 0 to 2)." << endl;
            cout << "Press enter to type again or type 'q' to quit." << endl;

            getline(cin, res);
        }
        else
            break;


        if (res == "q") {
            res = "";
            return false;
        }
    }

    while (true) {
        cout << "Enter mark: ";
        getline(cin, mark);
        mark = trim(mark);

        if (mark.empty()) {
            cout << "This field cannot be empty." << endl;
            cout << "Press enter to type again or type 'q' to quit." << endl;

            getline(cin, res);
        }
        else if (stoi(mark) < 0 || stoi(mark) > 100) {
            cout << "This field valid is not valid. (Must be between 0 to 100)." << endl;
            cout << "Press enter to type again or type 'q' to quit." << endl;

            getline(cin, res);
        }
        else
            break;


        if (res == "q") {
            res = "";
            return false;
        }
    }

    vector<string> targetRow = readTxtFile(STUDENT_MARKS, 0, studentId)[0];
    vector<string> topRow = readTxtFile(STUDENT_MARKS, 0, "index")[0];

    for (int i = 1; i < topRow.size(); i++) {
        if (topRow[i] == course) {
            vector<string> markVec = splitBySpace(targetRow[i]);
            string finalMark;

            if (markVec[stoi(markType)] == "-") {
                cout << "Student not registered in the course." << endl;
                return false;
            }

            for (int j = 0; j < 3; j++) {
                if (j == stoi(markType) || stoi(markType) == 3) {
                    finalMark += mark + " ";
                }
                else if (j < markVec.size()) {
                    finalMark += markVec[j] + " ";
                }
                else {
                    finalMark += "0 "; 
                }
            }

            targetRow[i] = trim(finalMark);
            break;
        }
    }

    updateTxtFile(STUDENT_MARKS, 0, studentId, targetRow);
>>>>>>> d3e046512ab807f351ee6c8daeceb103ccf7fd40
}