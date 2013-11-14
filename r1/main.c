#include "r2.h"
#include "r3.h"

pcb *running;
pcb_queue *ready, *blocked;

void main() {
  pcb *commhan, *idle;
  context *con_commhan, *con_idle;
  sys_init(MODULE_R3); //don't forget to change version
  r3Init();
  queueInit();
  //create commhand and idle processes
  commhan = sys_alloc_mem(sizeof(pcb));
  idle = sys_alloc_mem(sizeof(pcb));
  setupPCB(commhan, "TERMINAL", SYS, 127);
  setupPCB(idle, "IDLE", SYS, -128);
  con_commhan = (context *) commhan->top;
  con_idle = (context *) idle->top;
  load_procs(commhan, con_commhan, &commhand);
  load_procs(idle, con_idle, &IDLE);
  dispatch();
  queueFree();
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
