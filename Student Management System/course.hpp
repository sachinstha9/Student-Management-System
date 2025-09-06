#ifndef COURSE_HPP
#define COURSE_HPP

class Course {
private:
    std::string code;
    std::string name;
    std::string description;
    std::string teachers;

    std::string COURSE_FILE = "COURSEDETAILS.txt",
        STUDENT_FILE = "STUDENTDETAILS.txt",
        TEACHER_FILE = "TEACHERDETAILS.txt",
        ADMIN_FILE = "ADMINDETAILS.txt",
        GRADE_FILE = "GRADE.txt";

public:
    bool add();
    bool del();
    bool edit();
    bool editMark(std::string id, std::string course, int markType, std::string mark);
    std::vector<std::string> getMarks(const std::string& studentId, const std::string& courseName);
    bool enroll(std::string studentId = "");
    bool disenroll(std::string studentId = "");
    bool assignTeacher();
    bool disassignTeacher();
    std::vector<std::string> getEnrolledCourses(std::string studentId = "");
    void generateReport(std::string studentId = "");
    void show();
};

#endif