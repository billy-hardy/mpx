#include <stdio.h>

/*Author: Billy Hardy
 *
 *
 */
int help(int argc, char *argv) {
  FILE *fptr;
  char *buffer;
  int fSize;
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
    } else {
      fputs("Memory error", stderr);
    }
  } else {
    sys_req(WRITE, TERMINAL,"No such command"); //TODO write to command line
  }
}
