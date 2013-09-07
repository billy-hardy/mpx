#include "r1.h"

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/31 by Billy (fixed logic)
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

 void displayHelp(int argc, char *command) {
  FILE *fptr;
  char file[48],cReturn[2], invalidCommand[30];
  char *buffer;
  int bufferSize, invCommandSize, cReturnSize;
  if(argc == 1) {
    fptr = fopen("help.txt", "r");
  } else {
    strcpy(file, command);
    strcat(file, ".txt");
    fptr = fopen(file, "r");
  }
  if(fptr != NULL) {
	fseek(fptr, 0L, SEEK_END);
	bufferSize = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);
    
	buffer = (char *)sys_alloc_mem(bufferSize+1);
	fread(buffer, bufferSize, 1, fptr);
	buffer[bufferSize]=0;
		
		sys_req(WRITE, TERMINAL, buffer, &bufferSize);
	
	
	//while(fscanf(fptr, "%s ", buffer) != EOF) {
    //  strcat(buffer, " ");
    //  bufferSize = strlen(buffer);
    //  sys_req(WRITE, TERMINAL, buffer, &bufferSize);
      //TODO: read an enter
    
    strcpy(cReturn, "\n");
    cReturnSize = 1;
    sys_req(WRITE, TERMINAL, cReturn, &cReturnSize);
  } else {
    strcpy(invalidCommand,"No such command\n");
    invCommandSize = strlen(invalidCommand);
    sys_req(WRITE, TERMINAL, invalidCommand, &invCommandSize);
  }
  fclose(fptr);
  sys_free_mem(buffer);
}
