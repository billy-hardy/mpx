#include "r2.h"

extern pcb *running;
extern pcb_queue *ready;
extern pcb_queue *blocked;

//Parameters: name
// Must display name, class, state, suspended status and priority
// Displays error if PCB doesn't exist
int showPCB(int argc, char **argv) { //This is a little bloated with variables....
  if(argc != 2) {
	pcb *tempPCB;
	char PCBBuffer[256];
	char tempLine[128];
	int bufferSize, tempInt;
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
	if((tempPCB =findPCB(argv[1])) != NULL){
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
	else
		printError(PCB_NOT_FOUND);
  }
  return LOOP;
}

//Parameters: none
//shows all info for all PCBs in ready queue
//requires PAGINATION
int showReady(int argc, char **argv) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
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
    //TODO: stuff
  }
  return LOOP;
}

//Parameters: none
//shows all info for all PCBs in all queues
//requires PAGINATION
int showAll(int argc, char **argv) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
  }
  return LOOP;
}
