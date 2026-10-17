#include <stdio.h>

int main(int argc, char* argv[]) {
  const char* filename = (argc > 1) ? argv[1] : "data.txt";
  FILE* file = fopen(filename, "w");

  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  printf("Writing lines to %s...\n", filename);

  fprintf(file, "Hello, file I/O in C!\n");
  fprintf(file, "This is another line.\n");

  if (fclose(file) != 0) {
    perror("fclose");
    return 1;
  }

  file = fopen(filename, "r");

  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  printf("Reading contents:\n");

  char line[256];
  int lines_read = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    printf("%s", line);
    lines_read++;
  }

  if (ferror(file)) {
    perror("fgets");
    fclose(file);
    return 1;
  }

  if (fclose(file) != 0) {
    perror("fclose");
    return 1;
  }

  printf("Lines read: %d\n", lines_read);

  return 0;
}