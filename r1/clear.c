#include "r1.h"

int clearScreen(int argc, char **argv) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    char dummy[12];
    int dummySize = 11;
    sys_req(CLEAR, TERMINAL, dummy, &dummySize);
  }
  return LOOP;
}
