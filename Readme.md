# 🎓 Student Management System

## 🌟 Overview

The Student Management System provides a **role-based console interface** designed for academic operations management. Built with C++14, it offers secure access controls and persistent data storage through plain text files.

### Key Highlights
- 🔐 **Multi-Role Authentication** - Admin, Teacher, Student access levels
- 📊 **Comprehensive Management** - Students, Teachers, Courses, Grades
- 💾 **Persistent Storage** - File-based data management
- 🖥️ **Command-Driven Interface** - Intuitive text-based interaction

---

## 📋 Quick Navigation

| Section | Description |
|---------|-------------|
| [🚀 Getting Started](#-getting-started) | Installation and setup instructions |
| [💡 Features](#-features) | Complete feature overview |
| [👤 User Roles](#-user-roles) | Role-specific capabilities |
| [🗂️ Architecture](#️-architecture) | Project structure and files |
| [⚡ Usage Guide](#-usage-guide) | How to use the system |
| [📚 Commands](#-commands) | Complete command reference |

---

## 👤 User Roles

### 🔧 Administrator
```
✅ Complete system control
✅ Manage all users and courses
✅ Teacher-course assignments
✅ Grade management
✅ System reports
```

### 👨‍🏫 Teacher
```
✅ Student and course management
✅ Grade assignment
✅ Course reports
✅ Profile updates
```

### 👩‍🎓 Student
```
✅ View personal information
✅ Course enrollment
✅ Grade viewing
✅ Profile updates
```

---

## 🗂️ Architecture

### Project Structure
```
📁 Student Management System/
│
├── 🔧 Core Files
│   ├── main.cpp                # Application entry point
│   ├── common.hpp              # Shared utilities
│   └── filemodification.hpp    # File I/O operations
│
├── 📝 Module Headers
│   ├── student.hpp             # Student operations
│   ├── teacher.hpp             # Teacher operations
│   ├── course.hpp              # Course operations
│   └── admin.hpp               # Admin operations
│
└── 📊 Data Files
    ├── STUDENTDETAILS.txt      # Student records
    ├── TEACHERDETAILS.txt      # Teacher records
    ├── COURSEDETAILS.txt       # Course records
    ├── ADMINDETAILS.txt        # Administrator records
    ├── GRADE.txt               # Grade records
    └── COMMANDS.txt            # Available commands
```

---

## ⚡ Usage Guide

### 1. Application Startup
```
🎓 Student Management System
==============================
Select User Type:
[1] Administrator
[2] Teacher  
[3] Student
>> 
```

### 2. Authentication
```
👤 Login Required
Username: your_username
Password: ********
✅ Login successful!
```

### 3. Command Interface
```
📝 Command Prompt
Available commands: help, exit, logout
>> help
📋 Displaying all available commands...
```

---

## 📚 Commands

### 🔐 System Commands
| Command | Function | Available to |
|---------|----------|-------------|
| `help` | 📖 Show command list | All users |
| `exit` | 🚪 Exit application | All users |
| `logout` | 🔐 End current session | All users |

### 👩‍🎓 Student Operations
| Command | Function | Admin | Teacher | Student |
|---------|----------|-------|---------|---------|
| `add student` | ➕ Create student record | ✅ | ✅ | ❌ |
| `delete student` | ❌ Remove student | ✅ | ✅ | ❌ |
| `edit student` | ✏️ Update student info | ✅ | ✅ | ✅ |
| `show student` | 👁️ View student list | ✅ | ✅ | ✅ |

### 👨‍🏫 Teacher Operations
| Command | Function | Admin | Teacher | Student |
|---------|----------|-------|---------|---------|
| `add teacher` | ➕ Create teacher account | ✅ | ❌ | ❌ |
| `delete teacher` | ❌ Remove teacher | ✅ | ❌ | ❌ |
| `edit teacher` | ✏️ Update teacher info | ✅ | ✅ | ❌ |
| `show teacher` | 👁️ View teacher list | ✅ | ✅ | ✅ |

### 📚 Course Operations
| Command | Function | Admin | Teacher | Student |
|---------|----------|-------|---------|---------|
| `add course` | ➕ Create new course | ✅ | ✅ | ❌ |
| `delete course` | ❌ Remove course | ✅ | ✅ | ❌ |
| `edit course` | ✏️ Update course info | ✅ | ✅ | ❌ |
| `show course` | 👁️ View course catalog | ✅ | ✅ | ✅ |

### 📝 Enrollment & Grades
| Command | Function | Admin | Teacher | Student |
|---------|----------|-------|---------|---------|
| `enroll course` | 📚 Enroll in course | ✅ | ✅ | ✅ |
| `disenroll course` | 📤 Leave course | ✅ | ✅ | ✅ |
| `show enroll-course` | 👁️ View enrollments | ✅ | ✅ | ✅ |
| `edit mark` | ✏️ Assign/update grades | ✅ | ✅ | ❌ |
| `show report` | 📊 Generate reports | ✅ | ✅ | ✅ |

### 🛡️ Administrative Operations
| Command | Function | Admin | Teacher | Student |
|---------|----------|-------|---------|---------|
| `add admin` | ➕ Create admin account | ✅ | ❌ | ❌ |
| `assign teacher` | 👨‍🏫 Assign teacher to course | ✅ | ❌ | ❌ |
| `disassign teacher` | 👨‍🏫 Remove teacher assignment | ✅ | ❌ | ❌ |

---

## ⚠️ Important Notes

> **Security**: All commands are case-sensitive and require proper authentication.

> **Permissions**: Users can only execute commands permitted for their role level.

> **Data Integrity**: All changes are automatically saved to persistent storage.
