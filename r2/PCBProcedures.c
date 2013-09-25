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

pcb *setupPCB(char name[], int class, int priority) {
  pcb *toSetup;
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
  return toSetup;
}

int paramsGood(char name[], int class, int priority) {
  int returnVal;
  returnVal = (strlen(name) < 11 && strlen(name) > 7);
  returnVal &= (class == SYS || class == APP);
  returnVal &= (priority < 128 && priority > -129);
  return returnVal;
}

//believe it works
pcb *findPCB(char *name) {
  pcb *returnVal = NULL;
  returnVal = (strcmp(name, running->name) == 0) ? running : find(name, &ready); //if not running, check ready queue
  returnVal = (returnVal != NULL) ? returnVal : find(name, &blocked);   //if NULL, check blocked queue
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

//believe it works
//TODO: helper function for insertion?
int insertPCB(pcb *toInsert) {
  int returnVal;
  pcb *curr;
  pcb_queue *queue;
  if(toInsert->state == BLOCKED) {
    queue = blocked;
    queue->tail->prev = toInsert;
    toInsert->next = queue->tail;
    queue->tail = toInsert;
    toInsert->prev = NULL;
    queue->count++;
    returnVal = SUCCESS;
  } else if(toInsert->state == READY) {
    queue = ready;
    curr = queue->tail;
    while(curr->priority < toInsert->priority) {
      curr = curr->next;
    }
    if(curr == NULL) {
      queue->head->next = toInsert;
      toInsert->prev = queue->head;
      queue->head = toInsert;
      toInsert->next = NULL;
    } else {
      curr->prev->next = toInsert;
      toInsert->prev = curr->prev;
      curr->prev = toInsert;
      toInsert->next = curr;
    }
    returnVal = SUCCESS;
  } else {
    returnVal = NO_SUCH_MODE;
  }
  return returnVal;
}

//Hasn't been compiled
int removePCB(pcb *toRemove) {
  int returnVal;
  pcb *curr;
  pcb_queue *queue;
  if(toRemove->state == READY) {
    queue = ready;
  } else {
    queue = blocked;
  }
  curr = queue->tail;
  for(i=0; i<queue->count; i++) {
    if(curr == toRemove) {
      break;
    } else {
      curr = curr->next;
    }
  }
  if(curr == NULL) {
    returnVal = PCB_NOT_FOUND;
  } else if(i == queue->count) {
    queue->head = curr->prev;
    queue->head->next = NULL;
    curr-next = curr->prev = NULL;
    returnVal = SUCCESS;
  } else if(i == 0) {
    queue->tail = curr->next;
    queue->tail->prev = NULL;
    curr->next = curr->prev = NULL;
    returnVal = SUCCESS;
  } else {
    curr->next->prev = curr->prev;
    curr->prev->next = curr->next;
    curr->next = curr->prev = NULL;
    returnVal = SUCCESS;
  }
  return returnVal;
}

void printError(int errorCode) {
  char buffer[100];
  int bufferSize;
  if(errorCode == PCB_NOT_FOUND) {
    strcpy(buffer, "\nProcess specified not found.\n\n");
  }
  bufferSize = strlen(buffer);
  sys_req(WRITE, TERMINAL, buffer, &bufferSize);
}
