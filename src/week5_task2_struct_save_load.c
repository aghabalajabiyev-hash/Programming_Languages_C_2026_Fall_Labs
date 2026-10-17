#include <stdio.h>

typedef struct Student {
  char name[50];
  int age;
  float gpa;
} Student;

int save_student(const Student* s, const char* filename) {
  FILE* file = fopen(filename, "w");

  if (file == NULL) {
    return -1;
  }

  if (fprintf(file, "%s %d %.2f\n", s->name, s->age, s->gpa) < 0) {
    fclose(file);
    return -1;
  }

  if (fclose(file) != 0) {
    return -1;
  }

  return 0;
}

int load_student(const char* filename, Student* out) {
  FILE* file = fopen(filename, "r");

  if (file == NULL) {
    return -1;
  }

  int fields_read = fscanf(file, "%49s %d %f", out->name, &out->age, &out->gpa);

  if (fclose(file) != 0) {
    return -1;
  }

  if (fields_read != 3) {
    return -1;
  }

  return 0;
}

int main(void) {
  Student student = {"Alice", 21, 3.75f};
  Student loaded_student;

  printf("Saving student to file...\n");

  if (save_student(&student, "student.txt") != 0) {
    fprintf(stderr, "Error saving student.\n");
    return 1;
  }

  printf("Loading student from file...\n");

  if (load_student("student.txt", &loaded_student) != 0) {
    fprintf(stderr, "Error loading student.\n");
    return 1;
  }

  printf("Loaded student: %s, %d, GPA %.2f\n", loaded_student.name,
         loaded_student.age, loaded_student.gpa);

  return 0;
}