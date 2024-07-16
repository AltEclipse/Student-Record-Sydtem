#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for student information
typedef struct {
    char name[50];
    int rollNumber;
    float marks;
} Student;

// Function to greet the user
void greetUser() {
    char name[50];
    printf("Welcome to the Student Record System!\n");
    printf("Please enter your name: ");
    scanf("%49s", name);  // Limit input to avoid buffer overflow
    printf("Hello, %s!\n", name);
}

// Function to input student marks
void inputStudentMarks(Student *student) {
    printf("Enter student name: ");
    scanf("%49s", student->name);
    printf("Enter roll number: ");
    scanf("%d", &student->rollNumber);
    printf("Enter marks: ");
    scanf("%f", &student->marks);
}

// Function to display student result
void displayStudentResult(const Student *student) {
    printf("Name: %s, Roll Number: %d, Marks: %.2f - %s\n",
           student->name, student->rollNumber, student->marks,
           (student->marks >= 40) ? "Passed" : "Failed");
}

// Function to save students to a file
void saveStudentsToFile(Student *students, int count) {
    FILE *file = fopen("students.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d %.2f\n", students[i].name, students[i].rollNumber, students[i].marks);
    }
    fclose(file);
}

// Function to load students from a file
void loadStudentsFromFile(Student **students, int *count) {
    FILE *file = fopen("students.txt", "r");
    if (file == NULL) {
        // If the file does not exist, initialize count to 0 and return
        *count = 0;
        return;
    }
    *count = 0;
    while (!feof(file)) {
        *students = realloc(*students, (*count + 1) * sizeof(Student));
        if (*students == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        fscanf(file, "%49s %d %f", (*students)[*count].name, &(*students)[*count].rollNumber, &(*students)[*count].marks);
        (*count)++;
    }
    fclose(file);
}

// Function to search for a student by name and roll number
void searchStudent(Student *students, int count) {
    char name[50];
    int rollNumber;
    printf("Enter student name to search: ");
    scanf("%49s", name);
    printf("Enter roll number to search: ");
    scanf("%d", &rollNumber);

    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].name, name) == 0 && students[i].rollNumber == rollNumber) {
            displayStudentResult(&students[i]);
            return;
        }
    }
    printf("Student with name %s and roll number %d not found.\n", name, rollNumber);
}

// Function to calculate the average marks
void calculateAverageMarks(Student *students, int count) {
    if (count == 0) {
        printf("No students to calculate average.\n");
        return;
    }
    float sum = 0;
    for (int i = 0; i < count; i++) {
        sum += students[i].marks;
    }
    printf("Average marks: %.2f\n", sum / count);
}

// Function to compare students by marks for sorting
int compareStudentsByMarks(const void *a, const void *b) {
    float marksA = ((Student *)a)->marks;
    float marksB = ((Student *)b)->marks;
    return (marksA > marksB) - (marksA < marksB);
}

// Function to sort students by marks
void sortStudentsByMarks(Student *students, int count, int order) {
    qsort(students, count, sizeof(Student), compareStudentsByMarks);
    if (order == 1) {
        printf("Students sorted in ascending order.\n");
    } else {
        printf("Students sorted in descending order.\n");
        for (int i = 0; i < count / 2; i++) {
            Student temp = students[i];
            students[i] = students[count - i - 1];
            students[count - i - 1] = temp;
        }
    }
}

// Function to delete a student by name and roll number
void deleteStudent(Student **students, int *count) {
    char name[50];
    int rollNumber;
    printf("Enter student name to delete: ");
    scanf("%49s", name);
    printf("Enter roll number to delete: ");
    scanf("%d", &rollNumber);

    for (int i = 0; i < *count; i++) {
        if (strcmp((*students)[i].name, name) == 0 && (*students)[i].rollNumber == rollNumber) {
            // Shift remaining students to fill the gap
            for (int j = i; j < *count - 1; j++) {
                (*students)[j] = (*students)[j + 1];
            }
            // Resize the memory block
            *students = realloc(*students, (*count - 1) * sizeof(Student));
            if (*students == NULL && *count > 1) {
                printf("Memory allocation failed!\n");
                exit(1);
            }
            (*count)--;
            printf("Student with name %s and roll number %d deleted.\n", name, rollNumber);
            return;
        }
    }
    printf("Student with name %s and roll number %d not found.\n", name, rollNumber);
}

// Function to manage student records
void manageStudents() {
    int choice;
    Student *students = NULL;
    int count = 0;

    loadStudentsFromFile(&students, &count);

    do {
        printf("1. Add Student\n2. Display Students\n3. Search Student by Name and Roll Number\n4. Calculate Average Marks\n5. Sort Students by Marks\n6. Delete Student by Name and Roll Number\n7. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                students = realloc(students, (count + 1) * sizeof(Student));
                if (students == NULL) {
                    printf("Memory allocation failed!\n");
                    exit(1);
                }
                inputStudentMarks(&students[count]);
                count++;
                break;
            case 2:
                for (int i = 0; i < count; i++) {
                    displayStudentResult(&students[i]);
                }
                break;
            case 3:
                searchStudent(students, count);
                break;
            case 4:
                calculateAverageMarks(students, count);
                break;
            case 5:
                printf("Enter 1 for ascending order or 2 for descending order: ");
                int order;
                scanf("%d", &order);
                sortStudentsByMarks(students, count, order);
                break;
            case 6:
                deleteStudent(&students, &count);
                break;
            case 7:
                saveStudentsToFile(students, count);
                printf("Students saved. Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);

    free(students);
}

int main() {
    greetUser();
    manageStudents();
    return 0;
}
