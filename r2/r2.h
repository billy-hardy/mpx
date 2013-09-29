#ifndef R2_H
#define R2_H

#include <stdio.h>
#include "../r1/r1.h"

#define LOOP 1 //loop as before
#define TRUE 1
#define FALSE 0
#define STACK_SIZE 1024

//constants
#define SYS 0 //system process
#define APP 1 //application process
#define BLOCKED -1
#define READY 0
#define RUNNING 1

struct pcbstruct {
  char name[11];
  int class;      //This name may need to be changed(I think class is reserved)
  int priority;
  int state;      //running, ready or blocked
  int suspended;  //suspended or not
  struct pcbstruct *next, *prev;
  //stack
  unsigned char *top; 
  unsigned char *bottom;
  //memory information
  int memory_size;
  unsigned char *load_address;
  unsigned char *exec_address;
};

typedef struct pcbstruct pcb;

typedef struct {
  int count;
  pcb *head, *tail;
} pcb_queue;

//modes
#define FIFO 1
#define PRIORITY -1

//error codes
#define SUCCESS 0
#define NO_SUCH_MODE -1
#define PCB_NOT_FOUND -2
#define NOT_ENOUGH_MEM -3
#define INVALID_PARAMS -4

//function prototypes
//definePCB.c
int createPCB(int argc, char **argv);
int deletePCB(int argc, char **argv);
int integerCheck(char *);
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
void printPCB(pcb *);
void showQueue(pcb_queue *);
//PCBProcedures.c
pcb *allocatePCB();
int freePCB(pcb *toFree);
pcb *setupPCB(char name[], int class, int priority);
int paramsGood(char name[], int class, int priority);
pcb *findPCB(char *name);
pcb *find(char *name, pcb_queue *queue);
int insertPCB(pcb *toInsert);
int removePCB(pcb *toRemove);
void printError(int errorCode);

#endif
