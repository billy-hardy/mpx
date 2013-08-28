#include <stdio.h>
#include <string.h>

#define TRUE 1
#define NUM_COMMANDS 1

void tokenizer(int *, char **, char *);
void commhand();

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

void commhand() {
  int maxSize, promptSize, i;
  char buffer[64], prompt[3];
  int (*functions[NUM_COMMANDS]) (int *, char **);
  char commands[NUM_COMMANDS][60];
  int argc;
  char **argv;
  maxSize = 64;
  promptSize = 3;
  prompt = "$>";
  functions[0] = &help; commands[0] = "help";
  while(TRUE) {
    sys_req(WRITE, TERMINAL, prompt, &promptSize);
    sys_req(READ, TERMINAL, buffer, &maxSize);
    for(i=0; i<NUM_COMMANDS; i++) {
      if(commands[i] == argv[0]) {
	functions[i](argc, argv);
	break;
      }
    }
    sys_req(WRITE, TERMINAL, buffer, &maxSize);
  }
}
