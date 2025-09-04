#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>   

#include "teacher.hpp"
#include "common.hpp"

using namespace std;

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
}