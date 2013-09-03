#include "r1.h"

//Author: Billy Hardy
//Date Created: 8/31 3pm
//Last Modified: n/a
//Pre-cond: commhand.c matched on exit and argc == 1
//Post-cond: exits repl iff user types 'y'
int exit(int argc, char **argv) {
  char confirm[75], ans[21];
  int confirmSize, maxSize, yes, response, returnVal;
  if(argc == 1) {
    strcpy(confirm, "Are you sure you want to exit mpx? (y/n)\n");
    confirmSize = strlen(confirm);
    do {
      sys_req(WRITE, TERMINAL, confirm, &confirmSize);
      maxSize = 20;
      sys_req(READ, TERMINAL, ans, &maxSize);
      yes = strcmp(ans, "y");
      response = yes || strcmp(ans, "n");
    } while(!response);
    returnVal = yes?(!LOOP):LOOP; //if 'y', exit repl, else continue repl
  } else {
    invalidArgs(argv[0]); //argv[0] has the name of the command
    returnVal = LOOP;
  }
  return returnVal;
}
