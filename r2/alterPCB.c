#include "r2.h"


//Parameters: name
// calls findPCB(), changes PCB to blocked state
// and reinserts it to correct queue
int blockPCB(int argc, char **argv) {
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff 
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
