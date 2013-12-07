#include "r2.h"

//BLOCKPCB
//Author: Robert Brown
//Input: argc(int number of token arguments),
//       argv(the token containing the name of the PCB to block
//Output: LOOP (1)
//Calls findPCB(), changes PCB to blocked state
//and reinserts it to correct queue
int blockPCB(int argc, char **argv) {
  pcb *process;
  int errorCode;
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    process = findPCB(argv[1]);
    if(process != NULL) {
      if(removePCB(process) == SUCCESS) {
		process->state = BLOCKED;
		insertPCB(process);
      }
      errorCode = SUCCESS; 
    } else {
      errorCode = PCB_NOT_FOUND;
    }
    printError(errorCode);
  }
  return LOOP;
}



//SUSPENDPCB
//Author: Robert Brown
//Input: argc(int number of token arguments),
//       argv(the character arrays of tokens)
//Output: LOOP(1)
// Calls findPCB(), changes PCB to suspended state
// and reinserts it to correct queue
int suspendPCB(int argc, char **argv) { //How to handle the running process?
	pcb *tempPCB;
	int errorCode;
  if(argc != 2) {
    invalidArgs(argv[0]);
  } 
  else {
		tempPCB = findPCB(argv[1]);
		if(tempPCB != NULL) {
			if(tempPCB->class == APP) {
				tempPCB->suspended = TRUE;
				errorCode = SUCCESS;
			} else {
				errorCode = SUSP_SYS_PROC;
			}
		} else{
			errorCode = PCB_NOT_FOUND;
		}
		printError(errorCode);
	}
  return LOOP;
}

//RESUMEPCB
//Author: Robert Brown
//Input: argc(int number of token arguments),
//       argv(character arrays of tokens)
//Output: LOOP (1)
// Calls findPCB(), changes PCB to not suspended state
// and reinserts it to correct queue
int resumePCB(int argc, char **argv) { //Handle the running process  ** Does this need to be removed and inserted
	pcb *tempPCB;
	int errorCode;
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
	if((tempPCB = findPCB(argv[1])) != NULL){
		if(tempPCB->class != SYS) {
			tempPCB->suspended = FALSE;
			errorCode = SUCCESS;
		} else {
			errorCode = RES_SYS_PROC;
		}
	}
	else
		errorCode = PCB_NOT_FOUND;
	
	printError(errorCode);
  }
  return LOOP;
}

//SETPCBPRIORITY
//Author: Robert Brown
//Input: argc(int number of token arguments)
//       argv(the char arrays of tokens)
//Parameters: name and priority
// Calls findPCB(), changes PCB priority
// and if it's in the ready queue, 
// removes and reinserts it (to maintain priority queue)
// Otherwise, just changes priority
int setPCBPriority(int argc, char **argv) {
	pcb *tempPCB;
	int errorCode;
	int processVal;
  if(argc != 3) { 
    invalidArgs(argv[0]);
  } else {
		if((tempPCB = findPCB(argv[1])) != NULL) {
			if(tempPCB->class != SYS) {
				processVal =  priorityCheck(argv[2]);
				if(processVal != INVALID_PRIOR) {
					if(tempPCB->state == BLOCKED) {
						tempPCB->priority = processVal;
					} else {
						removePCB(tempPCB);
						tempPCB->priority = processVal;
						insertPCB(tempPCB);
					}
				} else {
					errorCode = INVALID_PRIOR;		
				}
			} else {
				errorCode = CHG_PRI_SYS_PROC;
			}
		} else {
			errorCode = PCB_NOT_FOUND;
		}
		printError(errorCode);
  }
  return LOOP;
}
