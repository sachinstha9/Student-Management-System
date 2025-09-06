#ifndef ADMIN_HPP
#define ADMIN_HPP

class Admin {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string password;

    std::string COURSE_FILE = "COURSEDETAILS.txt",
        STUDENT_FILE = "STUDENTDETAILS.txt",
        TEACHER_FILE = "TEACHERDETAILS.txt",
        ADMIN_FILE = "ADMINDETAILS.txt",
        GRADE_FILE = "GRADE.txt";

public:
    bool add();
    bool del();
    bool edit();
    void show();
};

#endif