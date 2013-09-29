#include "r2.h"

extern pcb *running;
extern pcb_queue *ready;
extern pcb_queue *blocked;

//Parameters: name
// calls findPCB(), changes PCB to blocked state
// and reinserts it to correct queue
int blockPCB(int argc, char **argv) {
  pcb *process;
  int errorCode;
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    process = find(argv[1]);
    if(process != NULL) {
      if(removePCB(process) == SUCCESS) {
		process->state = BLOCKED;
		insertPCB(process);
      }
      errorCode = SUCCESS; //Move this up?********************
    } else {
      errorCode = PCB_NOT_FOUND;
    }
    printError(errorCode);
  }
  return LOOP;
}


//This should work RB
//Parameters: name
// calls findPCB(), changes PCB to unblocked state
// and reinserts it to correct queue
int unblockPCB(int argc, char **argv) {
	pcb *tempPCB;
	int errorCode;
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    if((tempPCB = findPCB(argv[1])) != NULL){
		if(removePCB(tempPCB) == SUCCESS){
			tempPCB->state = READY;
			insertPCB(tempPCB);
			errorCode = SUCCESS;
		}
	}
	else{
		errorCode = PCB_NOT_FOUND;
	}
	printError(errorCode);
  }
  return LOOP;
}

//Parameters: name
// calls findPCB(), changes PCB to suspended state
// and reinserts it to correct queue
int suspendPCB(int argc, char **argv) { //How to handle the running process?
	pcb *tempPCB;
	int errorCode;
  if(argc != 2) {
    invalidArgs(argv[0]);
  } 
  else {
	if((tempPCB = findPCB(argv[1])) != NULL){
		tempPCB->suspended = TRUE;
		errorCode = SUCCESS;
	}
	else{
		errorCode = PCB_NOT_FOUND;
	}
	printError(errorCode);
  }
  return LOOP;
}

//Parameters: name
// calls findPCB(), changes PCB to not suspended state
// and reinserts it to correct queue
int resumePCB(int argc, char **argv) { //Handle the running process  ** Does this need to be removed and inserted
	pcb *tempPCB;
	int errorCode;
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
	if((tempPCB = findPCB(argv[1])) != NULL){
		tempPCB->suspended = FALSE;
		errorCode = SUCCESS;
	}
	else
		errorCode = PCB_NOT_FOUND;
	
	printError(errorCode);
  }
  return LOOP;
}

//Parameters: name and priority
// calls findPCB(), changes PCB priority
// and removes and reinserts it (to maintain priority queue)
int setPCBPriority(int argc, char **argv) {//Handle the running process
	pcb *tempPCB;
	int errorCode;
	int processVal;
  if(argc != 3) { 
    invalidArgs(argv[0]);
  } else {
	if((tempPCB = findPCB(argv[1])) != NULL){
		
		if((processVal = integerCheck(argv[2])) != NULL){
			remove(tempPCB);
			tempPCB->processVal;
			insertPCB(tempPCB);
		}
		else{
		//Not an integer ... ADD ERROR CODES FOR THIS
		
		}
	}
	else
		errorCode = PCB_NOT_FOUND;
	printError(errorCode);
  }
  return LOOP;
}
//Just a test to see how this branching works
