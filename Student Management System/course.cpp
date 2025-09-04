#include <iostream>
#include <string>
#include <vector>

#include "course.hpp"
#include "common.hpp"

using namespace std;

std::vector<std::string> Course::askDetails() {
	cout << "Fill out the details to add new course.\n" << endl;

	while (true) {
		cout << "Couse Code*: ";
		getline(cin, courseCode);

		if (courseCode.empty()) {
			cout << "This field cannot be empty." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else if (countRowsInFile(COURSE_DETAILS, 0, courseCode) != 0) {
			cout << "This field value is invalid (Must be unique)." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else
			break;

		if (res == "q") {
			res = "";
			return {};
		}
	}

	while (true) {
		cout << "Couse Name*: ";
		getline(cin, courseName);

		if (courseName.empty()) {
			cout << "This field cannot be empty." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else if (countRowsInFile(COURSE_DETAILS, 1, courseName) != 0) {
			cout << "This field value is invalid (Must be unique)." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else
			break;

		if (res == "q") {
			res = "";
			return {};
		}
	}
	
	cout << "Couse Description: ";
	getline(cin, courseDesc);

	while (true) {
		cout << "Couse Teacher*: ";
		getline(cin, courseTeacher);

		if (courseTeacher.empty()) {
			cout << "This field cannot be empty." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else
			break;

		if (res == "q") {
			res = "";
			return {};
		}
	}

	return { courseCode, courseName, courseDesc, courseTeacher };
}

bool Course::addCourse() {
	return writeTxtFile(COURSE_DETAILS, { askDetails() });
}

void Course::showCourse(int colNumber, std::string colValue) {
	printTable(readTxtFile(COURSE_DETAILS, colNumber, colValue));
}

bool Course::editCourse(int colNumber, std::string colValue) {
	cout << "Fill out the details to edit details of course." << endl;
	cout << "Leave blank to leave it as it was.\n" << endl;

	if (countRowsInFile(COURSE_DETAILS, colNumber, colValue) != 1) {
		cout << "Sorry, course with provided id doesn't exists." << endl;
		return false;
	}

	while (true) {
		cout << "Couse Code*: ";
		getline(cin, courseCode);

		if (countRowsInFile(COURSE_DETAILS, 0, courseCode) != 0) {
			cout << "This field value is invalid (Must be unique)." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else
			break;

		if (res == "q") {
			res = "";
			return {};
		}
	}

	while (true) {
		cout << "Couse Name*: ";
		getline(cin, courseName);

		if (countRowsInFile(COURSE_DETAILS, 1, courseName) != 0) {
			cout << "This field value is invalid (Must be unique)." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else
			break;

		if (res == "q") {
			res = "";
			return {};
		}
	}

	cout << "Couse Description: ";
	getline(cin, courseDesc);

	cout << "Couse Teacher*: ";
	getline(cin, courseTeacher);

	return updateTxtFile(COURSE_DETAILS, 0, trim(colValue), { {courseCode, courseName, courseDesc, courseTeacher} });
}

bool Course::deleteCourse(std::string courseCode) {
	return deleteRowTxtFile(COURSE_DETAILS, 0, courseCode);
}

