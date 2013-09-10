#include "r2.h"

//Parameters: name, class and priority
// name must be unique
// class and priority must be valid
int createPCB(int argc, char **argv) {
  if(argc != 4) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
  }
  return LOOP;
}

//Parameters: name
// name must be valid
int deletePCB(int argc, char **argv) {
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
  }
  return LOOP;
}
