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
    fgets(name, sizeof(name), stdin);  // Use fgets for better input handling
    name[strcspn(name, "\n")] = '\0';  // Remove trailing newline
    printf("Hello, %s!\n", name);
}

// Function to input student marks
void inputStudentMarks(Student *student) {
    printf("Enter student name: ");
    fgets(student->name, sizeof(student->name), stdin);  // Use fgets for better input handling
    student->name[strcspn(student->name, "\n")] = '\0';  // Remove trailing newline
    printf("Enter roll number: ");
    scanf("%d", &student->rollNumber);
    printf("Enter marks: ");
    scanf("%f", &student->marks);
    getchar();  // Consume leftover newline character
}

// Function to input multiple students
void inputMultipleStudents(Student **students, int *count) {
    int numStudents;
    printf("Enter the number of students to add: ");
    scanf("%d", &numStudents);
    getchar();  // Consume leftover newline character

    *students = realloc(*students, (*count + numStudents) * sizeof(Student));
    if (*students == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < numStudents; i++) {
        printf("Entering details for student %d:\n", *count + 1);
        inputStudentMarks(&(*students)[*count]);
        (*count)++;
    }
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
        fprintf(file, "%s\n%d\n%.2f\n", students[i].name, students[i].rollNumber, students[i].marks);
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
    char buffer[50];
    while (fgets(buffer, sizeof(buffer), file)) {
        *students = realloc(*students, (*count + 1) * sizeof(Student));
        if (*students == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove trailing newline
        strcpy((*students)[*count].name, buffer);
        fscanf(file, "%d\n%f\n", &(*students)[*count].rollNumber, &(*students)[*count].marks);
        fgets(buffer, sizeof(buffer), file);  // Consume the leftover newline
        (*count)++;
    }
    fclose(file);
}

// Function to search for a student by name
void searchStudentByName(Student *students, int count) {
    char name[50];
    printf("Enter student name to search: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';  // Remove trailing newline

    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].name, name) == 0) {
            displayStudentResult(&students[i]);
            return;
        }
    }
    printf("Student with name %s not found.\n", name);
}

// Function to search for a student by roll number
void searchStudentByRollNumber(Student *students, int count) {
    int rollNumber;
    printf("Enter roll number to search: ");
    scanf("%d", &rollNumber);
    getchar();  // Consume leftover newline

    for (int i = 0; i < count; i++) {
        if (students[i].rollNumber == rollNumber) {
            displayStudentResult(&students[i]);
            return;
        }
    }
    printf("Student with roll number %d not found.\n", rollNumber);
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

// Function to delete a student by name
void deleteStudentByName(Student **students, int *count) {
    char name[50];
    printf("Enter student name to delete: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';  // Remove trailing newline

    for (int i = 0; i < *count; i++) {
        if (strcmp((*students)[i].name, name) == 0) {
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
            printf("Student with name %s deleted.\n", name);
            return;
        }
    }
    printf("Student with name %s not found.\n", name);
}

// Function to delete a student by roll number
void deleteStudentByRollNumber(Student **students, int *count) {
    int rollNumber;
    printf("Enter roll number to delete: ");
    scanf("%d", &rollNumber);
    getchar();  // Consume leftover newline

    for (int i = 0; i < *count; i++) {
        if ((*students)[i].rollNumber == rollNumber) {
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
            printf("Student with roll number %d deleted.\n", rollNumber);
            return;
        }
    }
    printf("Student with roll number %d not found.\n", rollNumber);
}

// Function to manage student records
void manageStudents() {
    int choice;
    Student *students = NULL;
    int count = 0;

    loadStudentsFromFile(&students, &count);

    do {
        printf("1. Add Student\n2. Add Multiple Students\n3. Display Students\n4. Search Student by Name\n5. Search Student by Roll Number\n6. Calculate Average Marks\n7. Sort Students by Marks\n8. Delete Student by Name\n9. Delete Student by Roll Number\n10. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Consume leftover newline

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
                inputMultipleStudents(&students, &count);
                break;
            case 3:
                for (int i = 0; i < count; i++) {
                    displayStudentResult(&students[i]);
                }
                break;
            case 4:
                searchStudentByName(students, count);
                break;
            case 5:
                searchStudentByRollNumber(students, count);
                break;
            case 6:
                calculateAverageMarks(students, count);
                break;
            case 7:
                printf("Enter 1 for ascending order or 2 for descending order: ");
                int order;
                scanf("%d", &order);
                getchar();  // Consume leftover newline
                sortStudentsByMarks(students, count, order);
                break;
            case 8:
                deleteStudentByName(&students, &count);
                break;
            case 9:
                deleteStudentByRollNumber(&students, &count);
                break;
            case 10:
                saveStudentsToFile(students, count);
                printf("Students saved. Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 10);

    free(students);
}

int main() {
    greetUser();
    manageStudents();
    return 0;
}

