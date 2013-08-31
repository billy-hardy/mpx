#include "commhand.h"

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/31 by Billy
//Pre-cond: input is user input, words separated by spaces or tabs
//Post-cond: input is split on the spaces and tabs and written to *argv
//           and *argc contains the length of *argv
void tokenizer(int *argc, char ***argv, char *input) {
  char delimeters[5], token[strlen(input)];
  *argc = 0;
  strcpy(delimeters, " \t");
  token = strtok(input, delimeters);
  while(token != NULL) {
    *argv[argc] = (char *) sys_mem_alloc(sizeof(char)*strlen(token));
    strcpy(*argv[argc], token);
    *argc++;
    token = strtok(NULL, delimeters);
  }
}

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/29 by Billy
//Pre-cond: none
//Post-cond: command is read in from user, parsed, matched to
//           a function, function is executed, and loop
int commhand() {
  int maxSize, promptSize, i, exitSize;
  char buffer[64], prompt[60], exitMessage[60];
  int (*functions[NUM_COMMANDS]) (int *, char **);
  char commands[NUM_COMMANDS][60];
  int argc;
  char **argv;
  int repl;
  maxSize = 64;
  promptSize = 3;
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
    tokenize(&argc, &argv, buffer);
    if(strcmp(argv[0], "rename")) { //I think rename might be extra credit
		if(strcmp(argv[1], "prompt")) {
			strcpy(prompt, argv[2]);
			promptSize = strlen(prompt);
		} else {
			rename(&commands, argc, argv);
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

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/31 by Billy (it was broken)
//Pre-cond: *commands is the array of command names, argc is the length of argv,
//          argv[1] is the command to be renamed and argv[2] is its new name
//Post-cond: proper command is renamed
void rename(char ***commands, int argc, char **argv) {
  int i;
  char **commandToRenamePtr;
  if(argc != 3) {
	invalidArgs(argv[0]);
  } else {
		for(i=0; i<NUM_COMMANDS; i++) {
			if(strcmp(*commands[i], argv[1])) {
				commandToRenamePtr= &(*commands[i]); //round about, but don't know if it will be parsed correctly otherwise
				strcpy(commandToRenamePtr, argv[2]);
			}
		}
	}
}
