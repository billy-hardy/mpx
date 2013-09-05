#include "r1.h"

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/31 by Billy
//Pre-cond: input is user input, words separated by spaces or tabs
//Post-cond: input is split on the spaces and tabs and written to *argv
//           and *argc contains the length of *argv
void tokenize(int *argc, char ***argv, char *input) {
  char token[60];
  strcpy(token, strtok(input, " \t\n"));
  for((*argc)=0; (*argc) < 5; tokenLoop++) { //continue while less than 5 arguments
    strcpy((*argv)[*argc], token);
    (*argc)++;
    strcpy(token, strtok(NULL, " \t\n"));
    if(token == NULL) //break when out of tokens
      break;
  }
}

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/29 by Billy
//Pre-cond: none
//Post-cond: command is read in from user, parsed, matched to
//           a function, function is executed, and loop
int commhand() {
  int maxSize, promptSize, i, exitSize, invalidRenameSize;
  char buffer[64], prompt[60], exitMessage[60], invalidRename[60];
  int (*functions[NUM_COMMANDS]) (int, char **);
  char commands[NUM_COMMANDS][60];
  int argc;
  char *argv[5];
  int repl;
  maxSize = 64;
  promptSize = 2;
  strcpy(prompt, "$>");
  //functions go below here
  functions[0] = &help;    strcpy(commands[0], "help");
  functions[1] = &date;    strcpy(commands[1], "date");
  functions[2] = &version; strcpy(commands[2], "version");
  functions[3] = &ls;      strcpy(commands[3], "ls");
  functions[4] = &exit;    strcpy(commands[4], "exit");
  //functions go above here (don't forget to change NUM_COMMANDS)
  repl = LOOP;
  while(repl) {
    sys_req(WRITE, TERMINAL, prompt, &promptSize);
    sys_req(READ, TERMINAL, buffer, &maxSize);
    strcat(buffer, "\0");
    tokenize(&argc, &argv, buffer);
    if(strcmp(argv[0], "rename")) { //I think rename might be extra credit
      if(argc != 3) {
	invalidArgs(argv[0]);
      } else if(strcmp(argv[1], "prompt")) {
	strcpy(prompt, argv[2]);
	promptSize = strlen(prompt);
      } else { //check if renaming other commands
	for(i=0; i<NUM_COMMANDS; i++) {
	  if(strcmp(commands[i], argv[1])) {
	    strcpy(commands[i], argv[2]);
	    break;
	  }
	}
	if(i==NUM_COMMANDS) {
	  strcpy(invalidRename, "That's not a command, sorry. Type commands for a list of commands\n"); //TODO: make "commands" command.
	  invalidRenameSize = strlen(invalidRename);
	  sys_req(WRITE, TERMINAL, invalidRename, &invalidRenameSize);
	}
      }
    } else {                        //Loop through the other commands
      for(i=0; i<NUM_COMMANDS; i++) {
	if(strcmp(commands[i], argv[0])) {
	  repl = functions[i](argc, argv);
	  break;
	}
      }
    }
  }
  strcpy(exitMessage,"Goodbye");
  exitSize = strlen(exitMessage);
  sys_req(WRITE, TERMINAL, exitMessage, &exitSize);
  //Clean up processes here
  return 0;
}
