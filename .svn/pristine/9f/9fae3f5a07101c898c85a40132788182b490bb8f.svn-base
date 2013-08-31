#include "common.h"

//Author: Billy Hardy
//Date Created: 8/30 4pm
//Date Revised: n/a
//Purpose: Use when too few/many arguments are given
//Pre-cond: command is the name of the command calling this
//Post-cond: buffer is written to the screen 
void invalidArgs(char *command) {
  int bufferSize;
  char buffer[75];
  strcpy(buffer, "%s: invalid number of arguments. Type \"help\" for help.\n");
  sys_req(WRITE, TERMINAL, buffer, &bufferSize);
}
