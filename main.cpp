#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream> 
#include <algorithm> 

class Subject {
public:
    std::string name;
    int marks;
};

class Student {
public:
    std::string name;
    std::vector<Subject> subjects;
    int totalMarks;
};

class Class {
public:
    std::string name;
    std::vector<Student> students;
};


void writeStudentDataToFile(const std::vector<Class>& classes) {
    std::ofstream outputFile("student_marks.txt");
    if (outputFile.is_open()) {
        for (const auto& cls : classes) {
            for (const auto& student : cls.students) {
                outputFile << cls.name << "," << student.name;
                for (const auto& subject : student.subjects) {
                    outputFile << "," << subject.name << "," << subject.marks;
                }
                outputFile << "\n";
            }
        }
        outputFile.close();
    } else {
        std::cerr << "Error opening the file to write data.\n";
    }
}

void readStudentDataFromFile(std::vector<Class>& classes) {
    std::ifstream inputFile("student_marks.txt");
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::stringstream ss(line);
            std::string className, studentName, subjectName;
            int marks;
            std::getline(ss, className, ',');
            std::getline(ss, studentName, ',');
            Class* cls = nullptr;
            for (auto& c : classes) {
                if (c.name == className) {
                    cls = &c;
                    break;
                }
            }
            if (cls == nullptr) {
                classes.push_back(Class{ className });
                cls = &classes.back();
            }
            cls->students.push_back(Student{ studentName });
            Student& student = cls->students.back();
            while (std::getline(ss, subjectName, ',')) {
                std::getline(ss, line, ',');
                marks = std::stoi(line);
                student.subjects.push_back(Subject{ subjectName, marks });
            }
        }
        inputFile.close();
    } else {
        std::cerr << "Error opening the file to read data.\n";
    }
}

void calculateTotalMarksAndRank(std::vector<Class>& classes) {
    for (auto& cls : classes) {
        for (size_t i = 0; i < cls.students.size(); ++i) {
            int totalMarks = 0;
            for (const auto& subject : cls.students[i].subjects) {
                totalMarks += subject.marks;
            }
            cls.students[i].totalMarks = totalMarks;
        }
        std::sort(cls.students.begin(), cls.students.end(),
            [](const Student& a, const Student& b) {
                return a.totalMarks > b.totalMarks;
            }
        );

        int rank = 1;
        for (size_t i = 0; i < cls.students.size(); ++i) {
            if (i > 0 && cls.students[i].totalMarks < cls.students[i - 1].totalMarks) {
                ++rank;
            }
            cls.students[i].rank = rank;
        }
    }

    // Calculate school rank by merging all class students and re-ranking.
    std::vector<Student> schoolStudents;
    for (const auto& cls : classes) {
        schoolStudents.insert(schoolStudents.end(), cls.students.begin(), cls.students.end());
    }
    std::sort(schoolStudents.begin(), schoolStudents.end(),
        [](const Student& a, const Student& b) {
            return a.totalMarks > b.totalMarks;
        }
    );

    int rank = 1;
    for (size_t i = 0; i < schoolStudents.size(); ++i) {
        if (i > 0 && schoolStudents[i].totalMarks < schoolStudents[i - 1].totalMarks) {
            ++rank;
        }
        schoolStudents[i].rank = rank;
    }
}

void printRank(const std::vector<Class>& classes) {
    std::cout << "School Rank:\n";
    for (const auto& student : classes[0].students) {
        std::cout << "Name: " << student.name << ", Rank: " << student.rank << ", Total Marks: " << student.totalMarks << "\n";
    }

    for (const auto& cls : classes) {
        std::cout << cls.name << " Class Rank:\n";
        for (const auto& student : cls.students) {
            std::cout << "Name: " << student.name << ", Rank: " << student.rank << ", Total Marks: " << student.totalMarks << "\n";
        }
    }
}

int main() {
    std::vector<Class> classes;

    // You can add student marks to the classes here.
    // For example:
    // Class bioClass{ "Bio Stream Class 1" };
    // bioClass.students.push_back(Student{ "Student1" });
    // bioClass.students.back().subjects.push_back(Subject{ "Biology", 85 });
    // bioClass.students.back().subjects.push_back(Subject{ "Physics", 78 });
    // bioClass.students.back().subjects.push_back(Subject{ "Chemistry", 90 });
    // classes.push_back(bioClass);

    // Read student data from the file (if available).
    readStudentDataFromFile(classes);

    // Calculate total marks and rank.
    calculateTotalMarksAndRank(classes);

    // Print ranks.
    printRank(classes);

    // Write student data to the file for future use.
    writeStudentDataToFile(classes);

    return 0;
}
