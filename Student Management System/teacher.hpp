#ifndef TEACHER_HPP
#define TEACHER_HPP

class Teacher {
private:
    std::string id;
    std::string name;
    std::string course;
    std::string joinedDate;
    std::string res;

    std::string STUDENT_DETAILS = "student.txt";
    std::string STUDENT_MARKS = "grade.txt";
    std::string COURSE_DETAILS = "courses.txt";
    std::string TEACHER_DETAILS = "teacher.txt";

public:
    std::vector<std::string> askDetails();
    bool addTeacher();
    bool inputMarks(std::string teacherId);
};

#endif
