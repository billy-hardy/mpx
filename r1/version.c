#include "version.h"

//Author: Billy Hardy
//Date Created: 8/30
//Last Modified: 8/31 by Billy (changed invalidArgs argument)
//Pre-cond: commhand.c matched on version and argc == 1
//Post-cond: the current version is printed to the screen
int version(int argc, char **argv) {
  if(argc == 1) {
    char buffer[50];
    int bufferSize;
    strcpy(buffer, "MPX is at version %s, with that last modification on %i/%i/%i", VERSION, DAY, MONTH, YEAR);
    bufferSize = strlen(buffer);
    sys_req(WRITE, TERMINAL, buffer, &bufferSize);
  } else {
    invalidArgs(argv[0]);
  }
  return LOOP;
}
