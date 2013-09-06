#include "r1.h"

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/31 by Billy
//		 9/5/13 by Robert
//				Changes made to tokenize and commhand
//				the repl should now work correctly.
//				sorry I had to remove the ***
//Pre-cond: input is user input, words separated by spaces or tabs
//Post-cond: input is split on the spaces and tabs and written to *argv
//           and *argc contains the length of *argv
void tokenize(int *argc, char *argv[], char *input) {
  char *token;
  int tempArgC=0;
  token = strtok(input, " \t\n");
  while(token != NULL){
  argv[tempArgC] = token;
  token = strtok(NULL, " \t\n");
  tempArgC++;
  }
  *argc = tempArgC;

}

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/29 by Billy
//Pre-cond: none
//Post-cond: command is read in from user, parsed, matched to
//           a function, function is executed, and loop
int commhand() {
  int maxSize, promptSize, i, invalidCommandSize, exitSize;
  char buffer[64], prompt[60], invalidCommand[128], exitMessage[60];
  int (*functions[NUM_COMMANDS]) (int, char **);
  char commands[NUM_COMMANDS][60];
  int argc;
  char *argv[5];  //Memory Leak here?  Is this being allocated correctly later when it is used?
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
    buffer[0] = '\0';
    sys_req(WRITE, TERMINAL, prompt, &promptSize);
    sys_req(READ, TERMINAL, buffer, &maxSize);
    tokenize(&argc, argv, buffer);
    if(strcmp(argv[0], "renamePrompt")==1) { //I think rename might be extra credit
      if(argc != 2) {
	invalidArgs(argv[0]);
      } else {
	strcpy(prompt, argv[1]);
	promptSize = strlen(prompt);
      }
    } else {                        //Loop through the other commands
      for(i=0; i<NUM_COMMANDS; i++) {
	if(!strcmp(commands[i], argv[0])) {
	  repl = functions[i](argc, argv);
	  break;
	}
      }
      if(i==NUM_COMMANDS) {
	strcpy(invalidCommand, "That is not a valid command.\nType \"help\" for more information!\n");
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
