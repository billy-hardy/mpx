#include "r1.h"

int history(int argc, char *argv[]) {
  FILE *fptr;
  if(argc != 1) {
    //TODO: print error
  } else {
    fptr = fopen("history.txt", "r");
    paginate(fread(fptr));
  }
  return LOOP;
}

void printCommandToFile(char command[]) {
  FILE *fptr;
  fptr = fopen("history.txt", "a");
  fprintf(fptr, command);
  fclose(fptr);
}

void cleanUpHistory() {
  remove("history.txt");
}
