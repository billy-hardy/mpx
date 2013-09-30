#include "r1.h"

int history(int argc, char *argv[]) {
  FILE *fptr;
  char *buffer;
  int bufferSize;
  if(argc != 1) {
    //TODO: print error
  } else {
    fptr = fopen("history.txt", "r");
	fseek(fptr, 0L, SEEK_END);
	bufferSize = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);
    
	buffer = (char *)sys_alloc_mem(bufferSize+1);
	fread(buffer, bufferSize, 1, fptr);
	
    sys_req(WRITE, TERMINAL, buffer, &bufferSize);
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
