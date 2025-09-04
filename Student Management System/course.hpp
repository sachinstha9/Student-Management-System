#ifndef COURSE_HPP
#define COURSE_HPP

class Course {
private:
    std::string courseCode;
    std::string courseName;
    std::string courseDesc;
    std::string courseTeacher;
    std::string COURSE_DETAILS = "courses.txt";
    std::string res;

public:
    std::vector<std::string> askDetails();
    bool addCourse();
    void showCourse(int colNumber = -1, std::string colValue = "");
    bool editCourse(int colNumber, std::string colValue);
    bool deleteCourse(std::string colValue);
};

#endif
