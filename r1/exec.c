#include "r1.h"
#include <string.h>

/* Author: Billy Hardy
   Last Edited : 11/29/2013 -- Robert (added comments)
   Input: argc(number of command tokens) argv (string of tokens)
   Output: LOOP (1)
   Used for execution of commands within a file (extra credit) */
	 
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
				strncpy(command, strtok(buffer, delims), 127);
				do {
					if(strlen(command) < 127) {
						returnVal = eval(command);
						if(!returnVal) break;
					}
					strncpy(command, strtok(NULL, delims), 127);
				} while(command != NULL);
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
