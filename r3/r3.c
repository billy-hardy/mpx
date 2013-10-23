#include "r3.h"

unsigned short ss_save;
unsigned short sp_save;
unsigned short new_ss;
unsigned short new_sp;

char sys_stack[SYS_STACK_SIZE];
pcb * running;
void interrupt dispatch() {
  if(ss_save == NULL) {
    //save stack pointers
    ss_save = _SS;
    sp_save = _SP;
  }
  running = getNextRunning();
  if(running != NULL) {
    removePCB(running);
    running->state = RUNNING;
    new_ss = FP_SEG(running->top);
    new_sp = FP_OFF(running->top) + SYS_STACK_SIZE;
    _SS = new_ss;
    _SP = new_sp;
  } else {
    //restore stack pointers
    _SS = ss_save;
    _SP = sp_save;
    ss_save = NULL;
    sp_save = NULL;
  }
}


void interrupt sys_call() {
  unsigned char* MK_FP(unsigned int SEGMENT, unsigned int OFFSET);
  params *param_ptr = (params*) (MK_FP(_SS, _SP)+sizeof(context));
  //save stack pointers
  ss_save = _SS;
  sp_save = _SP;
  //switch to temp stack
  new_ss = FP_SEG(&sys_stack);
  new_sp = FP_OFF(&sys_stack) + SYS_STACK_SIZE;
  _SS = new_ss;
  _SP = new_sp;
  switch(param_ptr->op_code) {
  case(IDLE):
    running->state = READY;
    insertPCB(running);
    break;
  case(EXIT):
    freePCB(running);
    running = NULL;
    break;
  }
  dispatch();
}

void load_procs(char *name, int class, int priority, void *func) {
  pcb *np;
  context *npc;
  setupPCB(np, name, class, priority);
  npc = (context *) np->top;
  npc->IP = FP_OFF(func);
  npc->CS = FP_SEG(func);
  npc->FLAGS = 0x200;
  npc->DS = _DS;
  npc->ES = _ES;
  insertPCB(np);
}

void r3Init() {
  sys_set_vec(sys_call);
  ss_save = NULL;
  sp_save = NULL;
}

int loadTestProcess(int argc, char *argv[]) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    //TODO: load processes 1-5
  }
  return LOOP;
}

int callDispatch(int argc, char *argv[]) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    
  }
  return 1; //This is temporary
}
