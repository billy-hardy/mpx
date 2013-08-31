#include "commhand.h"

void tokenizer(int *argc, char ***argv, char *input) {
  char delimeters[1], token[strlen(input)];
  *argc = 0;
  delimeters = {' '};
  token = strtok(input, delimeters);
  while(token != NULL) {
    *argv[argc] = (char *) sys_mem_alloc(sizeof(char)*strlen(token));
    strcpy(*argv[argc], token);
    *argc++;
    token = strtok(NULL, delimeters);
  }
}

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
  //functions go above here 
  repl = LOOP;
  while(repl) {
    sys_req(WRITE, TERMINAL, prompt, &promptSize);
    sys_req(READ, TERMINAL, buffer, &maxSize);
    tokenize(&argc, &argv, buffer);
    if(strcmp(argv[0], "rename")) {
      rename(&commands, argc, argv);
    } else {
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

void rename(char ***commands, int argc, char **argv) {
  int i;
  char **commandToRename;
  if(argc != 2) {
    invalidArgs("rename");
  } else {
    for(i=0; i<NUM_COMMANDS; i++) {
      if(strcmp(*commands[i], argv[0])) {
	commandToRenamePtr= &(*commands[i]);
	strcpy(commandToRenamePtr, argv[1]);
      }
    }
  }
}
