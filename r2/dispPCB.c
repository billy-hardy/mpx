#include "r2.h"

//Parameters: name
// Must display name, class, state, suspended status and priority
// Displays error if PCB doesn't exist
int showPCB(int argc, char **argv) { //This is a little bloated with variables....
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

//Parameters: none
//shows all info for all PCBs in ready queue
//requires PAGINATION
int showReady(int argc, char **argv) { // I think this should work.
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
		showQueue(ready);
  }
  return LOOP;
}

//Parameters: none
//shows all info for all PCBs in blocked queue
//requires PAGINATION
int showBlocked(int argc, char **argv) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
		showQueue(blocked);
  }
  return LOOP;
}

//Parameters: none
//shows all info for all PCBs in all queues
//requires PAGINATION
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

void printTable() {
	char buffer[128];
	int bufferSize;
	strcpy(buffer, "\nName\tClass\t\tState\tSuspended\tPriority\n");
	bufferSize = strlen(buffer);
	sys_req(WRITE, TERMINAL, buffer, &bufferSize);
}

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