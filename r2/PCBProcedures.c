#include "r2.h"

//globals

void allocatePCB(pcb *newPCB) {
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
		toFree->top = NULL;
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
		default:
			buffer[0] = '\0';
	}
	bufferSize = strlen(buffer);
	sys_req(WRITE, TERMINAL, buffer, &bufferSize);
}







