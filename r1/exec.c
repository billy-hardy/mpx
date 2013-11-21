#include "r1.h"

int exec(int argc, char **argv) {
  int returnVal = LOOP;
  const char *delims = ";\n";
  if(argc != 1) {
    int i, j, fLength;
    FILE *fptr;
    char *buffer, errorMessage[128], *command;
    int errorMessageSize;
    for(i=1; i<argc; i++) {
      fptr = fopen(argv[i], "r");
      if(fptr != NULL) {
				fseek(fptr, 0L, SEEK_END);
				fLength = ftell(fptr);
				fseek(fptr, 0L, SEEK_SET);
				buffer = (char *)sys_alloc_mem(fLength*sizeof(char));
				fread(buffer, 1, fLength, fptr);
				command = strtok(buffer, delims);
				do {
					if(strlen(command) < 127) {
						returnVal = eval(command);
						if(!returnVal) break;
					}
				} while(command = strtok(NULL, delims) != NULL);
      } else {
				strcpy(errorMessage, "Invalid file.\n");
				errorMessageSize = strlen(errorMessage);
				sys_req(WRITE, TERMINAL, errorMessage, &errorMessageSize);
      }
      sys_free_mem(buffer);
      fclose(fptr);
    } 
  } else {
    invalidArgs(argv[0]);
  }
  return returnVal;
}
