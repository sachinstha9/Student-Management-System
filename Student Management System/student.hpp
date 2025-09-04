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
    std::string courses;

    std::string STUDENT_DETAILS = "student.txt";
    std::string STUDENT_ENROLLMENT = "student_enrollment.txt";
    std::string COURSE_DETAILS = "courses.txt";

    //Student(std::string temp = "") {
    //    id = temp;
    //    name = temp;
    //    grade = temp;
    //    dob = temp;
    //    address = temp;
    //    email = temp;
    //    contact = temp;
    //    guardianName = temp;
    //    guardianContact = temp;
    //    joinedDate = temp;
    //}
public:
    std::vector<std::string> askDetails();
    bool addStudent();
    void showStudent(int colNumber = -1, std::string colValue = "");
    bool editStudent(int colNumber, std::string colValue);
    bool deleteStudent(std::string colValue);
    bool enrollStudent(std::string studentId);
};

#endif
