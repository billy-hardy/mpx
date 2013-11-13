#include "r2.h"
#include "r3.h"

char sys_stack[SYS_STACK_SIZE];
unsigned short ss_save;
unsigned short sp_save;
unsigned short new_ss;
unsigned short new_sp;

void allocatePCB(pcb *newPCB) {
  if(newPCB != NULL) {
		newPCB->bottom = (unsigned char *) sys_alloc_mem(STACK_SIZE*sizeof(unsigned char));
		newPCB->top = newPCB->bottom + STACK_SIZE - sizeof(context);
  }
}

int freePCB(pcb *toFree) {
  int returnVal;
  if(toFree == NULL) {
    returnVal = PCB_NOT_FOUND;
  } else {
    sys_free_mem(toFree->bottom);
    toFree->top = NULL;
	sys_free_mem(toFree->load_address);
    sys_free_mem(toFree);
    returnVal = SUCCESS;
  }
  return returnVal;
}

void setupPCB(pcb *toSetup, char *name, int class, int priority) {
  int errorCode;
  allocatePCB(toSetup);
  if(toSetup != NULL) {
    if(paramsGood(name, class, priority)) {
      strcpy(toSetup->name, name);
      toSetup->class = class;
      toSetup->priority = priority;
      toSetup->state = READY;
      toSetup->suspended = FALSE;
      toSetup->memory_size = 0;
      toSetup->load_address = NULL;
      toSetup->exec_address = NULL;
      errorCode = SUCCESS;
    } else {
      toSetup = NULL;
      errorCode = INVALID_PARAMS;
    }
  } else {
    errorCode = NOT_ENOUGH_MEM;
  }
  printError(errorCode);
}

int paramsGood(char name[], int class, int priority) {
  int returnVal;
  returnVal = strlen(name) < 11;
  returnVal = returnVal && (class == SYS || class == APP);
  returnVal = returnVal && (priority < 128 && priority > -129);
  return returnVal;
}


pcb *findPCB(char *name) {
  pcb *returnVal = NULL;
  returnVal = (strcmp(name, running->name) == 0) ? running : find(name, ready); //if not running, check ready queue
  returnVal = (returnVal != NULL) ? returnVal : find(name, blocked);   //if NULL, check blocked queue
  return returnVal;
}


pcb *find(char *name, pcb_queue *queue) {
  int i;
  pcb *curr = queue->head, *returnVal = NULL;
  for(i=0; i<queue->count; i++) {
    if(strcmp(curr->name, name) == 0) {  //found it
      returnVal = curr;
      break;
    } else {
      curr = curr->next;  //didn't, go to next
    }
  }
  return returnVal;
}


int insertPCB(pcb *toInsert) {
  pcb *curr, *temp;
  toInsert->next = NULL;
  toInsert->prev = NULL;
  if(toInsert->state == BLOCKED) {
    if(blocked->head == NULL) { //queue is empty
      blocked->tail = toInsert;
      blocked->head = toInsert;
      toInsert->next = NULL;
      toInsert->prev = NULL;
    } else { //insert in the tail
      blocked->tail->next = toInsert;
      toInsert->prev = blocked->tail;
      toInsert->next = NULL;
      blocked->tail = toInsert;
    }
    blocked->count++;
  } else if(toInsert->state == READY) {
    if(ready->head == NULL && ready->tail == NULL){ //Queue is empty
      ready->head = toInsert;
      ready->tail = toInsert; //May want this to just start out NULL
      ready->count++;
      return SUCCESS; //Bad I know
    }
    else{
      curr = ready->head;
      temp = NULL;
      while(curr !=NULL && curr->priority >= toInsert->priority)
	curr = curr->next;
					
      if(curr == NULL){ //passed Tail, make the new Tail
	if(ready->tail == ready->head){
	  temp = ready->head;
	  temp->next = toInsert;
	  toInsert->prev = temp;
	  ready->tail = toInsert;
	  ready->count++;	
	  return SUCCESS;
	}
	else{
	  ready->tail->next = toInsert;
	  toInsert->prev = ready->tail;
	  ready->tail = toInsert;
	  ready->count++;
	  return SUCCESS;
	}
      }
      if(curr == ready->head){ //Make new Head
	if(ready->tail ==ready->head){
	  temp = ready->head;
	  toInsert->next = temp;
	  temp->prev = toInsert;
	  ready->head = toInsert;
	  ready->count++;
	  return SUCCESS;
	}
	else{
	  toInsert->next = ready->head;
	  ready->head->prev = toInsert;
	  ready->head = toInsert;
	  ready->count++;
	  return SUCCESS;
	}
      }
      else{
	toInsert->next = curr;
	toInsert->prev = curr->prev;
	curr->prev->next = toInsert;
	curr->prev = toInsert;
	ready->count++;
	return SUCCESS;
      }
    }			
  }
  return NULL;
}


int removePCB(pcb *toRemove){
  pcb *temp;
  pcb_queue *queue;
	
  if(toRemove->state == BLOCKED)
    queue = blocked;
  else
    queue = ready;
	
  if(toRemove == queue->head && toRemove == queue->tail){
    queue->head = NULL;
    queue->tail = NULL;
    queue->count--;
  }
  else if(toRemove == queue->head){
    queue->head = queue->head->next;
    queue->head->prev = NULL;
    queue->count--;
  }
  else if(toRemove == queue->tail){
    queue->tail = queue->tail->prev;
    queue->tail->next = NULL;
    queue->count--;
  }
  else{
    toRemove->prev->next = toRemove->next;
    toRemove->next->prev = toRemove->prev;
    queue->count--;		
  }	
  return SUCCESS;
}

void printError(int errorCode) {  
  char buffer[256];
  int bufferSize;
  switch(errorCode) {
  case PCB_NOT_FOUND:
    strcpy(buffer, "\nProcess specified not found.\n\n");
    break;
  case INVALID_PARAMS:
    strcpy(buffer, "\nParameters are incorrect.\n\n");
    break;
  case NOT_ENOUGH_MEM:
    strcpy(buffer, "\nNot enough memory for system to operate. Please reboot.\n\n");
    break;
  case INVALID_PRIOR:
    strcpy(buffer, "\nInvalid Priority.  Must be between -128 and 127.\n\n");
    break;
  case INVALID_CLASS:
    strcpy(buffer, "\nInvalid Class Value.  Must be sys or app.\n\n");
    break;
  case DUP_PCB:
    strcpy(buffer, "\nPCB Name already exists.  Names must be unique!\n\n");
    break;
  case SUSP_SYS_PROC:
	strcpy(buffer, "\nUnable to suspend system procedures.\n\n");
	break;
  case ERR_SUP_NAMLNG: //-114
    strcpy(buffer, "\nName too long for buffer.\n\n");
    break;
  case ERR_SUP_FILNFD: //-117
    strcpy(buffer, "\nFile not found!\n\n");
    break;
  case ERR_SUP_FILINV: //-118
    strcpy(buffer, "\nInvalid File Type!\n\n");
    break;
  case ERR_SUP_PROGSZ: //-119
    strcpy(buffer, "\nProgram Size Error!\n\n");
    break;
  case ERR_SUP_LDADDR: //-120
    strcpy(buffer, "\nInvalid Program Load Address!\n\n");
    break;
  case ERR_SUP_LDFAIL: //-116
    strcpy(buffer, "\nError: Loading Program Failed\n\n");
    break;
  default:
    buffer[0] = '\0';
    break;
  }
  bufferSize = strlen(buffer);
  sys_req(WRITE, TERMINAL, buffer, &bufferSize);
}

pcb *getNextRunning() {
  pcb *returnVal, *temp;
  returnVal = NULL;
  if(ready->head != NULL) {
    temp = ready->head;
    while(temp != NULL) {
      if(!temp->suspended) {
	returnVal = temp;
	break;
      }
      temp = temp->next;
    }
  }
  return returnVal;
}

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
    //restore stack pointers
    _SS = ss_save;
    _SP = sp_save;
    ss_save = NULL;
    sp_save = NULL;
  }
}


void interrupt sys_call() {
  //char *stack;
   static params *param_ptr;
  running->top = MK_FP(_SS, _SP);
  

  //switch to temp stack
  new_ss = FP_SEG(&sys_stack);
  new_sp = FP_OFF(&sys_stack) + SYS_STACK_SIZE;
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
}

void r3Init() {
  sys_set_vec(sys_call);
  ss_save = NULL;
  sp_save = NULL;
}

