#include "r2.h"

//Parameters: name
// Must display name, class, state, suspended status and priority
// Displays error if PCB doesn't exist
int showPCB(int argc, char **argv) { //This is a little bloated with variables....
  	pcb *tempPCB;

  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
	if((tempPCB =findPCB(argv[1])) != NULL){
		printPCB(tempPCB);
	}
	else
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
	int pcbsDisplayed = 0;
	pcb *tempPCB = NULL; 
	
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    if(ready -> count >0){
		strcpy(dummy,"\nREADY QUEUE:\n");
		dummySize = strlen(dummy);
		sys_req(WRITE, TERMINAL, dummy, &dummySize);
		tempPCB = ready->head;
		while(tempPCB != NULL){
			pcbsDisplayed++;
			if((pcbsDisplayed%4))
				printPCB(tempPCB);
			else{
				sys_req(READ, TERMINAL, dummy, &dummySize);
				printPCB(tempPCB);
			}
			tempPCB = tempPCB->next;
		}
	}
	else{
		strcpy(dummy, "\nNothing exists in the Ready queue!\n");
		dummySize = strlen(dummy);
		sys_req(WRITE, TERMINAL, dummy, &dummySize);	
	}
	if(blocked -> count > 0){
	strcpy(dummy,"\nBLOCKED QUEUE:\n");
	dummySize = strlen(dummy);
	sys_req(WRITE, TERMINAL, dummy, &dummySize);
		tempPCB = blocked->head;
		while(tempPCB != NULL){
			pcbsDisplayed++;
			if((pcbsDisplayed%4))
				printPCB(tempPCB);
			else{
				sys_req(READ, TERMINAL, dummy, &dummySize);
				printPCB(tempPCB);
			}
			tempPCB = tempPCB->next; //Changed this to prev;
		}
	
	}
	else{
		strcpy(dummy, "\nNothing exists in the Blocked queue!\n");
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
	int loopVal = 0;
	
	if(in -> count > 0){
		tempPCB = in -> head;
		while(loopVal < in -> count){
			loopVal++;
			if(loopVal%4)
				printPCB(tempPCB);
			else
				sys_req(READ, TERMINAL, dummy, &dummySize); 
				
			tempPCB = tempPCB->next; 
		}		
	}
	else{
		strcpy(dummy, "Nothing exists in this queue!\n");
		dummySize = strlen(dummy);
		sys_req(WRITE, TERMINAL, dummy, &dummySize);
	}
}

void printPCB(pcb* tempPCB){
	char PCBBuffer[256];
	char tempLine[128];
	int bufferSize, tempInt;
	
	sprintf(tempLine, "Displaying PCB named: %s\n", tempPCB->name);
	strcpy(PCBBuffer, tempLine);
	tempInt = tempPCB->class;
	sprintf(tempLine, "Class: %s", tempInt == 1 ? "Application\n" : "System\n");
	tempInt = tempPCB->state;
	strcat(PCBBuffer, tempLine);
	sprintf(tempLine, "State: %s", tempInt == -1 ? "Blocked\n" : tempInt == 0 ? "Ready\n" :  "Running\n");
	strcat(PCBBuffer, tempLine);
	tempInt = tempPCB->suspended;
	sprintf(tempLine, "Suspended Status: %s", tempInt == TRUE ? "TRUE\n" : "FALSE\n");
	strcat(PCBBuffer, tempLine);
	tempInt = tempPCB->priority;
	sprintf(tempLine, "Priority: %d\n", tempInt);
	strcat(PCBBuffer, tempLine);
	
	bufferSize = strlen(PCBBuffer);
	sys_req(WRITE, TERMINAL, PCBBuffer, &bufferSize);
}