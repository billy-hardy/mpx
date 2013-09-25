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
      if(removePCB(process, ready) == SUCCESS) {
	process->state = BLOCKED;
	insertPCB(process, blocked);
      }
      errorCode = SUCCESS;
    } else {
      errorCode = PCB_NOT_FOUND;
    }
    printError(errorCode);
  }
  return LOOP;
}

//Parameters: name
// calls findPCB(), changes PCB to unblocked state
// and reinserts it to correct queue
int unblockPCB(int argc, char **argv) {
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
  }
  return LOOP;
}

//Parameters: name
// calls findPCB(), changes PCB to suspended state
// and reinserts it to correct queue
int suspendPCB(int argc, char **argv) {
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
  }
  return LOOP;
}

//Parameters: name
// calls findPCB(), changes PCB to not suspended state
// and reinserts it to correct queue
int resumePCB(int argc, char **argv) {
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
  }
  return LOOP;
}

//Parameters: name and priority
// calls findPCB(), changes PCB priority
// and removes and reinserts it (to maintain priority queue)
int setPCBPriority(int argc, char **argv) {
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
  }
  return LOOP;
}
