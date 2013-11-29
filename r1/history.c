#include "r1.h"
/* HISTORY
   Author: Billy Hardy
   Input: argc (number of args), argv (actual command)
   Output: 1 (LOOP)
   Command to display recent history of commands executed
   Accepts only a valid history command, or returns invalid args */
int history(int argc, char *argv[]) {
  FILE *fptr;
  char buffer[130];
  int bufferSize;
	int lines = 1;
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    fptr = fopen("history.txt", "r");
		while(fgets(buffer, 100, fptr) != NULL) {
			bufferSize = strlen(buffer);
			sys_req(WRITE, TERMINAL, buffer, &bufferSize);
			if(lines%22 == 0) {
			  sys_req(READ, TERMINAL, buffer, &bufferSize);
				lines = 0;
			}
			lines++;
		}
		fclose(fptr);
		if(lines == 0) {
			strcpy(buffer, "No commands executed thus far.\n");
			bufferSize = strlen(buffer);
			sys_req(WRITE, TERMINAL, buffer, &bufferSize);
		}
	}
  return LOOP;
}
/* PRINTCOMMANDTOFILE
   Author: Billy Hardy
   Input: char[] (actual command executed)
   Output: void
   Writes command to history file to be referenced on history command */
void printCommandToFile(char command[]) {
  FILE *fptr;
  fptr = fopen("history.txt", "a");
  fprintf(fptr, command);
  fclose(fptr);
}
/* CLEANUPHISTORY
   Author: Billy Hardy
   Input: void
   Output: void
   Removes the 'history.txt' file upon exit of MPX */
void cleanUpHistory() {
  remove("history.txt");
}
