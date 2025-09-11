#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

#include "course.hpp"
#include "filemodification.hpp"
#include "common.hpp"

using namespace std;

// simple input helper
static auto getInput = [](const string& prompt, string& value) -> bool {
    cout << prompt;
    getline(cin, value);
    value = trim(value);
    return !value.empty();
    };

// retry prompt
static auto askRetry = []() -> bool {
    char res;
    cout << "Do you want to try again? (y/n): ";
    cin >> res;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return tolower(res) == 'y';
    };

// join vector into string
static auto joinVector = [](const vector<string>& vec, const string& delim = " ") -> string {
    string result;
    for (size_t i = 0; i < vec.size(); ++i) {
        result += vec[i];
        if (i < vec.size() - 1) result += delim;
    }
    return result;
    };

// grade calculation
static string calculateGrade(double finalScore) {
    if (finalScore >= 90.0) return "A+";
    if (finalScore >= 85.0) return "A";
    if (finalScore >= 80.0) return "A-";
    if (finalScore >= 75.0) return "B+";
    if (finalScore >= 70.0) return "B";
    if (finalScore >= 65.0) return "B-";
    if (finalScore >= 60.0) return "C+";
    if (finalScore >= 55.0) return "C";
    if (finalScore >= 50.0) return "C-";
    return "F";
}

// -------------------
// CRUD operations
// -------------------
bool Course::add() {
    while (true) {
        getInput("Course Code: ", code);
        if (!code.empty() && readTxtFile(COURSE_FILE, 0, code).empty()) break;
        cout << "Course code must be unique.\n";
        if (!askRetry()) return false;
    }

    if (!getInput("Course Name: ", name)) return false;

    vector<vector<string>> grade = readTxtFile(GRADE_FILE);
    vector<string> newGradeCol = { code };
    for (size_t i = 0; i < grade.size() - 1; ++i)
        newGradeCol.push_back("- -");

    return writeTxtFile(COURSE_FILE, { {code, name, "-"} }) &&
        addColumnToTxtFile(GRADE_FILE, newGradeCol);
}

bool Course::del() {
    cout << "Enter course code: ";
    string temp;
    getline(cin, temp);
    temp = trim(temp);

    if (temp.empty() || readTxtFile(COURSE_FILE, 0, temp).empty()) {
        cout << "Invalid course code.\n";
        return false;
    }

    return deleteRowTxtFile(COURSE_FILE, 0, temp);
}

bool Course::edit() {
    cout << "Enter course code to edit: ";
    string temp;
    getline(cin, temp);
    temp = trim(temp);

    if (temp.empty() || readTxtFile(COURSE_FILE, 0, temp).empty()) {
        cout << "Course not found.\n";
        return false;
    }

    getInput("New Course Code: ", code);
    getInput("New Course Name: ", name);
    getInput("New Description: ", description);

    return updateTxtFile(COURSE_FILE, 0, temp, { code, name, description, teachers });
}

// -------------------
// Marks
// -------------------
bool Course::editMark(string id, string course, int markType, string mark) {
    auto targetRow = readTxtFile(GRADE_FILE, 0, id);
    auto topRow = readTxtFile(GRADE_FILE, 0, "index");

    for (size_t i = 1; i < topRow[0].size(); ++i) {
        if (topRow[0][i] == course) {
            vector<string> markVec = split(targetRow[0][i]);
            if (mark != "-") {
                int m = stoi(mark);
                if (m < 0 || m > 100) {
                    cout << "Mark must be 0-100.\n";
                    return false;
                }
            }
            targetRow[0][i] = (markType == 1 ? mark + " " + markVec[1] : markVec[0] + " " + mark);
        }
    }
    return updateTxtFile(GRADE_FILE, 0, id, targetRow[0]);
}

vector<string> Course::getMarks(const string& studentId, const string& courseName) {
    auto targetRow = readTxtFile(GRADE_FILE, 0, studentId);
    auto topRow = readTxtFile(GRADE_FILE, 0, "index");

    if (targetRow.empty() || topRow.empty()) return { "Error: Grade data not found" };

    for (size_t i = 1; i < topRow[0].size(); ++i)
        if (topRow[0][i] == courseName) return split(targetRow[0][i]);

    return { "Error: Course not found in grade sheet" };
}

// -------------------
// Enrollment
// -------------------
bool Course::enroll(string studentId) {
    cout << "Enter course code: ";
    string tempCode;
    getline(cin, tempCode);
    tempCode = trim(tempCode);

    if (tempCode.empty() || readTxtFile(COURSE_FILE, 0, tempCode).empty()) {
        cout << "Course not found.\n";
        return false;
    }

    if (studentId.empty()) {
        cout << "Enter student id: ";
        getline(cin, studentId);
        studentId = trim(studentId);
        if (studentId.empty() || readTxtFile(STUDENT_FILE, 0, studentId).empty()) {
            cout << "Student not found.\n";
            return false;
        }
    }

    auto marks = getMarks(studentId, tempCode);
    if (marks[0] == "-" && marks[1] == "-")
        return editMark(studentId, tempCode, 1, "0") && editMark(studentId, tempCode, 2, "0");

    cout << "Student already enrolled.\n";
    return false;
}

bool Course::disenroll(string studentId) {
    cout << "Enter course code: ";
    string tempCode;
    getline(cin, tempCode);
    tempCode = trim(tempCode);

    if (tempCode.empty() || readTxtFile(COURSE_FILE, 0, tempCode).empty()) {
        cout << "Course not found.\n";
        return false;
    }

    if (studentId.empty()) {
        cout << "Enter student id: ";
        getline(cin, studentId);
        studentId = trim(studentId);
        if (studentId.empty() || readTxtFile(STUDENT_FILE, 0, studentId).empty()) {
            cout << "Student not found.\n";
            return false;
        }
    }

    auto marks = getMarks(studentId, tempCode);
    if (marks[0] != "-" || marks[1] != "-")
        return editMark(studentId, tempCode, 1, "-") && editMark(studentId, tempCode, 2, "-");

    cout << "Student not enrolled.\n";
    return false;
}

// -------------------
// Teacher Assignment
// -------------------
bool Course::assignTeacher() {
    cout << "Enter course code: ";
    string tempCode;
    getline(cin, tempCode);
    tempCode = trim(tempCode);

    if (tempCode.empty() || readTxtFile(COURSE_FILE, 0, tempCode).empty()) {
        cout << "Course not found.\n";
        return false;
    }

    cout << "Enter teacher id: ";
    string tempId;
    getline(cin, tempId);
    tempId = trim(tempId);

    if (tempId.empty() || readTxtFile(TEACHER_FILE, 0, tempId).empty()) {
        cout << "Teacher not found.\n";
        return false;
    }

    vector<string> courseDetails = readTxtFile(COURSE_FILE, 0, tempCode)[0];
    vector<string> assigned = split(courseDetails[3]);
    if (find(assigned.begin(), assigned.end(), tempId) != assigned.end()) {
        cout << "Teacher already assigned.\n";
        return false;
    }

    courseDetails[3] = (courseDetails[3] == "-" ? tempId : courseDetails[3] + " " + tempId);
    return updateTxtFile(COURSE_FILE, 0, tempCode, courseDetails);
}

bool Course::disassignTeacher() {
    cout << "Enter course code: ";
    string tempCode;
    getline(cin, tempCode);
    tempCode = trim(tempCode);

    if (tempCode.empty() || readTxtFile(COURSE_FILE, 0, tempCode).empty()) {
        cout << "Course not found.\n";
        return false;
    }

    cout << "Enter teacher id: ";
    string tempId;
    getline(cin, tempId);
    tempId = trim(tempId);

    if (tempId.empty() || readTxtFile(TEACHER_FILE, 0, tempId).empty()) {
        cout << "Teacher not found.\n";
        return false;
    }

    vector<string> courseDetails = readTxtFile(COURSE_FILE, 0, tempCode)[0];
    vector<string> assigned = split(courseDetails[3]);
    assigned.erase(remove(assigned.begin(), assigned.end(), tempId), assigned.end());
    courseDetails[3] = assigned.empty() ? "-" : joinVector(assigned);

    return updateTxtFile(COURSE_FILE, 0, tempCode, courseDetails);
}

// -------------------
// Reports
// -------------------
vector<string> Course::getEnrolledCourses(string studentId) {
    if (studentId.empty()) {
        cout << "Enter student id: ";
        getline(cin, studentId);
        studentId = trim(studentId);
        if (studentId.empty() || readTxtFile(STUDENT_FILE, 0, studentId).empty()) {
            cout << "Student not found.\n";
            return {};
        }
    }

    auto targetRow = readTxtFile(GRADE_FILE, 0, studentId)[0];
    auto topRow = readTxtFile(GRADE_FILE)[0];
    vector<string> enrolled;

    for (size_t i = 1; i < topRow.size(); ++i)
        if (targetRow[i] != "- -") enrolled.push_back(topRow[i]);

    return enrolled;
}

void Course::generateReport(string studentId) {
    if (studentId.empty()) {
        cout << "Enter student id: ";
        getline(cin, studentId);
        studentId = trim(studentId);
        if (studentId.empty() || readTxtFile(STUDENT_FILE, 0, studentId).empty()) {
            cout << "Student not found.\n";
            return;
        }
    }

    auto studentDetails = readTxtFile(STUDENT_FILE, 0, studentId)[0];
    auto enrolledCourses = getEnrolledCourses(studentId);

    cout << "Student Id: " << studentId << "\nName: " << studentDetails[1] << "\nEmail: " << studentDetails[3] << "\n";

    double avgScore = 0;
    if (!enrolledCourses.empty()) {
        for (auto& c : enrolledCourses) {
            auto marks = getMarks(studentId, c);
            double score = stoi(marks[0]) * 0.3 + stoi(marks[1]) * 0.7;
            avgScore += score;
            cout << "Course: " << c << " | Internal: " << marks[0] << " | Final: " << marks[1]
                << " | Score: " << score << " | Grade: " << calculateGrade(score) << "\n";
        }
        avgScore /= enrolledCourses.size();
        cout << "Average Score: " << avgScore << " | Overall Grade: " << calculateGrade(avgScore) << "\n";
    }
    else {
        cout << "No courses enrolled.\n";
    }
}

void Course::show() {
    printTable(readTxtFile(COURSE_FILE));
}
