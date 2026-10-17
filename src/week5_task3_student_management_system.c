// week5_task3_student_management_system.c
// Name: Aghabala Surname: Jabiyev Student ID: 251ADB102
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
// TODO: Implement the functions to load, save, add, and list students.
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
  char name[NAME_LEN];
  int id;
  float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls
// them directly)
int load_students(Student arr[], int max);  // returns number of records loaded
int save_students(const Student arr[], int count);  // 0 on success, -1 on error
void add_student(Student arr[], int* count);
void list_students(const Student arr[], int count);
int read_line(const char* prompt, char* buf, size_t size);

int main(void) {
  Student students[MAX_STUDENTS];
  int count = 0;
  char input[LINE_LEN];
  int running = 1;

  count = load_students(students, MAX_STUDENTS);
  printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

  while (running) {
    printf("\n=== Student Management System ===\n");
    printf("1. List students\n");
    printf("2. Add student\n");
    printf("3. Save and Exit\n");

    int choice;
    if (read_line("Select an option: ", input, sizeof input) != 0) {
      choice = 3;  // end of input: save and exit instead of looping forever
    } else {
      char* end;
      long value = strtol(input, &end, 10);
      choice = (end != input && *end == '\0')
                   ? (int)value
                   : 0;  // non-numbers become invalid
    }

    switch (choice) {
      case 1:
        list_students(students, count);
        break;
      case 2:
        add_student(students, &count);
        break;
      case 3:
        if (save_students(students, count) != 0) {
          fprintf(stderr, "Error saving student data.\n");
          return EXIT_FAILURE;
        }
        printf("Data saved. Goodbye!\n");
        running = 0;
        break;
      default:
        printf("Invalid option. Try again.\n");
    }
  }

  return EXIT_SUCCESS;
}

// Provided helper – prints a prompt and reads one line from stdin into buf.
// The trailing newline is removed. Returns 0 on success, -1 on end of input
// (EOF).
int read_line(const char* prompt, char* buf, size_t size) {
  printf("%s", prompt);
  fflush(stdout);
  if (fgets(buf, (int)size, stdin) == NULL) {
    return -1;
  }
  size_t len = strcspn(buf, "\r\n");
  if (buf[len] == '\0' && len == size - 1) {
    // Line was longer than the buffer: discard the rest of it.
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
  }
  buf[len] = '\0';
  return 0;
}

// TODO: Implement load_students()
// Open DATA_FILE for reading. If it cannot be opened (e.g. first run), return
// 0. Read the file line by line with fgets(); parse "name,id,gpa" with
// sscanf(line, "%49[^,],%d,%f", ...) and keep only lines where sscanf
// returned 3. Stop when max records have been loaded. Close the file and return
// the number loaded.
int load_students(Student arr[], int max) {
  FILE* fp = fopen(DATA_FILE, "r");
  if (fp == NULL) {
    return 0;
  }

  char line[LINE_LEN];
  int count = 0;

  while (count < max && fgets(line, sizeof(line), fp) != NULL) {
    Student student;

    if (sscanf(line, "%49[^,],%d,%f", student.name, &student.id,
               &student.gpa) == 3) {
      if (student.name[0] != '\0' && student.gpa >= 0.0f &&
          student.gpa <= 4.0f) {
        arr[count] = student;
        count++;
      }
    }
  }

  fclose(fp);
  return count;
}

// TODO: Implement save_students()
// Open DATA_FILE for writing ("w"), return -1 if fopen() fails.
// Write one line per student: fprintf(fp, "%s,%d,%.2f\n", ...). Close the
// file and return 0.
int save_students(const Student arr[], int count) {
  FILE* fp = fopen(DATA_FILE, "w");
  if (fp == NULL) {
    return -1;
  }

  for (int i = 0; i < count; i++) {
    if (fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa) < 0) {
      fclose(fp);
      return -1;
    }
  }

  if (fclose(fp) != 0) {
    return -1;
  }

  return 0;
}

// TODO: Implement add_student()
// 1. If *count >= MAX_STUDENTS, print "Cannot add more students (limit
// reached)." and return.
// 2. Use read_line() to ask "Enter name: ", "Enter ID: " and "Enter GPA: ".
//    If read_line() returns -1 (end of input), just return.
// 3. Validate: name not empty, no ',' in name, ID is an integer, GPA is a
// number in 0.0–4.0
//    (strtol / strtof are useful). Otherwise print "Invalid input. Student
//    not added." and return.
// 4. Store the student at arr[*count], increase *count, print "Student added
// successfully!"
void add_student(Student arr[], int* count) {
  if (*count >= MAX_STUDENTS) {
    printf("Cannot add more students (limit reached).\n");
    return;
  }

  char name[NAME_LEN];
  char id_input[LINE_LEN];
  char gpa_input[LINE_LEN];
  char* end;
  long id;
  float gpa;

  if (read_line("Enter name: ", name, sizeof(name)) != 0) {
    return;
  }
  if (read_line("Enter ID: ", id_input, sizeof(id_input)) != 0) {
    return;
  }
  if (read_line("Enter GPA: ", gpa_input, sizeof(gpa_input)) != 0) {
    return;
  }

  if (name[0] == '\0' || strchr(name, ',') != NULL) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  id = strtol(id_input, &end, 10);
  if (end == id_input || *end != '\0' || id < 0 || id > 2147483647L) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  gpa = strtof(gpa_input, &end);
  if (end == gpa_input || *end != '\0' || !(gpa >= 0.0f && gpa <= 4.0f)) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  snprintf(arr[*count].name, NAME_LEN, "%s", name);
  arr[*count].id = (int)id;
  arr[*count].gpa = gpa;
  (*count)++;

  printf("Student added successfully!\n");
}

void list_students(const Student arr[], int count) {
  if (count == 0) {
    printf("No students found.\n");
    return;
  }

  printf("%-5s %-20s %s\n", "ID", "Name", "GPA");

  for (int i = 0; i < count; i++) {
    printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
  }
}