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
    printf("Enter Marks: ");
    scanf("%d", &newStudent->marks);
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

// Function to calculate average
double calculateAverage(int marks[], int count) {
    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += marks[i];
    }
    return sum / count;
}

// Function to compare two integers for qsort
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Function to calculate median
double calculateMedian(int marks[], int count) {
    if (count == 0) return 0;
    // Sort the marks for median calculation
    qsort(marks, count, sizeof(int), compare);
    if (count % 2 == 0) {
        return (marks[count / 2 - 1] + marks[count / 2]) / 2.0;
    } else {
        return marks[count / 2];
    }
}

// Function to calculate Q1 and Q3
void calculateQuartiles(int marks[], int count, double *Q1, double *Q3) {
    qsort(marks, count, sizeof(int), compare);
    if (count % 2 == 0) {
        // Even number of elements
        int mid = count / 2;
        *Q1 = calculateMedian(marks, mid);
        *Q3 = calculateMedian(marks + mid, mid);
    } else {
        // Odd number of elements
        int mid = count / 2;
        *Q1 = calculateMedian(marks, mid);
        *Q3 = calculateMedian(marks + mid + 1, mid);
    }
}

// Function to view student statistics
void viewStudentStatistics(student_node *student) {
    int choice;
    int marks[100]; // Array to hold marks of students assigned to the teacher
    int count = 0;

    // Collect marks of students assigned to the teacher
    for (int i = 0; i < TABLE_SIZE; i++) {
        student_node *currentStudent = studentTable[i];
        while (currentStudent != NULL) {
            if (currentStudent->assignedTeacherID == student->assignedTeacherID) {
                marks[count++] = currentStudent->marks;
            }
            currentStudent = currentStudent->next;
        }
    }

    while (1) {
        printf("\n1. View Marks\n2. View Statistics (Average, Median, Q1, Q3)\n3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Name: %s\n", student->name);
                printf("Marks: %d\n", student->marks);
                break;
            case 2:
                {
                    double average = calculateAverage(marks, count);
                    double median = calculateMedian(marks, count);
                    double Q1, Q3;
                    calculateQuartiles(marks, count, &Q1, &Q3);

                    printf("Statistics for students assigned to Teacher ID %d:\n", student->assignedTeacherID);
                    printf("Average: %.2f\n", average);
                    printf("Median: %.2f\n", median);
                    printf("Q1: %.2f\n", Q1);
                    printf("Q3: %.2f\n", Q3);
                }
                break;
            case 3:
                return;
            default:
                printf("Invalid option, try again.\n");
                break;
        }
    }
}

// Main function
int main() {
    loadStudents();
    loadTeachers();

    int choice;
    while (1) {
        printf("\n1. Register Student\n2. Register Teacher\n3. Student Login\n4. Exit\n");
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
                exit(0);
            default:
                printf("Invalid option, try again.\n");
        }
    }

    return 0;
}
