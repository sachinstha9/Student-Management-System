#ifndef TEACHER_HPP
#define TEACHER_HPP

class Teacher {
private:
    std::string id;
    std::string name;
<<<<<<< HEAD
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
=======
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
>>>>>>> d3e046512ab807f351ee6c8daeceb103ccf7fd40
