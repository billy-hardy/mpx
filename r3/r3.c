#include "r3.h"

void load_procs(pcb *np, context *npc, void (*func) (void)) {
  npc->IP = FP_OFF(func);
  npc->CS = FP_SEG(func);
  npc->FLAGS = 0x200;
  npc->DS = _DS;
  npc->ES = _ES;
  insertPCB(np);
}


int loadTestProcess(int argc, char *argv[]) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    pcb *processes[5];
    context *contexts[5];
    void (*functions[5]) (void);
    char names[5][20];
    int classes[5];
    int priorities[5];
    int i;
    functions[0] = &test1_R3; strcpy(names[0], "test1"); classes[0] = APP; priorities[0] = 0;
    functions[1] = &test2_R3; strcpy(names[1], "test2"); classes[1] = SYS; priorities[1] = 8;
    functions[2] = &test3_R3; strcpy(names[2], "test3"); classes[2] = APP; priorities[2] = 0;
    functions[3] = &test4_R3; strcpy(names[3], "test4"); classes[3] = APP; priorities[3] = 0;
    functions[4] = &test5_R3; strcpy(names[4], "test5"); classes[4] = SYS; priorities[4] = 0;
		
    for(i=0; i<5; i++) {
      if(findPCB(names[i]) == NULL) {
	processes[i] = sys_alloc_mem(sizeof(pcb));
	setupPCB(processes[i], names[i], classes[i], priorities[i]);
	contexts[i] = (context *) processes[i]->top;
	load_procs(processes[i], contexts[i], functions[i]);
      } else {
	printError(DUP_PCB);
      }
    }
  }
  return LOOP;
}

int callDispatch(int argc, char *argv[]) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    dispatch();
  }
  return LOOP;
}
