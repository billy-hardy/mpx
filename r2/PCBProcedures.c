#include "r2.h"

//globals
pcb *running;
pcb_queue *ready, *blocked;

void allocatePCB(pcb *newPCB) {
  newPCB = (pcb *) sys_alloc_mem(sizeof(pcb));
  if(newPCB != NULL) {
    newPCB->name[0] = '\0';
    newPCB->class = SYS;
    newPCB->priority = 0;
    newPCB->state = READY;
    newPCB->suspended = FALSE;
    newPCB->next = NULL;
    newPCB->prev = NULL;
    newPCB->top = NULL;
    newPCB->bottom = NULL;
    newPCB->memory_size = 0;
    newPCB->load_address = NULL;
    newPCB->exec_address = NULL;
  }
}

//believe it works
int freePCB(pcb *toFree) {
  int returnVal;
  if(toFree == NULL) {
    returnVal = PCB_NOT_FOUND;
  } else {
    sys_free_mem(toFree->bottom);
    sys_free_mem(toFree);
    returnVal = SUCCESS;
  }
  return returnVal;
}

void setupPCB(pcb *toSetup, char name[], int class, int priority) {
  int errorCode;
  allocatePCB(toSetup);
  if(toSetup != NULL) {
    if(paramsGood(name, class, priority)) {
      strcpy(toSetup->name, name);
      toSetup->class = class;
      toSetup->priority = priority;
      toSetup->state = READY;
      toSetup->suspended = FALSE;
      toSetup->bottom = (unsigned char *) sys_alloc_mem(STACK_SIZE*sizeof(unsigned char));
      toSetup->top = toSetup->bottom;
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
  returnVal = (strlen(name) < 11 && strlen(name) > 7);
  returnVal = returnVal && (class == SYS || class == APP);
  returnVal = returnVal && (priority < 128 && priority > -129);
  return returnVal;
}

//believe it works
pcb *findPCB(char *name) {
  pcb *returnVal = NULL;
  returnVal = (strcmp(name, running->name) == 0) ? running : find(name, ready); //if not running, check ready queue
  returnVal = (returnVal != NULL) ? returnVal : find(name, blocked);   //if NULL, check blocked queue
  return returnVal;
}

//believe it works
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

int removePCB(pcb *toRemove){
	pcb *temp;
	pcb_queue *queue;
	
	int returnVal = NULL;

		if(toRemove->state == BLOCKED) queue = blocked;	
			else queue = ready;
		if(queue->head == queue-> tail){
			queue->head = queue->tail = NULL;
		}
		else if(queue->head == temp){
			queue->head = temp->prev;
		}
		else if(queue->tail == temp){
			queue->tail = temp->next;
		}
		else{
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
		}	
}

void printError(int errorCode) {  //This just currently prints out ERROR, not SUCCESS ... fix this.
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
		default:
			buffer[0] = '\0';
	}
	bufferSize = strlen(buffer);
	sys_req(WRITE, TERMINAL, buffer, &bufferSize);
}
