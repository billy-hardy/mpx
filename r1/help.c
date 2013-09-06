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
    //fptr = fopen("generalHelp", "r");
  } else if(argc == 2) {
	displayHelp(argc, argv[1]);
    //fptr = fopen(argv[1], "r");
  } else {
    invalidArgs(argv[0]);
  }
  return LOOP; //Tanner would hate this, but I don't feel like duplicating the checks
 }
 
 void displayHelp(int argc, char *command){
  FILE *fptr;
  char buffer[60], invalidCommand[30];
  int bufferSize, invCommandSize, commandSize;
  
  if(argc == 1){
	//fptr = fopen("generalHelp.txt", "r");
	//commandSize = 0;
	}else{
		//fptr = fopen(command, "r");
		//commandSize = strlen(command);
		//command[commandSize] = '\0';
		
	}	
  if(fptr != NULL) {
    while(fscanf(fptr, "%s ", buffer) != EOF) {
      bufferSize = strlen(buffer);
      sys_req(WRITE, TERMINAL, buffer, &bufferSize);
      //TODO: read an enter
    } 
  } else {
    strcpy(invalidCommand,"No such command\n");
    invCommandSize = strlen(invalidCommand);
    sys_req(WRITE, TERMINAL, invalidCommand, &invCommandSize);
  }
  fclose(fptr);
}
