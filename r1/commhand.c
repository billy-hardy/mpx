#include "r2.h"
#include "r3.h"

pcb_queue *ready, *blocked;

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
  while(token != NULL){
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
  int maxSize, promptSize, i, invalidCommandSize, exitSize, numCommands;
  char buffer[129], prompt[60], invalidCommand[128], exitMessage[60], *commands[65];
  int repl;
  pcb *temp, *temp2, *temp3, *temp4; 
  promptSize = 2;
  strcpy(prompt, "$>");
  buffer[0] = '\0';
  strcpy(buffer, "Welcome to MPX!\n\n");
  maxSize = strlen(buffer);
  sys_req(WRITE, TERMINAL, buffer, &maxSize);
  maxSize = 129;
  do { //Loops until exit command is given
    buffer[0] = '\0';
    sys_req(WRITE, TERMINAL, prompt, &promptSize);
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
  cleanUpHistory();
  queueFree();
  delay(750);
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
  functions[6] = &createPCB;        strcpy(commands[6], "create");
  functions[7] = &setPCBPriority;   strcpy(commands[7], "setPriority");
  functions[8] = &deletePCB;        strcpy(commands[8], "delete");
  functions[9] = &blockPCB;         strcpy(commands[9], "block");
  functions[10] = &unblockPCB;      strcpy(commands[10], "unblock");
  functions[11] = &suspendPCB;      strcpy(commands[11], "suspend");
  functions[12] = &resumePCB;       strcpy(commands[12], "resume");
  functions[13] = &showAll;         strcpy(commands[13], "showAll");
  functions[14] = &showPCB;         strcpy(commands[14], "showPCB");
  functions[15] = &showReady;       strcpy(commands[15], "showReady");
  functions[16] = &showBlocked;     strcpy(commands[16], "showBlocked");
  functions[17] = &exec;            strcpy(commands[17], "exec");
  functions[18] = &clearScreen;     strcpy(commands[18], "clear");
  functions[19] = &loadTestProcess; strcpy(commands[19], "load");
  functions[20] = &callDispatch;    strcpy(commands[20], "dispatch");
	
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

//Initializes the Queue
//Author Billy, Last Edited, Robert
void queueInit() {
  ready = (pcb_queue *) sys_alloc_mem(sizeof(pcb_queue));
  ready->head = NULL;
  ready->tail = NULL;
  ready->count = 0;
  blocked = (pcb_queue *) sys_alloc_mem(sizeof(pcb_queue));
  blocked->head = NULL;
  blocked->tail = NULL;
  blocked->count = 0;
}
//Frees leftover PCBs on Exit
//Author Billy
void queueFree() {
  pcb *temp;
  while(ready->count > 0) {
    temp = ready->head;
    removePCB(temp);
    freePCB(temp);
  }
  sys_free_mem(ready);
  while(blocked->count > 0) {
    temp = blocked->head;
    removePCB(temp);
    freePCB(temp);
  }
  sys_free_mem(blocked);
}
