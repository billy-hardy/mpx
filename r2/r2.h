#ifndef R2_H
#define R2_H

#include <stdio.h>
#include "r1.h"

#define LOOP 1 //loop as before
#define TRUE 1
#define FALSE 0
#define STACK_SIZE 1024
#define COM_STACK_SIZE 4*STACK_SIZE

//constants
#define SYS 0 //system process
#define APP 1 //application process
#define INV_CLASS -1
#define BLOCKED -1
#define READY 0
#define RUNNING 1

typedef struct pcb {
  char name[11];
  int class;      //This name may need to be changed(I think class is reserved)
  int priority;
  int state;      //running, ready or blocked
  int suspended;  //suspended or not
  struct pcb *next, *prev;
  //stack
  unsigned char *top; 
  unsigned char *bottom;
  //memory information
  int memory_size;
  unsigned char *load_address;
  unsigned char *exec_address;
} pcb;

typedef struct {
  int count;
  pcb *head, *tail;
} pcb_queue;

//error codes
#define SUCCESS 0
#define PCB_NOT_FOUND -4
#define NOT_ENOUGH_MEM -2
#define INVALID_PARAMS -3
#define DUP_PCB -5
#define INVALID_PRIOR -6
#define INVALID_CLASS -7
#define SUSP_SYS_PROC -8
#define RES_SYS_PROC -9
#define CHG_PRI_SYS_PROC -10
#define TERM_SYS_PROC -11
#define INV_COM_ALIAS -12
#define ALIAS_TOO_LONG -13

//function prototypes
//definePCB.c
int createPCB(int argc, char **argv);
int deletePCB(int argc, char **argv);
void queueInit();
int priorityCheck(char *);
int parseClass(char *);
//alterPCB.c
int blockPCB(int argc, char **argv);
int suspendPCB(int argc, char **argv);
int resumePCB(int argc, char **argv);
int setPCBPriority(int argc, char **argv);
//dispPCB.c
int showPCB(int argc, char **argv);
int showReady(int argc, char **argv);
int showBlocked(int argc, char **argv);
int showAll(int argc, char **argv);
void printTable();
void printPCB(pcb *);
void showQueue(pcb_queue *);
//PCBProcedures.c
void allocatePCB(pcb *newPCB, int);
int freePCB(pcb *toFree);
void setupPCB(pcb *toSetup, char *name, int class, int priority);
int paramsGood(char name[], int class, int priority);
pcb *findPCB(char *name);
pcb *find(char *name, pcb_queue *queue);
int insertPCB(pcb *toInsert);
int removePCB(pcb *toRemove);
void printError(int errorCode);
pcb *getNextRunning();
void unblockPCB(pcb *toUnblock);
void empty_queue(pcb_queue *);

extern pcb *running;
extern pcb_queue *ready;
extern pcb_queue *blocked;

#endif
