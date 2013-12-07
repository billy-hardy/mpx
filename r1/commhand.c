#include "r2.h"
#include "r3.h"

char prompt[60];
int promptSize;

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 8/31 by Billy
//		 9/5/13 by Robert
//				Changes made to tokenize and commhand
//				the repl should now work correctly.
//				sorry I had to remove the ***
//		 9/7/13 by Robert
//				Fixed the repl to allow for blank entries.
//				Added some \n's and commented code
//Pre-cond: input is user input, words separated by spaces or tabs
//Post-cond: input is split on the spaces and tabs and written to *argv
//           and *argc contains the length of *argv
void tokenize(int *argc, char *argv[], char *input, const char *delimeters) {
  char *token;
  int tempArgC=0;
  token = strtok(input, delimeters);
  while(token != NULL) {
    argv[tempArgC] = token;
    token = strtok(NULL, delimeters);
    tempArgC++;
  }
  *argc = tempArgC;
}

//Author: Billy Hardy
//Date Created: 8/28
//Last Modified: 10/3 by Robert
//Pre-cond: none
//Post-cond: command is read in from user, parsed, matched to
//           a function, function is executed, and loop
void commhand() {
  int maxSize, i, invalidCommandSize, exitSize, numCommands;
  char buffer[129], invalidCommand[128], exitMessage[60], *commands[65];
  int repl;
  pcb *temp, *temp2, *temp3, *temp4;
  strcpy(prompt, "$>");
  promptSize = strlen(prompt);
  buffer[0] = '\0';
  strcpy(buffer, "Welcome to MPX!\n\n");
  maxSize = strlen(buffer);
  sys_req(WRITE, TERMINAL, buffer, &maxSize);
  maxSize = 129;
  do { //Loops until exit command is given
    buffer[0] = '\0';
    sys_req(WRITE, TERMINAL, prompt, &promptSize);
		maxSize = 128;
    sys_req(READ, TERMINAL, buffer, &maxSize);
    tokenize(&numCommands, commands, buffer, ";");
    for(i=0; i<numCommands; i++) {
      if(strlen(commands[i]) > 1) {
				repl = eval(commands[i]);
      }
      if(!repl) break;
    }
  } while(repl);
  strcpy(exitMessage,"Goodbye\n");
  exitSize = strlen(exitMessage);
  sys_req(WRITE, TERMINAL, exitMessage, &exitSize);
  delay(800);
  empty_queue(ready);
  empty_queue(blocked);
  cleanUpHistory();
}

int eval(char *buffer) {
  int argc, repl, i, invalidCommandSize;
  char *argv[65], invalidCommand[100], print[129];
  int (*functions[NUM_COMMANDS]) (int, char **);
  char commands[NUM_COMMANDS][128];
  //functions go below here
  functions[0] = &help;             strcpy(commands[0], "help");
  functions[1] = &date;             strcpy(commands[1], "date");
  functions[2] = &version;          strcpy(commands[2], "version");
  functions[3] = &ls;               strcpy(commands[3], "ls");
  functions[4] = &exitMPX;          strcpy(commands[4], "exit");
  functions[5] = &history;          strcpy(commands[5], "history");
  functions[6] = &setPCBPriority;   strcpy(commands[6], "setPriority");
  functions[7] = &suspendPCB;       strcpy(commands[7], "suspend");
  functions[8] = &resumePCB;        strcpy(commands[8], "resume");
  functions[9] = &showAll;          strcpy(commands[9], "showAll");
  functions[10] = &showPCB;         strcpy(commands[10], "showPCB");
  functions[11] = &showReady;       strcpy(commands[11], "showReady");
  functions[12] = &showBlocked;     strcpy(commands[12], "showBlocked");
  functions[13] = &exec;            strcpy(commands[13], "exec");
  functions[14] = &clearScreen;     strcpy(commands[14], "clear");
  functions[15] = &load;	    			strcpy(commands[15], "load");
  functions[16] = &terminate;	    	strcpy(commands[16], "terminate");
  functions[17] = &changePrompt;    strcpy(commands[17], "prompt");
	
  //functions go above here
  strcpy(print, buffer);
  tokenize(&argc, argv, buffer, " \t\n");
  for(i=0; i<NUM_COMMANDS; i++) {
    if(!strcmp(commands[i], argv[0])) {
      repl = functions[i](argc, argv);
      break;
    }
  }
  printCommandToFile(print);
  if(i==NUM_COMMANDS) {  //Invalid argument catch
    strcpy(invalidCommand, "\nThat is not a valid command.\nType \"help\" for more information!\n\n");
    invalidCommandSize = strlen(invalidCommand);
    sys_req(WRITE, TERMINAL, invalidCommand, &invalidCommandSize);
  }
  return repl;
}


int changePrompt(int argc, char **argv) {
  int len;
  char inv[100];
  if(argc == 1) {
    strcpy(prompt, "$>");
  } else if(argc == 2) {
    if(strlen(argv[1]) < 11) {
      strcpy(prompt, argv[1]);
    } else {
      strcpy(inv, "\nThe max size of the prompt is 10.\n\n");
      len = strlen(inv);
      sys_req(WRITE, TERMINAL, inv, &len);
    }
  } else {
    invalidArgs(argv[0]);
  }
	promptSize = strlen(prompt);
  return LOOP;
}
