#include "r2.h"
#include "r3.h"

pcb *running;
pcb_queue *ready, *blocked;

void main() {
  sys_init(MODULE_F); //don't forget to change version
  queueInit();
  r3Init();
  r5Init();
  r6Init();
  io_init();
  dispatch();
  queueFree();
  io_tear_down();
  sys_exit();
}

void r5Init() {
  //TODO
}

void r6Init() {
  pcb *commhan;
  context *con_commhan;
  //create commhand and idle processes
  commhan = sys_alloc_mem(sizeof(pcb));
  setupPCB(commhan, "TERMINAL", SYS, 127);
  con_commhan = (context *) commhan->top;
  load_procs(commhan, con_commhan, &commhand);
  loadProgram("IDLE", -128);
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
  sys_free_mem(ready);
  sys_free_mem(blocked);
}

void empty_pcb_queue(pcb_queue *queue) {
  pcb *temp;
  while(queue->count > 0) {
    temp = queue->head;
    removePCB(temp);
    freePCB(temp);
  }
}
