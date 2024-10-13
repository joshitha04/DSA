#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100
#define STUDENT_FILE "students.txt"
#define TEACHER_FILE "teachers.txt"

// Define structures for student and teacher
typedef struct Student {
    int studentID;
    char name[50];
    char pass[20];
    int marks;
    int assignedTeacherID; // Track assigned teacher ID
    struct Student *next;
} student_node;

typedef struct Teacher {
    int teacherID;
    char name[50];
    char pass[20];
    struct Teacher *next;
} teacher_node;

// Global variables
student_node *studentTable[TABLE_SIZE];
teacher_node *teacherTable[TABLE_SIZE];

// Hash function for student IDs
int hashStudent(int studentID) {
    return studentID % TABLE_SIZE;
}

// Hash function for teacher IDs
int hashTeacher(int teacherID) {
    return teacherID % TABLE_SIZE;
}

// Function to load students from the file
void loadStudents() {
    FILE *file = fopen(STUDENT_FILE, "r");
    if (!file) return;

    student_node *newStudent;
    while (!feof(file)) {
        newStudent = (student_node *)malloc(sizeof(student_node));
        fscanf(file, "%d %s %s %d %d\n",
               &newStudent->studentID,
               newStudent->name,
               newStudent->pass,
               &newStudent->marks,
               &newStudent->assignedTeacherID);
        newStudent->next = NULL;

        int index = hashStudent(newStudent->studentID);
        if (studentTable[index] == NULL) {
            studentTable[index] = newStudent;
        } else {
            student_node *current = studentTable[index];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newStudent;
        }
    }
    fclose(file);
}

// Function to load teachers from the file
void loadTeachers() {
    FILE *file = fopen(TEACHER_FILE, "r");
    if (!file) return;

    teacher_node *newTeacher;
    while (!feof(file)) {
        newTeacher = (teacher_node *)malloc(sizeof(teacher_node));
        fscanf(file, "%d %s %s\n", 
               &newTeacher->teacherID,
               newTeacher->name,
               newTeacher->pass);
        newTeacher->next = NULL;

        int index = hashTeacher(newTeacher->teacherID);
        if (teacherTable[index] == NULL) {
            teacherTable[index] = newTeacher;
        } else {
            teacher_node *current = teacherTable[index];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newTeacher;
        }
    }
    fclose(file);
}

// Function to save students to the file
void saveStudents() {
    FILE *file = fopen(STUDENT_FILE, "w");
    for (int i = 0; i < TABLE_SIZE; i++) {
        student_node *current = studentTable[i];
        while (current != NULL) {
            fprintf(file, "%d %s %s %d %d\n", 
                    current->studentID,
                    current->name,
                    current->pass,
                    current->marks,
                    current->assignedTeacherID);
            current = current->next;
        }
    }
    fclose(file);
}

// Function to save teachers to the file
void saveTeachers() {
    FILE *file = fopen(TEACHER_FILE, "w");
    for (int i = 0; i < TABLE_SIZE; i++) {
        teacher_node *current = teacherTable[i];
        while (current != NULL) {
            fprintf(file, "%d %s %s\n", 
                    current->teacherID,
                    current->name,
                    current->pass);
            current = current->next;
        }
    }
    fclose(file);
}

// Function to register a student
void registerStudent() {
    student_node *newStudent = (student_node *)malloc(sizeof(student_node));
    printf("Enter Student ID: ");
    scanf("%d", &newStudent->studentID);
    printf("Enter Name: ");
    scanf("%s", newStudent->name);
    printf("Enter Password: ");
    scanf("%s", newStudent->pass);
    newStudent->marks = 0; // Initialize marks to 0
    printf("Enter Assigned Teacher ID: ");
    scanf("%d", &newStudent->assignedTeacherID);
    newStudent->next = NULL;

    int index = hashStudent(newStudent->studentID);
    if (studentTable[index] == NULL) {
        studentTable[index] = newStudent;
    } else {
        student_node *current = studentTable[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newStudent;
    }
    saveStudents(); // Save to file after registration
    printf("Student registered successfully!\n");
}

// Function to register a teacher
void registerTeacher() {
    teacher_node *newTeacher = (teacher_node *)malloc(sizeof(teacher_node));
    printf("Enter Teacher ID: ");
    scanf("%d", &newTeacher->teacherID);
    printf("Enter Name: ");
    scanf("%s", newTeacher->name);
    printf("Enter Password: ");
    scanf("%s", newTeacher->pass);
    newTeacher->next = NULL;

    int index = hashTeacher(newTeacher->teacherID);
    if (teacherTable[index] == NULL) {
        teacherTable[index] = newTeacher;
    } else {
        teacher_node *current = teacherTable[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newTeacher;
    }
    saveTeachers(); // Save to file after registration
    printf("Teacher registered successfully!\n");
}

// Function for student login
void studentLogin() {
    int studentID;
    char pass[20];

    printf("Enter Student ID: ");
    scanf("%d", &studentID);
    
    printf("Enter Password: ");
    scanf("%s", pass);

    int index = hashStudent(studentID);
    student_node *currentStudent = studentTable[index];

    // Search for the student
    while (currentStudent != NULL) {
        if (currentStudent->studentID == studentID && strcmp(currentStudent->pass, pass) == 0) {
            printf("Login successful!\n");
            viewStudentStatistics(currentStudent); // Call to view marks or statistics
            return;
        }
        currentStudent = currentStudent->next;
    }
    printf("Invalid Student ID or Password.\n");
}

// Function to view student statistics
void viewStudentStatistics(student_node *student) {
    printf("Name: %s\n", student->name);
    printf("Marks: %d\n", student->marks);
    printf("Assigned Teacher ID: %d\n", student->assignedTeacherID);
}

// Function for teacher login
void teacherLogin() {
    int teacherID;
    char pass[20];

    printf("Enter Teacher ID: ");
    scanf("%d", &teacherID);

    printf("Enter Password: ");
    scanf("%s", pass);

    int index = hashTeacher(teacherID);
    teacher_node *currentTeacher = teacherTable[index];

    // Search for the teacher
    while (currentTeacher != NULL) {
        if (currentTeacher->teacherID == teacherID && strcmp(currentTeacher->pass, pass) == 0) {
            printf("Login successful! Teacher: %s\n", currentTeacher->name);
            int choice;
            while (1) {
                printf("\n1. Assign Marks\n2. View Assigned Students\n3. View Statistics of Assigned Students\n4. Exit\n");
                printf("Choose an option: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        modifyStudentMarks(teacherID);
                        break;
                    case 2:
                        showStudents(teacherID);
                        break;
                    case 3:
                        showStatistics(teacherID);
                        break;
                    case 4:
                        printf("Exiting teacher dashboard.\n");
                        return;
                    default:
                        printf("Invalid choice, please try again.\n");
                }
            }
        }
        currentTeacher = currentTeacher->next;
    }
    printf("Invalid Teacher ID or Password.\n");
}

// Function to show students assigned to a teacher
void showStudents(int teacherID) {
    int found = 0;
    printf("Students assigned to you:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        student_node *currentStudent = studentTable[i];
        while (currentStudent != NULL) {
            if (currentStudent->assignedTeacherID == teacherID) {
                printf("Student ID: %d, Name: %s\n", currentStudent->studentID, currentStudent->name);
                found = 1;
            }
            currentStudent = currentStudent->next;
        }
    }
    if (!found) {
        printf("No students assigned to you.\n");
    }
}

// Function to modify student marks by teacher
void modifyStudentMarks(int teacherID) {
    int studentID;
    int newMarks;

    printf("Enter Student ID to assign marks: ");
    scanf("%d", &studentID);
    
    int index = hashStudent(studentID);
    student_node *currentStudent = studentTable[index];

    // Search for the student
    while (currentStudent != NULL) {
        if (currentStudent->studentID == studentID && currentStudent->assignedTeacherID == teacherID) {
          
            printf("Enter new marks for student %s: ", currentStudent->name);
            scanf("%d", &newMarks);
            currentStudent->marks = newMarks; // Assign new marks
            saveStudents(); // Save updated marks to the file
            printf("Marks assigned successfully!\n");
            return;
        }
        currentStudent = currentStudent->next;
    }
    printf("Student not found or not assigned to you.\n");
}

// Function to show statistics of assigned students
void showStatistics(int teacherID) {
    int totalMarks = 0;
    int count = 0;
    int minMarks = 101; // Assuming marks are in the range 0-100
    int maxMarks = -1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        student_node *currentStudent = studentTable[i];
        while (currentStudent != NULL) {
            if (currentStudent->assignedTeacherID == teacherID) {
                totalMarks += currentStudent->marks;
                count++;
                if (currentStudent->marks < minMarks) {
                    minMarks = currentStudent->marks;
                }
                if (currentStudent->marks > maxMarks) {
                    maxMarks = currentStudent->marks;
                }
            }
            currentStudent = currentStudent->next;
        }
    }

    if (count > 0) {
        float average = (float)totalMarks / count;
        printf("Total Students: %d\n", count);
        printf("Average Marks: %.2f\n", average);
        printf("Minimum Marks: %d\n", minMarks);
        printf("Maximum Marks: %d\n", maxMarks);
    } else {
        printf("No students assigned to you.\n");
    }
}

// Main function
int main() {
    loadStudents(); // Load students from file
    loadTeachers(); // Load teachers from file

    int choice;
    while (1) {
        printf("\n1. Register Student\n2. Register Teacher\n3. Student Login\n4. Teacher Login\n5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerStudent();
                break;
            case 2:
                registerTeacher();
                break;
            case 3:
                studentLogin();
                break;
            case 4:
                teacherLogin();
                break;
            case 5:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice, please try again.\n");
        }
    }

    return 0;
}
