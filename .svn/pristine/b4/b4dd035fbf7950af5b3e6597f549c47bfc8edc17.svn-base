#include "help.h"

/*Author: Billy Hardy
 *
 *
 */
int help(int argc, char **argv) {
  FILE *fptr;
  char *buffer, invalidCommand[30];
  int fSize, result, lSize, iCommandSize;
  fptr = fopen(argv[2], "r");
  if(fptr != NULL) {
    fseek(fptr, 0, SEEK_END);
    fSize = ftell(fptr); //get size of file
    rewind(fptr);
    buffer = (char *) malloc(sizeof(char)*fSize);
    if(buffer != NULL) {
      result = fread(buffer, 1, lSize, fptr);
      if(result != lSize) {
	fputs("Reading error", stderr);
      }
      sys_req(WRITE, TERMINAL, buffer, &fSize);
    } else {
      fputs("Memory error", stderr);
    }
  } else {
    invalidCommand = "No such command";
    iCommandSize = strlen(invalidCommand);
    sys_req(WRITE, TERMINAL, invalidcommand, &iCommandSize);
  }
}
