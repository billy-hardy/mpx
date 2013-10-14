#include "r1.h"

int exec(int argc, char **argv) {
  int returnVal = LOOP;
  if(argc != 1) {
    int i;
    FILE *fptr;
    char buffer[129], errorMessage[128];
    int errorMessageSize;
    for(i=1; i<argc; i++) {
      fptr = fopen(argv[i], "r");
      if(fptr != NULL) {
	while(fscanf(fptr, "%s;", buffer) != EOF) {
	  returnVal = eval(buffer);
	  if(returnVal != LOOP)
	    break;
	}
	if(returnVal != LOOP)
	  break;
      } else {
	strcpy(errorMessage, "Invalid file.\n");
	errorMessageSize = strlen(errorMessage);
	sys_req(WRITE, TERMINAL, errorMessage, &errorMessageSize);
      }
    } 
  } else {
    invalidArgs(argv[0]);
  }
  return returnVal;
}
