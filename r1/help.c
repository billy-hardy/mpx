#include "r1.h"

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 9/7 Robert : Fixed print output (no more duplicate return locations)
//Pre-cond: commhand.c matched on help and argc == 1 || argc == 2
//          if argc == 2, argv[1] contains a valid command
//Pos-cond: contents of the corresponding file are printed to the screen
int help(int argc, char **argv) {
  if(argc == 1) {
	displayHelp(argc, NULL);
  } else if(argc == 2) {
	displayHelp(argc, argv[1]);
  } else {
    invalidArgs(argv[0]);
  }
  return LOOP;
 }

 
 //Author: Robert Brown
 //Date Created: 9/6
 //Last Modified: 9/7 by Robert:  Fixed print output to match file format.
 
 //TODO:	CREATE LOOPING STRUCTURE FOR PAGINATION
 //Pre-cond: help has matched the value of argc to a specific argument number of arguments
 //			 and argv contains a valid command.  Takes number of args: argc, and arguments pointer: command
 void displayHelp(int argc, char *command) {
  FILE *fptr;
  char file[48],cReturn[2], invalidCommand[30];
  char *buffer;
  int bufferSize, invCommandSize, cReturnSize;
  if(argc == 1) { //General help 
    fptr = fopen("r1/help/help.txt", "r");
  } else { //Specific command help
    sprintf(file, "r1/help/%s.txt", command);
    fptr = fopen(file, "r");
  }
  if(fptr != NULL) { // If file exists, perform writing to terminal
	fseek(fptr, 0L, SEEK_END);
	bufferSize = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);
    
	buffer = (char *)sys_alloc_mem(bufferSize+1);
	fread(buffer, bufferSize, 1, fptr);
	buffer[bufferSize]=0;
		
	sys_req(WRITE, TERMINAL, buffer, &bufferSize);

    strcpy(cReturn, "\n");
    cReturnSize = 1;
    sys_req(WRITE, TERMINAL, cReturn, &cReturnSize);
  } else { //Invalid command / file Catch.
    strcpy(invalidCommand,"No such command\n");
    invCommandSize = strlen(invalidCommand);
    sys_req(WRITE, TERMINAL, invalidCommand, &invCommandSize);
  }
  fclose(fptr);
  sys_free_mem(buffer);
}
