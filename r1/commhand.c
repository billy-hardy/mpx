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
  int maxSize, promptSize, i, invalidCommandSize, exitSize;
  char buffer[64], prompt[60], invalidCommand[60], exitMessage[60];
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
    if(strcmp(argv[0], "renamePrompt")) { //I think rename might be extra credit
      if(argc != 2) {
	invalidArgs(argv[0]);
      } else {
	strcpy(prompt, argv[1]);
	promptSize = strlen(prompt);
      } 
    } else {                        //Loop through the other commands
      for(i=0; i<NUM_COMMANDS; i++) {
	if(strcmp(commands[i], argv[0])) {
	  repl = functions[i](argc, argv);
	  break;
	}
      }
      if(i==NUM_COMMANDS) {
	strcpy(invalidCommand, "That is not a valid command. Type \"help\" for help");
	invalidCommandSize = strlen(invalidCommand);
	sys_req(WRITE, TERMINAL, invalidCommand, &invalidCommandSize);
      }
    }
  }
  strcpy(exitMessage,"Goodbye\n");
  exitSize = strlen(exitMessage);
  sys_req(WRITE, TERMINAL, exitMessage, &exitSize);
  //Clean up processes here
  return 0;
}
