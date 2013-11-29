#include "r3.h"

params *param_ptr;
iocb *trm_iocb, *com_iocb;
//LOAD_PROCS
//Author: Billy Hardy
//Input: PCB : The PCB to be inserted
//       Context: The context structure of the PCB
//       Function: The function 'program' the PCB is to run
//Sets the registers and flags for the individual processes that will be loaded into the MPX system
void load_procs(pcb *np, context *npc, void (*func) (void)) {
  npc->IP = FP_OFF(func);
  npc->CS = FP_SEG(func);
  npc->FLAGS = 0x200;
  npc->DS = _DS;
  npc->ES = _ES;
  insertPCB(np);
}

//LOADTESTPROCESS
//Author: Billy Hardy
//Input: argc (the number of token arguments passed)
//       argv (the char pointers to the argument tokens)
//Output: LOOP (1)
//Places the test procedures into the MPX system.
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

//CALLDISPATCH
//Author: Billy Hardy
//Input: argc, argv (the usual number of args, and arguments themselves)
//Output: LOOP (1)
//Calls the dispatch function the number of arguments is correct
int callDispatch(int argc, char *argv[]) {
  if(argc != 1) {
    invalidArgs(argv[0]);
  } else {
    dispatch();
  }
  return LOOP;
}

//DISPATCH
//Author: Billy Hardy
//Input: void (this is an interrupt)
//Output: void
//Checks if stack pointes have been saved, if not, it saves them.
//Gets the next process to run, or restores the MPX state
//If a PCB is found, a context switch occurs, allowing that process to run
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
    new_sp = FP_OFF(running->top);
    _SS = new_ss;
    _SP = new_sp;
  } else {
    //restore stack pointer
    _SS = ss_save;
    _SP = sp_save;
    ss_save = NULL;
    sp_save = NULL;
  }
}

//SYS_CALL
//Author: Billy Hardy
//Input: void (interrupt)
//Output: void
//Accesses the parameters placed on the stack by sys_req, and determines the interrupt reason
void interrupt sys_call() {
  running->top = MK_FP(_SS, _SP);

  //switch to temp stack
  new_ss = FP_SEG(&sys_stack);
  new_sp = FP_OFF(&sys_stack) + SYS_STACK_SIZE;
  _SS = new_ss;
  _SP = new_sp;

  param_ptr = (params *) (running->top+sizeof(context));
  if(running != NULL) {
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
  }
  dispatch();
}

//R3INIT
//Author: Billy Hardy
//Program to initialize the stack pointers to NULL
void r3Init() {
  sys_set_vec(sys_call);
  ss_save = NULL;
  sp_save = NULL;
}

//IO_SCHEDULER
//Author: Billy Hardy
// TODO: Finish Comment
int io_scheduler() {
  iod *new_iod;
  iocb *device;
  switch(param_ptr->device_id) {
  case(TERMINAL):
    device = trm_iocb;
    break;
  case(COM_PORT):
    device = com_iocb;
    break;
  }
  new_iod = (iod *)sys_alloc_mem(sizeof(iod));
  new_iod->curr = running;
  strcpy(new_iod->name, running->name);
  new_iod->trans_buff = param_ptr->buff_addr;
  new_iod->count = param_ptr->count_addr;
  new_iod->request = param_ptr->op_code;
  insertIOD(device, new_iod);
  if(device->count == 0) {
    char buffer[102];
    int err_code, time_limit, tstart, length = 101;
    switch(param_ptr->device_id) {
    case(TERMINAL):
      switch(new_iod->request) {
      case(READ):
	trm_read();//TODO: parameters
	break;
      case(WRITE):
	trm_write();//TODO: parameters
	break;
      case(CLEAR):
	trm_clear();//TODO: parameters
	break;
      case(GOTOXY):
	trm_gotoxy();//TODO: parameters
	break;
      }
      break;
    case(COM_PORT):
      switch(new_iod->request) {
      case(READ):
	err_code = com_read(buffer, &length);
	time_limit = RD_TIME_LIMIT;
	if(err_code != 0) {
	  printf("error reading!\n");
	  printf("error code = %d\n", err_code);
	}
	break;
      case(WRITE):
	err_code = com_write(buffer, &length);
	time_limit = WR_TIME_LIMIT;
	if(err_code != 0) {
	  printf("error writing!\n");
	  printf("error code = %d\n", err_code);
	}
	break;
      case(CLEAR):
      case(GOTOXY):
	//error state
	break;
      }
      break;
    }
    device->event_flag = 0;
    tstart = time(NULL);
    while(device->event_flag == 0) {
      if((time(NULL)-tstart) > time_limit) {
	printf("TIMEOUT: event flag not set\n");
      }
    }
  }
  running->state = BLOCKED;
}

//INSERTIOD
//Author: Billy Hardy
//TODO: FINISH COMMENT
void insertIOD(iocb *device, iod *to_insert) {
  if(device->count == 0) {
    device->head = device->tail = to_insert;
  } else {
    device->tail->next = to_insert;
    device->tail = to_insert;
  }
  device->count++;
}

//IO_INIT
//Author: Billy Hardy
//TODO: FINISH COMMENT
void io_init() {
  trm_iocb = (iocb *) sys_alloc_mem(sizeof(iocb));
  com_iocb = (iocb *) sys_alloc_mem(sizeof(iocb));
  trm_open(&(trm_iocb->event_flag));
  com_open(&(com_iocb->event_flag), 1200);
}

//IO_TEAR_DOWN
//Author: Billy Hardy
//TODO: FINISH COMMENT
void io_tear_down() {
  trm_close();
  com_close();
  empty_iocb(trm_iocb);
  empty_iocb(com_iocb);
  sys_free_mem(trm_iocb);
  sys_free_mem(com_iocb);
}

//EMPTY_IOCB
//Author: Billy Hardy
//TODO: FINISH COMMENT
void empty_iocb(iocb *to_clear) {
  iod *curr;
  while(to_clear->count > 0) {
    curr = to_clear->head;
    to_clear->head = curr->next;
    (to_clear->count)--;
    sys_free_mem(curr);
  }
}
