#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "course.hpp"
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

static string calculateGrade(double finalScore) {
    if (finalScore >= 90.0) return "A+";
    else if (finalScore >= 85.0) return "A";
    else if (finalScore >= 80.0) return "A-";
    else if (finalScore >= 75.0) return "B+";
    else if (finalScore >= 70.0) return "B";
    else if (finalScore >= 65.0) return "B-";
    else if (finalScore >= 60.0) return "C+";
    else if (finalScore >= 55.0) return "C";
    else if (finalScore >= 50.0) return "C-";
    else return "F";
}

bool Course::add() {
    while (true) {
        if (!getInput("Course Code: ", code)) {
            if (!askRetry()) return false;
            continue;
        }
        if (readTxtFile(COURSE_FILE, 0, code).size() == 0) break;
        cout << "Course code already exists (Must be unique).\n";
        if (!askRetry()) return false;
    }

    while (true) {
        if (!getInput("Course Name: ", name)) {
            if (!askRetry()) return false;
            continue;
        }

        if (!name.empty()) break;
        cout << "Field cannot be empty.\n";
        if (!askRetry()) return false;
    }

    vector<vector<string>> grade = readTxtFile(GRADE_FILE);
    vector<string> newGradeCol = { code };
    for (int i = 0; i < grade.size() - 1; i++) {
        newGradeCol.push_back("- -");
    }

    return writeTxtFile(COURSE_FILE, { {code, name, ""} }) && addColumnToTxtFile(GRADE_FILE, newGradeCol);
}

bool Course::del() {
    string temp;

    cout << "Enter course code: ";
    getline(cin, temp);

    if (!temp.empty()) {
        if (readTxtFile(COURSE_FILE, 0, temp).empty()) {
            cout << "Course " << temp << " doesn't exists." << endl;
            return false;
        }
        else {
            return deleteRowTxtFile(COURSE_FILE, 0, temp);
        }
    }
    else {
        cout << "Field cann't be empty." << endl;
        return false;
    }
}

bool Course::edit() {
    string temp;

    cout << "Enter course code: ";
    getline(cin, temp);

    if (!temp.empty()) {
        if (readTxtFile(COURSE_FILE, 0, temp).empty()) {
            cout << "Course " << temp << " doesn't exists." << endl;
            return false;
        }
        else {
            while (true) {
                getInput("Course Code: ", code);

                if (!readTxtFile(COURSE_FILE, 0, code).empty()) break;
                cout << "Course code already exists (Must be unique).\n";
                if (!askRetry()) return false;
            }

            while (true) {
                getInput("Course Name: ", name);
                break;
            }

            while (true) {
                getInput("Course Description: ", description);
                break;
            }

            return updateTxtFile(COURSE_FILE, 0, temp, { code, name, description });
        }
    }
    else {
        cout << "Field cann't be empty." << endl;
        return false;
    }
}

bool Course::editMark(string id, string course, int markType, string mark) {
    vector<vector<string>> targetRow = readTxtFile(GRADE_FILE, 0, id);
    vector<vector<string>> topRow = readTxtFile(GRADE_FILE, 0, "index");

    for (int i = 1; i < topRow[0].size(); i++) {
        if (course == topRow[0][i]) {
            vector<string> markVec = split(targetRow[0][i]);
            if (mark != "-")
                if (stoi(mark) < 0 || stoi(mark) > 100) {
                    cout << "Mark cannot be above 100 or below 0." << endl;
                    return false;
                }

            if (markType == 1)
                targetRow[0][i] = mark + " " + markVec[1];
            else if (markType == 2)
                targetRow[0][i] = markVec[0] + " " + mark;
        }
    }

    return updateTxtFile(GRADE_FILE, 0, id, targetRow[0]);
}

vector<string> Course::getMarks(const string& studentId, const string& course) {
    vector<vector<string>> targetRow = readTxtFile(GRADE_FILE, 0, studentId);
    vector<vector<string>> topRow = readTxtFile(GRADE_FILE, 0, "index");

    if (targetRow.empty() || topRow.empty()) {
        return { "Error: Grade data not found" };
    }

    for (int i = 1; i < topRow[0].size(); i++) {
        if (topRow[0][i] == course) {
            string gradeEntry = targetRow[0][i];

            auto parts = split(gradeEntry);
            if (parts.size() != 2) {
                return { "Error: Invalid grade format" };
            }

            string internal = parts[0];
            string final = parts[1];

            return { internal, final };
        }
    }

    return { "Error: Course not found in grade sheet" };
}

bool Course::enroll(string studentId) {
    string tempCode, tempId;

    cout << "Enter course code: ";
    getline(cin, tempCode);
    tempCode = trim(tempCode);

    if (!tempCode.empty()) {
        if (readTxtFile(COURSE_FILE, 0, tempCode).empty()) {
            cout << "Course " << tempCode << " doesn't exists." << endl;
            return false;
        }
    }
    else {
        cout << "Field can't be empty." << endl;
        return false;
    }

    if (studentId.empty()) {
        cout << "Enter student id: ";
        getline(cin, tempId);
        tempId = trim(tempId);

        if (!tempId.empty()) {
            if (readTxtFile(STUDENT_FILE, 0, tempId).empty()) {
                cout << "Student " << tempId << " doesn't exists." << endl;
                return false;
            }
        }
        else {
            cout << "Field can't be empty." << endl;
            return false;
        }
    }
    else
        tempId = studentId;

    vector<string> marks = getMarks(tempId, tempCode);

    if (marks.size() == 2)
        if (marks[0] == "-" && marks[1] == "-") {
            return editMark(tempId, tempCode, 1, "0") &&
                editMark(tempId, tempCode, 2, "0");
        }
        else {
            cout << "Student " << tempId << " already enrolled in course " << tempCode << "." << endl;
            return false;
        }


    return false;
}

bool Course::disenroll(string studentId) {
    string tempCode, tempId;

    cout << "Enter course code: ";
    getline(cin, tempCode);
    tempCode = trim(tempCode);

    if (!tempCode.empty()) {
        if (readTxtFile(COURSE_FILE, 0, tempCode).empty()) {
            cout << "Course " << tempCode << " doesn't exists." << endl;
            return false;
        }
    }
    else {
        cout << "Field can't be empty." << endl;
        return false;
    }

    if (studentId.empty()) {
        cout << "Enter student id: ";
        getline(cin, tempId);
        tempId = trim(tempId);

        if (!tempId.empty()) {
            if (readTxtFile(STUDENT_FILE, 0, tempId).empty()) {
                cout << "Student " << tempId << " doesn't exists." << endl;
                return false;
            }
        }
        else {
            cout << "Field can't be empty." << endl;
            return false;
        }
    }
    else
        tempId = studentId;

    vector<string> marks = getMarks(tempId, tempCode);

    if (marks.size() == 2)
        if (marks[0] != "-" && marks[1] != "-") {
            return editMark(tempId, tempCode, 1, "-") &&
                editMark(tempId, tempCode, 2, "-");
        }
        else {
            cout << "Student " << tempId << " not enrolled in the course " << tempCode << "." << endl;
            return false;
        }


    return false;
}

bool Course::assignTeacher() {
    string tempCode, tempId;

    cout << "Enter course code: ";
    getline(cin, tempCode);
    tempCode = trim(tempCode);

    if (!tempCode.empty()) {
        if (readTxtFile(COURSE_FILE, 0, tempCode).empty()) {
            cout << "Course " << tempCode << " doesn't exists." << endl;
            return false;
        }
    }
    else {
        cout << "Field can't be empty." << endl;
        return false;
    }

    cout << "Enter teacher id: ";
    getline(cin, tempId);
    tempId = trim(tempId);

    if (!tempId.empty()) {
        if (readTxtFile(TEACHER_FILE, 0, tempId).empty()) {
            cout << "Teacher " << tempId << " doesn't exists." << endl;
            return false;
        }
    }
    else {
        cout << "Field can't be empty." << endl;
        return false;
    }

    vector<string> courseDetails = readTxtFile(COURSE_FILE, 0, tempCode)[0];

    if (courseDetails[2] == "-")
        courseDetails[2] = tempId;
    else {
        vector<string> assignedTeacherVec = split(courseDetails[2]);
        if (find(assignedTeacherVec.begin(), assignedTeacherVec.end(), tempId) != assignedTeacherVec.end()) {
            cout << "Teacher " << tempId << " already assigned to course " << tempCode << "." << endl;
            return false;
        }
        else
            courseDetails[2] += " " + tempId;
    }

    return updateTxtFile(COURSE_FILE, 0, tempCode, { courseDetails[0], courseDetails[1], courseDetails[2] });
}

bool Course::disassignTeacher() {
    string tempCode, tempId;

    cout << "Enter course code: ";
    getline(cin, tempCode);
    tempCode = trim(tempCode);

    if (!tempCode.empty()) {
        if (readTxtFile(COURSE_FILE, 0, tempCode).empty()) {
            cout << "Course " << tempCode << " doesn't exists." << endl;
            return false;
        }
    }
    else {
        cout << "Field can't be empty." << endl;
        return false;
    }

    cout << "Enter teacher id: ";
    getline(cin, tempId);
    tempId = trim(tempId);

    if (!tempId.empty()) {
        if (readTxtFile(TEACHER_FILE, 0, tempId).empty()) {
            cout << "Teacher " << tempId << " doesn't exists." << endl;
            return false;
        }
    }
    else {
        cout << "Field can't be empty." << endl;
        return false;
    }

    vector<string> courseDetails = readTxtFile(COURSE_FILE, 0, tempCode)[0];
    string newTeacher = "";
    for (auto c : split(courseDetails[2])) {
        if (c != tempId)
            newTeacher += c + " ";
    }

    newTeacher = trim(newTeacher);

    if (newTeacher.empty())
        newTeacher = "-";

    cout << newTeacher;

    return updateTxtFile(COURSE_FILE, 0, tempCode, { courseDetails[0], courseDetails[1], newTeacher });
}

vector<string> Course::getEnrolledCourses(string studentId) {
    string tempId;

    if (studentId.empty()) {
        cout << "Enter student id: ";
        getline(cin, tempId);
        tempId = trim(tempId);

        if (!tempId.empty()) {
            if (readTxtFile(STUDENT_FILE, 0, tempId).empty()) {
                cout << "Student " << tempId << " doesn't exists." << endl;
                return {};
            }
        }
        else {
            cout << "Field can't be empty." << endl;
            return {};
        }
    }
    else
        tempId = studentId;

    vector<string> targetRow = readTxtFile(GRADE_FILE, 0, tempId)[0];
    vector<string> topRow = readTxtFile(GRADE_FILE)[0];
    vector<string> enrolledCourses;

    for (int i = 1; i < topRow.size(); i++) {
        if (targetRow[i] != "- -") enrolledCourses.push_back(topRow[i]);
    }

    if (enrolledCourses.empty()) cout << "Student " << tempId << " not enrolled in any courses." << endl;

    return enrolledCourses;
}

void Course::generateReport(string studentId) {
    string tempId;

    if (studentId.empty()) {
        cout << "Enter student id: ";
        getline(cin, tempId);
        tempId = trim(tempId);

        if (!tempId.empty()) {
            if (readTxtFile(STUDENT_FILE, 0, tempId).empty()) {
                cout << "Student " << tempId << " doesn't exists." << endl;
                return;
            }
        }
        else {
            cout << "Field can't be empty." << endl;
            return;
        }
    }
    else tempId = studentId;

    vector<vector<string>> studentDetails = readTxtFile(STUDENT_FILE, 0, tempId);
    vector<string> enrolledCourses = getEnrolledCourses(tempId);
    int noOfCourse = 0;
    float avgFinalScore = 0.0;
    string overallGrade = "-";

    cout << "==============================================" << endl;
    cout << "------------STUDENT ACADEMIC REPORT-----------" << endl;
    cout << "==============================================" << endl;
    cout << endl;
    cout << "Student Details: " << endl;
    cout << "Student Id: " << tempId << endl;
    cout << "Name: " << studentDetails[0][1] << endl;
    cout << "Email: " << studentDetails[0][3] << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Course Enrollment & Performance" << endl;
    cout << "----------------------------------------------" << endl;

    if (enrolledCourses.empty()) cout << "Student not enrolled in any courses." << endl;
    else {
        for (auto c : enrolledCourses) {
            vector<string> mark = getMarks(tempId, c);

            if (mark.size() != 2) {
                cout << mark[0] << endl;
                continue;
            }

            cout << "Course: " << c << " (" << c << ")" << endl;
            cout << "- Internal Assessment(30 %) : " << mark[0] << " / 100" << endl;
            cout << "- Final Assessment(70 %) : " << mark[1] << " / 100" << endl;

            float finalScore = stoi(mark[0]) * 0.3 + stoi(mark[1]) * 0.7;
            avgFinalScore += finalScore;
            cout << "- Final Score : " << to_string(finalScore) << " / 100" << endl;
            cout << "- Grade : " << calculateGrade(finalScore) << endl;
            cout << endl;
        }

        noOfCourse = enrolledCourses.size();
        avgFinalScore /= enrolledCourses.size();
        overallGrade = calculateGrade(avgFinalScore);
    }

    cout << "----------------------------------------------" << endl;
    cout << "Overall Academic Progress:" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Number of Courses : " << to_string(noOfCourse) << endl;
    cout << "Average Final Score : " << to_string(avgFinalScore) << endl;
    cout << "Overall Grade : " << overallGrade << endl;
    cout << "==============================================" << endl;
}

void Course::show() {
    vector<vector<string>> courseDetails = readTxtFile(COURSE_FILE);

    if(courseDetails.size() <= 1) {
        cout << "No courses to show." << endl;
        return;
    }

    printTable(courseDetails);
}