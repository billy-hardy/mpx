#include "version.h"

int version(int argc, char **argv) {
  if(argc == 1) {
    char buffer[50];
    int bufferSize;
    strcpy(buffer, "MPX is at version %s, with that last modification on %i/%i/%i", VERSION, DAY, MONTH, YEAR);
    bufferSize = strlen(buffer);
    sys_req(WRITE, TERMINAL, buffer, &bufferSize);
  } else {
    invalidArgs("version");
  }
  return LOOP;
}
