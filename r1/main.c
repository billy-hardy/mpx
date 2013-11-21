#include "r2.h"
#include "r3.h"

pcb *running;
pcb_queue *ready, *blocked;

void main() {
  pcb *commhan;
  context *con_commhan;
  sys_init(MODULE_R3); //don't forget to change version
  r3Init();
  queueInit();
  //create commhand and idle processes
  commhan = sys_alloc_mem(sizeof(pcb));
  setupPCB(commhan, "TERMINAL", SYS, 127);
  con_commhan = (context *) commhan->top;
  load_procs(commhan, con_commhan, &commhand);
	loadProgram("IDLE", -128);
  dispatch();
  printf("Goodbye\n");
  delay(750);
  sys_exit();
}

//Initializes the Queue
//Author Billy, Last Edited, Robert
void queueInit() {
  ready = (pcb_queue *) sys_alloc_mem(sizeof(pcb_queue));
  ready->head = NULL;
  ready->tail = NULL;
  ready->count = 0;
  blocked = (pcb_queue *) sys_alloc_mem(sizeof(pcb_queue));
  blocked->head = NULL;
  blocked->tail = NULL;
  blocked->count = 0;
}

//Frees leftover PCBs on Exit
//Author Billy
void queueFree() {
  pcb *temp;
  while(ready->count > 0) {
    temp = ready->head;
    removePCB(temp);
    freePCB(temp);
  }
  sys_free_mem(ready);
  while(blocked->count > 0) {
    temp = blocked->head;
    removePCB(temp);
    freePCB(temp);
  }
  sys_free_mem(blocked);
}
