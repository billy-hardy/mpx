#include "r3.h"

params *param_ptr;
iocb *trm_iocb, *com_iocb;
char sys_stack[SYS_STACK_SIZE];
unsigned short ss_save;
unsigned short sp_save;
unsigned short new_ss;
unsigned short new_sp;
unsigned short cop_ss;
unsigned short cop_sp;

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

void process_io(iod *to_process, int device) {
	if(to_process->request == READ) {
		if (device == TERMINAL) {
			trm_read(to_process->trans_buff, to_process->count);
		} else if (device == COM_PORT) {
			com_read(to_process->trans_buff, to_process->count);
		}
	} else if(to_process->request == WRITE) { //Writing
		if (device == TERMINAL) {
			trm_write(to_process->trans_buff, to_process->count);
		} else if (device == COM_PORT) {
			com_write(to_process->trans_buff, to_process->count);
		}
	} else if(to_process->request == CLEAR) { //clear
		trm_clear();
	} else if(to_process->request == GOTOXY) {
		trm_gotoxy(0,0);
	}
}


//SYS_CALL
//Author: Billy Hardy
//Input: void (interrupt)
//Output: void
//Accesses the parameters placed on the stack by sys_req, and determines the interrupt reason
void interrupt sys_call() {
	static iod *temp_iod;
	cop_ss = _SS;
	cop_sp = _SP;
	running->top = MK_FP(cop_ss, cop_sp);
  param_ptr = (params *) (running->top + sizeof(context));
  //switch to temp stack
  new_ss = FP_SEG(sys_stack);
  new_sp = FP_OFF(sys_stack);
	new_sp += SYS_STACK_SIZE;
  _SS = new_ss;
  _SP = new_sp;
  trm_getc();
  if(com_iocb->event_flag == 1) {
    com_iocb->event_flag = 0;
    temp_iod = com_iocb->head;
		com_iocb->head = com_iocb->head->next;
		com_iocb->count--;
		unblockPCB(temp_iod->curr);
		sys_free_mem(temp_iod);
		temp_iod = temp_iod->next;
		if(temp_iod != NULL) {
			process_io(temp_iod, COM_PORT);
		}
		/*if(temp_iod != NULL) {
			switch(temp_iod->request) {
			case(READ):
				com_read(temp_iod->trans_buff, temp_iod->count);
				break;
			case(WRITE):
				com_write(temp_iod->trans_buff, temp_iod->count);
				break;
			}
		}*/
  }
	if(trm_iocb->event_flag == 1) {
    trm_iocb->event_flag = 0;
    temp_iod = trm_iocb->head;
		trm_iocb->head = trm_iocb->head->next;
		trm_iocb->count--;
	       //find based on name
	       //if found unblock
		unblockPCB(temp_iod->curr);
		sys_free_mem(temp_iod);
	       //	temp_iod = temp_iod->next;
		if(trm_iocb->head != NULL) {
			process_io(trm_iocb->head, TERMINAL);
		}
		/*if(temp_iod != NULL) {
			switch(param_ptr->op_code) {
			case(READ):
				trm_read(temp_iod->trans_buff, temp_iod->count);
				break;
			case(WRITE):
				trm_write(temp_iod->trans_buff, temp_iod->count);
				break;
			case(CLEAR):
				trm_clear();
				break;
			case(GOTOXY):
				trm_gotoxy(0,0);
				break;
			}
		}*/
	}
	switch(param_ptr->op_code) {
	case(IDLE):
		running->state = READY;
		insertPCB(running);
		break;
	case(EXIT):
		freePCB(running);
		running = NULL;
		break;
	default:
		io_scheduler();
		break;
	}
  dispatch();
}

/*
void interrupt sys_call() {
  static params *param_ptr;
  running->top = MK_FP(_SS, _SP);
  //switch to temp stack
  new_ss = FP_SEG(sys_stack);
  new_sp = FP_OFF(sys_stack) + SYS_STACK_SIZE;
  _SS = new_ss;
  _SP = new_sp;
  
  param_ptr = (params*) (running->top+sizeof(context));
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
}*/


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
void io_scheduler() {
  static iocb *device;
	static iod *temp_iod;
  switch(param_ptr->device_id) {
  case(TERMINAL):
    device = trm_iocb;
    break;
  case(COM_PORT):
    device = com_iocb;
    break;
  }
  temp_iod = (iod *)sys_alloc_mem(sizeof(iod));
  temp_iod->curr = running;
  strcpy(temp_iod->name, running->name);
  temp_iod->trans_buff = param_ptr->buf_p;
  temp_iod->count = param_ptr->count_p;
  temp_iod->request = param_ptr->op_code;
	insertIOD(device, temp_iod);
  if(device->count == 1) {
    process_io(temp_iod, param_ptr->device_id);
    //int time_limit, tstart;
    /*switch(param_ptr->device_id) {
    case(TERMINAL):
			switch(temp_iod->request) {
      case(READ):
				trm_read(temp_iod->trans_buff, temp_iod->count);
				break;
      case(WRITE):
				trm_write(temp_iod->trans_buff, temp_iod->count);
				break;
      case(CLEAR):
				trm_clear();
				break;
      case(GOTOXY):
				trm_gotoxy(0,0);
				break;
      }
      break;
    case(COM_PORT):
			switch(temp_iod->request) {
      case(READ):
				com_read(temp_iod->trans_buff, temp_iod->count);
				break;
      case(WRITE):
				com_write(temp_iod->trans_buff, temp_iod->count);
				break;
      }
      break;
    }*/
  } 
  running->state = BLOCKED;
	insertPCB(running);
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
		to_insert->next = NULL;
  }
  device->count++;
}

//IO_INIT
//Author: Billy Hardy
//TODO: allocates iocbs for ports and opens ports
void io_init() {
  trm_iocb = (iocb *) sys_alloc_mem(sizeof(iocb));
  com_iocb = (iocb *) sys_alloc_mem(sizeof(iocb));
  trm_open(&(trm_iocb->event_flag));
  com_open(&(com_iocb->event_flag), 1200);
	//trm_iocb->event_flag = 1;
	//com_iocb->event_flag = 1;
}

//IO_TEAR_DOWN
//Author: Billy Hardy
//Closes all ports and empties devices' iocbs.
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
//TODO: empties the iocb and frees the memory
//        associated with each iod.
void empty_iocb(iocb *to_clear) {
  iod *curr;
  while(to_clear->count > 0) {
    curr = to_clear->head;
    to_clear->head = curr->next;
    (to_clear->count)--;
    sys_free_mem(curr);
  }
}
