#include "r2.h"

// Author: Robert Brown
// Parameters: name
// Must display name, class, state, suspended status and priority
// Displays error if PCB doesn't exist
int showPCB(int argc, char **argv) { 
  pcb *tempPCB;
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else if((tempPCB =findPCB(argv[1])) != NULL) {
		printTable();
		printPCB(tempPCB);
	} else {
		printError(PCB_NOT_FOUND);
	}
	return LOOP;
}

//Author: Robert Brown
//Parameters: none
//shows all info for all PCBs in ready queue
int showReady(int argc, char **argv) { 
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
		showQueue(ready);
  }
  return LOOP;
}

//Author: Robert Brown
//Parameters: argc, number of token args, argv, the arguments
//Returns: LOOP (1)
//shows all info for all PCBs in blocked queue
int showBlocked(int argc, char **argv) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
		showQueue(blocked);
  }
  return LOOP;
}

//Author: Robert Brown
//Parameters: argc, number of token args, argv, the arguments
//Returns: LOOP (1)
//shows all info for all PCBs in all queues
int showAll(int argc, char **argv) { //Come up with a way to fix this duplication
	char dummy[1024];
	int dummySize;
	
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    if(ready -> count >0){
			strcpy(dummy,"\nREADY QUEUE:\n");
			dummySize = strlen(dummy);
			sys_req(WRITE, TERMINAL, dummy, &dummySize);
			showQueue(ready);
		} else{
			strcpy(dummy, "\nNothing exists in the Ready queue!\n");
			dummySize = strlen(dummy);
			sys_req(WRITE, TERMINAL, dummy, &dummySize);	
		}
		sys_req(READ, TERMINAL, dummy, &dummySize);
		if(blocked -> count > 0){
			strcpy(dummy,"\nBLOCKED QUEUE:\n");
			dummySize = strlen(dummy);
			sys_req(WRITE, TERMINAL, dummy, &dummySize);
			showQueue(blocked);
		} else{
			strcpy(dummy, "Nothing exists in the Blocked queue!\n");
			dummySize = strlen(dummy);
			sys_req(WRITE, TERMINAL, dummy, &dummySize);	
		}
  }
  return LOOP;
}

//Author: Robert Brown
//Input: pcb_queue in (a valid pcb queue structure)
//Output: void
//Displays with pagination all of the PCBs in the passed queue
void showQueue(pcb_queue *in){ 
	pcb *tempPCB;
	char dummy[512]; 
	int dummySize;
	int pcbsPrinted = 1;
	if(in -> count > 0) {
		printTable();
		tempPCB = in -> head;
		while(tempPCB != NULL){
			if(pcbsPrinted%15) {
				printPCB(tempPCB);
			} else {
				printPCB(tempPCB);
				sys_req(READ, TERMINAL, dummy, &dummySize);
				printTable();
			}
			pcbsPrinted++;
			tempPCB = tempPCB->next; 
		}		
	}
	else{
		strcpy(dummy, "Nothing exists in this queue!\n");
		dummySize = strlen(dummy);
		sys_req(WRITE, TERMINAL, dummy, &dummySize);
	}
}

//Author: Billy Hardy
//Input: void
//Output: void
//Neatly prints out PCB queue information
void printTable() {
	char buffer[128];
	int bufferSize;
	strcpy(buffer, "\nName\tClass\t\tState\tSuspended\tPriority\n");
	bufferSize = strlen(buffer);
	sys_req(WRITE, TERMINAL, buffer, &bufferSize);
}

//Author: Robert Brown
//Input: PCB (the PCB to be printed)
//Output: void
//Displays the information about the passed PCB
void printPCB(pcb* tempPCB) {
	char PCBBuffer[256];
	char tempLine[128];
	int bufferSize, tempInt;
	
	sprintf(tempLine, "%s\t", tempPCB->name);
	strcpy(PCBBuffer, tempLine);
	tempInt = tempPCB->class;
	sprintf(tempLine, "%s\t", tempInt == 1 ? "Application" : "System\t");
	tempInt = tempPCB->state;
	strcat(PCBBuffer, tempLine);
	sprintf(tempLine, "%s\t", tempInt == -1 ? "Blocked" : tempInt == 0 ? "Ready" :  "Running");
	strcat(PCBBuffer, tempLine);
	tempInt = tempPCB->suspended;
	sprintf(tempLine, "%s\t\t", tempInt == TRUE ? "TRUE" : "FALSE");
	strcat(PCBBuffer, tempLine);
	tempInt = tempPCB->priority;
	sprintf(tempLine, "%d\n", tempInt);
	strcat(PCBBuffer, tempLine);
	
	bufferSize = strlen(PCBBuffer);
	sys_req(WRITE, TERMINAL, PCBBuffer, &bufferSize);
}