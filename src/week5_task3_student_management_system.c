#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_SIZE 50
#define LINE_SIZE 256
#define DATA_FILE "students.txt"

typedef struct {
  char name[NAME_SIZE];
  int id;
  float gpa;
} Student;

int read_line(char* buffer, size_t size) {
  if (fgets(buffer, (int)size, stdin) == NULL) {
    return 0;
  }

  size_t length = strlen(buffer);

  if (length > 0 && buffer[length - 1] == '\n') {
    buffer[length - 1] = '\0';
  } else {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
  }

  return 1;
}

int load_students(Student arr[], int max) {
  FILE* file = fopen(DATA_FILE, "r");

  if (file == NULL) {
    return 0;
  }

  char line[LINE_SIZE];
  int count = 0;

  while (count < max && fgets(line, sizeof(line), file) != NULL) {
    Student student;
    char extra;

    if (strspn(line, " \t\r\n") == strlen(line)) {
      continue;
    }

    if (sscanf(line, "%49[^,],%d,%f %c", student.name, &student.id,
               &student.gpa, &extra) == 3) {
      if (student.name[0] != '\0' && student.gpa >= 0.0f &&
          student.gpa <= 4.0f) {
        arr[count++] = student;
      }
    }
  }

  fclose(file);
  return count;
}

int save_students(const Student arr[], int count) {
  FILE* file = fopen(DATA_FILE, "w");

  if (file == NULL) {
    return -1;
  }

  for (int i = 0; i < count; i++) {
    if (fprintf(file, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa) < 0) {
      fclose(file);
      return -1;
    }
  }

  if (fclose(file) != 0) {
    return -1;
  }

  return 0;
}

void add_student(Student arr[], int* count) {
  if (*count >= MAX_STUDENTS) {
    printf("Cannot add more students (limit reached).\n");
    return;
  }

  char name[LINE_SIZE];
  char id_text[LINE_SIZE];
  char gpa_text[LINE_SIZE];
  char* end;
  long id;
  float gpa;

  printf("Enter name: ");
  if (!read_line(name, sizeof(name))) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  printf("Enter ID: ");
  if (!read_line(id_text, sizeof(id_text))) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  printf("Enter GPA: ");
  if (!read_line(gpa_text, sizeof(gpa_text))) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  if (name[0] == '\0' || strchr(name, ',') != NULL ||
      strlen(name) >= NAME_SIZE) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  char* id_start = id_text;
  while (isspace((unsigned char)*id_start)) {
    id_start++;
  }

  if (*id_start == '\0') {
    printf("Invalid input. Student not added.\n");
    return;
  }

  id = strtol(id_start, &end, 10);
  while (isspace((unsigned char)*end)) {
    end++;
  }

  if (*end != '\0' || end == id_start || id < 0 || id > 2147483647L) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  char* gpa_start = gpa_text;
  while (isspace((unsigned char)*gpa_start)) {
    gpa_start++;
  }

  if (*gpa_start == '\0') {
    printf("Invalid input. Student not added.\n");
    return;
  }

  gpa = strtof(gpa_start, &end);
  while (isspace((unsigned char)*end)) {
    end++;
  }

  if (end == gpa_start || *end != '\0' || gpa < 0.0f || gpa > 4.0f) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  snprintf(arr[*count].name, NAME_SIZE, "%s", name);
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

int main(void) {
  Student students[MAX_STUDENTS];
  int count = load_students(students, MAX_STUDENTS);

  printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

  char choice_text[LINE_SIZE];

  while (1) {
    printf("\n=== Student Management System ===\n");
    printf("1. List students\n");
    printf("2. Add student\n");
    printf("3. Save and Exit\n");
    printf("Select an option: ");

    if (!read_line(choice_text, sizeof(choice_text))) {
      if (save_students(students, count) != 0) {
        fprintf(stderr, "Error saving student data.\n");
        return 1;
      }

      printf("Data saved. Goodbye!\n");
      break;
    }

    if (strcmp(choice_text, "1") == 0) {
      list_students(students, count);
    } else if (strcmp(choice_text, "2") == 0) {
      add_student(students, &count);
    } else if (strcmp(choice_text, "3") == 0) {
      if (save_students(students, count) != 0) {
        fprintf(stderr, "Error saving student data.\n");
        return 1;
      }

      printf("Data saved. Goodbye!\n");
      break;
    } else {
      printf("Invalid option. Please try again.\n");
    }
  }

  return 0;
}