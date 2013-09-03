#include "r1.h"

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/31 by Billy (fixed logic)
//Pre-cond: commhand.c matched on help and argc == 1 || argc == 2
//          if argc == 2, argv[1] contains a valid command
//Pos-cond: contents of the corresponding file are printed to the screen
int help(int argc, char **argv) {
  FILE *fptr;
  char *buffer, invalidCommand[30];
  int fSize, result, lSize, invCommandSize;
  if(argc == 1) {
    fptr = fopen("generalHelp", "r");
  } else if(argc == 2) {
    fptr = fopen(argv[1], "r");
  } else {
    invalidArgs(argv[0]);
    return LOOP; //Tanner would hate this, but I don't feel like duplicating the checks
  }
  if(fptr != NULL) {
    fseek(fptr, 0, SEEK_END);
    fSize = ftell(fptr); //get size of file
    rewind(fptr);
    buffer = (char *) sys_alloc_mem(sizeof(char)*fSize);
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
    strcpy(invalidCommand,"No such command");
    invCommandSize = strlen(invalidCommand);
    sys_req(WRITE, TERMINAL, invalidCommand, &invCommandSize);
  }
  return LOOP;
}
