#include "ls.h"

//Author:
//Date Created:
//Last Modified:
//Pre-cond: commhand.c matches on ls and argc == 1 | 2
//          if 2, argv[1] contains a valid directory
//Post-cond: directory listing is printed to screen
int ls(int argc, char **argv) {
  
  if(argc == 1) {
    //do shit
  } else if(argc == 2) {
    //do other shit
  } else {
    invalidArgs(argv[0]);
  }
  return LOOP;
}
