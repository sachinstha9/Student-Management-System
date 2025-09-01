#ifndef ADMIN_HPP
#define ADMIN_HPP

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
    void addStudent();
    void showStudent(int colNumber = -1, std::string colValue = "");
    void editStudent(int colNumber, std::string colValue);
};

#endif
