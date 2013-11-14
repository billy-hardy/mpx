#include "r1.h"

//Author: Billy Hardy
//Date Created: 8/31 3pm
//Last Modified: 9/5 by Billy:  added case sensitivity
//Pre-cond: commhand.c matched on exit and argc == 1
//Post-cond: exits repl iff user types 'y'
int exitMPX(int argc, char **argv) {
  char confirm[129], ans[129];
  int confirmSize, maxSize, yes, response, returnVal;
  if(argc == 1) {
    strcpy(confirm, "Are you sure you want to exit mpx? (y/n)\n");
    confirmSize = strlen(confirm);
    do {
      sys_req(WRITE, TERMINAL, confirm, &confirmSize);
      maxSize = 129;
      sys_req(READ, TERMINAL, ans, &maxSize);
      yes = (ans[0] == 'y') || (ans[0] == 'Y');
      response = yes || ((ans[0] == 'n') || (ans[0] == 'N'));
    } while(!response);
    returnVal = (yes?(!LOOP):LOOP); //if 'y', exit repl, else continue repl
  } else { //Invalid catch
    invalidArgs(argv[0]); //argv[0] has the name of the command
    returnVal = LOOP;
  }
  if(returnVal == !LOOP) {
    queueFree();
  }
  return returnVal;
}
