#ifndef STUDENT_HPP
#define STUDENT_HPP

class Student {
private:
    std::string id;
    std::string name;
    std::string grade;
    std::string dob;
    std::string address;
    std::string email;
    std::string contact;
    std::string guardianName;
    std::string guardianContact;
    std::string joinedDate;
    std::string res;

    std::string STUDENT_DETAILS = "student.txt";
    std::string STUDENT_MARKS = "grade.txt";
    std::string COURSE_DETAILS = "courses.txt";

public:
    std::vector<std::string> askDetails();
    bool addStudent();
    void showStudent(int colNumber = -1, std::string colValue = "");
    bool editStudent(int colNumber, std::string colValue);
    bool deleteStudent(std::string colValue);
    void insertMarks(std::string studentId, std::string courseCode, std::string mark, int markType);
    bool enrollStudent(std::string studentId);
    bool disenrollStudent(std::string studentId);
};

#endif
