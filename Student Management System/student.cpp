#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

#include "student.hpp"
#include "common.hpp"

using namespace std;

vector<string> Student::askDetails() {
	cout << "Fill out the details to register new student.\n" << endl;

	while (true) {
		cout << "Name*: ";
		getline(cin, name);
		name = trim(name);

		if (name.empty()) {
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

	while (true) {
		cout << "Grade*: ";
		getline(cin, grade);

		if (grade.empty()) {
			cout << "This field cannot be empty." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else if (grade != "1" && grade != "2" && grade != "3" && grade != "4" && grade != "5" && grade != "6") {
			cout << "This field value is invalid (Must be between 1 and 6)." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else
			break;

		if (res == "q") {
			res = "";
			return {};
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}


	while (true) {
		cout << "DOB* (in YYYY-MM-DD format): ";
		getline(cin, dob);
		dob = trim(dob);

		if (dob.empty()) {
			cout << "This field cannot be empty." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else {
			if (isValidDateFormat(dob)) {
				if (dob >= getTodayDate()) {
					cout << "This field value is invalid (DOB must be lesser than today's)." << endl;
					cout << "Press enter to type again or type 'q' to quit." << endl;

					getline(cin, res);
				}
				else {
					break;
				}
			}
			else {
				cout << "This field value is invalid (Date format is not matched)." << endl;
				cout << "Press enter to type again or type 'q' to quit." << endl;

				getline(cin, res);
			}
		}

		if (res == "q") {
			res = "";
			return {};
		}
	}

	while (true) {
		cout << "Address*: ";
		getline(cin, address);
		address = trim(address);

		if (address.empty()) {
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

	while (true) {
		cout << "Email*: ";
		getline(cin, email);
		email = trim(email);

		if (email.empty()) {
			cout << "This field cannot be empty." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else if (!isValidEmail(email)) {
			cout << "This field value is invalid." << endl;
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
		cout << "Contact(optional): ";
		getline(cin, contact);
		contact = trim(contact);
		
		if(contact.empty())
			break;

		if (!isValidNZMobileNumber(contact)) {
			cout << "This field value is invalid." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}

		if (res == "q") {
			res = "";
			return {};
		}
	}

	while (true) {
		cout << "Guardian's Name*: ";
		getline(cin, guardianName);
		guardianName = trim(guardianName);

		if (guardianName.empty()) {
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

	while (true) {
		cout << "Guardian's Contact*: ";
		getline(cin, guardianContact);
		guardianContact = trim(guardianContact);

		if (guardianContact.empty()) {
			cout << "This field cannot be empty." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}
		else if (!isValidNZMobileNumber(guardianContact)) {
			cout << "This field value is invalid." << endl;
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

	joinedDate = getTodayDate();

	string todayDate = joinedDate;
	todayDate.erase(std::remove(todayDate.begin(), todayDate.end(), '-'), todayDate.end());
	int totalNumOfS = countRowsInFile(STUDENT_DETAILS, 2, grade);

	id = todayDate + to_string(totalNumOfS + 1);

	return { id, name, grade, dob, address, email, contact, guardianName, guardianContact, joinedDate };
}

bool Student::addStudent() {
	return writeTxtFile(STUDENT_DETAILS, { askDetails() });
}

void Student::showStudent(int colNumber, string colValue) {
	printTable(readTxtFile(STUDENT_DETAILS, colNumber, colValue));
}

bool Student::editStudent(int colNumber, string colValue) {
	cout << "Fill out the details to edit details of student." << endl;
	cout << "Leave blank to leave it as it was.\n" << endl;

	if (countRowsInFile(STUDENT_DETAILS, colNumber, colValue) != 1) {
		cout << "Sorry, student with provided id doesn't exists." << endl;
		return false;
	}

	cout << "Name*: ";
	getline(cin, name);
	name = trim(name);

	while (true) {
		cout << "Grade*: ";
		getline(cin, grade);

		if (grade.empty())
			break;
		else if (grade != "1" && grade != "2" && grade != "3" && grade != "4" && grade != "5" && grade != "6") {
			cout << "This field value is invalid (Must be between 1 and 6)." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}

		if (res == "q") {
			res = "";
			return false;
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}


	while (true) {
		cout << "DOB* (in YYYY-MM-DD format): ";
		getline(cin, dob);
		dob = trim(dob);

		if (dob.empty())
			break;
		else {
			if (isValidDateFormat(dob)) {
				if (dob >= getTodayDate()) {
					cout << "This field value is invalid (DOB must be lesser than today's)." << endl;
					cout << "Press enter to type again or type 'q' to quit." << endl;

					getline(cin, res);
				}
				else {
					break;
				}
			}
			else {
				cout << "This field value is invalid (Date format is not matched)." << endl;
				cout << "Press enter to type again or type 'q' to quit." << endl;

				getline(cin, res);
			}
		}

		if (res == "q") {
			res = "";
			return false;
		}
	}

	cout << "Address*: ";
	getline(cin, address);
	address = trim(address);

	while (true) {
		cout << "Email*: ";
		getline(cin, email);
		email = trim(email);

		if (email.empty())
			break;
		else if (!isValidEmail(email)) {
			cout << "This field value is invalid." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}

		if (res == "q") {
			res = "";
			return false;
		}
	}

	while (true) {
		cout << "Contact(optional): ";
		getline(cin, contact);
		contact = trim(contact);

		if (contact.empty())
			break;
		else if (!isValidNZMobileNumber(contact)) {
			cout << "This field value is invalid." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}

		if (res == "q") {
			res = "";
			return false;
		}
	}


	cout << "Guardian's Name*: ";
	getline(cin, guardianName);
	guardianName = trim(guardianName);


	while (true) {
		cout << "Guardian's Contact*: ";
		getline(cin, guardianContact);
		guardianContact = trim(guardianContact);

		if (guardianContact.empty())
			break;
		else if (!isValidNZMobileNumber(guardianContact)) {
			cout << "This field value is invalid." << endl;
			cout << "Press enter to type again or type 'q' to quit." << endl;

			getline(cin, res);
		}

		if (res == "q") {
			res = "";
			return false;
		}
	}

	return updateTxtFile(STUDENT_DETAILS, 0, trim(colValue), { {id, name, grade, dob, address, email, contact, guardianName, guardianContact, joinedDate} });
}

bool Student::deleteStudent(std::string colValue) {
	return deleteRowTxtFile(STUDENT_DETAILS, 0, colValue);
}

bool Student::enrollStudent(string studentId) {
	if (countRowsInFile(STUDENT_DETAILS, 0, studentId) != 1) {
		cout << "Sorry, student with provided id doesn't exists." << endl;
		return false;
	}

	cout << "Enter courses (Put space in between): ";
	getline(cin, courses);
	courses = trim(courses);
	vector<string> coursesVec = splitBySpace(courses);

	bool checkCourseValidity = true;
	string marks = "";
	string newCourses = "";
	string newMarks = "";

	for (auto c : splitBySpace(courses)) {
		if (countRowsInFile(COURSE_DETAILS, 0, c) != 1) {
			cout << c << " doesn't exists." << endl;
			checkCourseValidity = false;
		}
	}

	if (!checkCourseValidity) return false;

	vector<vector<string>> studentCourseDetails = readTxtFile(STUDENT_ENROLLMENT, 0, studentId);

	if (studentCourseDetails.size() == 1) {
		vector<string> oldCourses = splitBySpace(studentCourseDetails[0][1]);

		for (const auto& course : coursesVec) {
			if (find(oldCourses.begin(), oldCourses.end(), course) == oldCourses.end()) {
				newCourses += course + " ";
				newMarks += "0 ";
			}
			else
				cout << "Course " << course << " already enrolled.\n";
		}

		newCourses = trim(newCourses);
		newMarks = trim(newMarks);

		newCourses = studentCourseDetails[0][1] + " " + newCourses;
		newMarks = studentCourseDetails[0][2] + " " + newMarks;

		return updateTxtFile(STUDENT_ENROLLMENT, 0, studentId, { studentId, newCourses, newMarks });
	}

	for (auto c : splitBySpace(courses)) {
		marks += " 0";
	}

	return writeTxtFile(STUDENT_ENROLLMENT, { {studentId, courses, marks}});
}

bool Student::disenrollStudent(string studentId) {
	if (countRowsInFile(STUDENT_DETAILS, 0, studentId) != 1) {
		cout << "Sorry, student with provided id doesn't exists." << endl;
		return false;
	}

	if (countRowsInFile(STUDENT_ENROLLMENT, 0, studentId) != 1) {
		cout << "Sorry, student with provided id is not enrolled in any courses." << endl;
		return false;
	}

	cout << "Enter courses (Put space in between): ";
	getline(cin, courses);
	courses = trim(courses);

	vector<string> coursesVec = splitBySpace(courses);

	bool checkCourseValidity = true;
	string marks;

	for (auto c : coursesVec) {
		if (countRowsInFile(COURSE_DETAILS, 0, c) != 1) {
			cout << c << " doesn't exists." << endl;
			checkCourseValidity = false;
		}
	}

	if (!checkCourseValidity) return false;

	vector<vector<string>> enrollStudentDetails = readTxtFile(STUDENT_ENROLLMENT, 0, studentId);
	vector<string> enrolledCourses = splitBySpace(enrollStudentDetails[0][1]);
	vector<string> enrolledCoursesMarks = splitBySpace(enrollStudentDetails[0][2]);

	string newEnrolledCourses = "";
	string newEnrolledCoursesMarks = "";

	for (const auto& course : coursesVec) {
		if (find(enrolledCourses.begin(), enrolledCourses.end(), course) == enrolledCourses.end()) {
			cout << "Course " << course << " not found in enrolled courses." << endl;
		}
	}

	unordered_set<string> toRemoveSet(coursesVec.begin(), coursesVec.end());

	for (size_t i = 0; i < enrolledCourses.size(); ++i) {
		if (toRemoveSet.find(enrolledCourses[i]) == toRemoveSet.end()) {
			newEnrolledCourses += enrolledCourses[i] + " ";
			newEnrolledCoursesMarks += enrolledCoursesMarks[i] + " ";
		}
	}

	newEnrolledCourses = trim(newEnrolledCourses);
	newEnrolledCoursesMarks = trim(newEnrolledCoursesMarks);

	if (courses == newEnrolledCourses)
		return false;

	if (newEnrolledCourses.empty() && newEnrolledCoursesMarks.empty())
		return deleteRowTxtFile(STUDENT_ENROLLMENT, 0, studentId);

	return updateTxtFile(STUDENT_ENROLLMENT, 0, studentId, { studentId, newEnrolledCourses, newEnrolledCoursesMarks });
}