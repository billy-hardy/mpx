#include "r2.h"

extern pcb *running;
extern pcb_queue ready;
extern pcb_queue blocked;

//Parameters: name
// Must display name, class, state, suspended status and priority
// Displays error if PCB doesn't exist
int showPCB(int argc, char **argv) {
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
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
