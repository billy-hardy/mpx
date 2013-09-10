#ifndef R2_H
#define R2_H

#include <stdio.h>

//constants
#define SYS 0 //system process
#define APP 1 //application process

typedef struct {
  char name[11];
  int class;
  int priority;
  //next two might be unnecessary
  int state;      //running, ready or blocked
  int suspended;  //suspended or not
  //do we want descriptors?
  PCB *next, *prev;
  //stack
  unsigned char *top; //TODO: this is eff-ed up.
  //memory information
  int memory_size;
  unsigned char *load_address;
  unsigned char *exec_address;
} PCB;

typedef struct {
  int count;
  PCB *head, *tail;
} pcb_queue;

//function prototypes
//definePCB.c
int createPCB(int argc, char **argv);
int deletePCB(int argc, char **argv);
//alterPCB.c
int blockPCB(int argc, char **argv);
int unblockPCB(int argc, char **argv);
int suspendPCB(int argc, char **argv);
int resumePCB(int argc, char **argv);
int setPCBPriority(int argc, char **argv);
//dispPCB.c
int showPCB(int argc, char **argv);
int showReady(int argc, char **argv);
int showBlocked(int argc, char **argv);
int showAll(int argc, char **argv);
//PCBProcedures.c
PCB *allocatePCB();
int freePCB(PCB *toFree);
PCB *setupPCB(char name[], int class, int priority);
PCB *findPCB(char *PCBname);
int insertPCB(PCB *toInsert);
int removePCB(PCB *toRemove);

#endif
