#include "r1.h"

int exec(int argc, char **argv) {
  int returnVal = LOOP;
  const char *delims = ";";
  if(argc != 1) {
    int i, j, numCommands=0, fLength;
    FILE *fptr;
    char *buffer, errorMessage[128], *commands[129];
    int errorMessageSize;
    for(i=1; i<argc; i++) {
      fptr = fopen(argv[i], "r");
			if(fptr != NULL) {
				fseek(fptr, 0L, SEEK_END);
				fLength = ftell(fptr);
				fseek(fptr, 0L, SEEK_SET);
				//rewind(fptr);
				buffer = (char *)sys_alloc_mem(fLength*sizeof(char));
				fread(buffer, 1, fLength, fptr);
				tokenize(&numCommands, commands, buffer, delims);
				for(j=0; j<numCommands; j++) {
					returnVal = eval(commands[j]);
					if(!returnVal) break;
				}
				for(j=0; j<numCommands; j++) {
					commands[j][0] = '\0';
				}
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
