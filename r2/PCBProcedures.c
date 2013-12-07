#include "r2.h"
#include "r3.h"

//ALLOCATEPCB
//Author: Billy Hardy
//Input: pcb to be allocated, int stack_size of PCB
//Output: void
//Allocates memory for the passed PCB structure
void allocatePCB(pcb *newPCB, int stack_size) {
  if(newPCB != NULL) {
    newPCB->bottom = (unsigned char *) sys_alloc_mem(stack_size*sizeof(unsigned char));
    newPCB->top = newPCB->bottom + stack_size - sizeof(context);
  }
}

//FREEPCB
//Author: Billy Hardy
//Input: PCB to be freed
//Output: Success Code, or PCB_NOT_FOUND Error
//Frees a PCB structure's allocated memory
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

//SETUPPCB
//Author: Billy Hardy
//Input: PCB (the PCB to be setup)
//       Name (Character pointer to the PCB name)
//       Class (The integer code for the respective PCB class)
//       Priority (the integer priority value of the PCB)
//Output: void
//Sets up the inner values for a PCB structure
void setupPCB(pcb *toSetup, char *name, int class, int priority) {
  if(toSetup != NULL) {
		if(strcmp(name, "TERMINAL")==0) {
			allocatePCB(toSetup, COM_STACK_SIZE);
		} else {
			allocatePCB(toSetup, STACK_SIZE);
		}
    if(paramsGood(name, class, priority)) {
      strcpy(toSetup->name, name);
      toSetup->class = class;
      toSetup->priority = priority;
      toSetup->state = READY;
      toSetup->suspended = FALSE;
      toSetup->memory_size = 0;
      toSetup->load_address = NULL;
      toSetup->exec_address = NULL;
    } else {
      toSetup = NULL;
      printError(INVALID_PARAMS);
    }
  } else {
    printError(NOT_ENOUGH_MEM);
  }
}

//PARMSGOOD
//Author: Billy Hardy
//Input: Name (char array)
//       Class (the integer class value)
//       Priority (the integet priority value)
//Output: False !=0, True 0
//Checks if the parameters for a PCB are of or within the correct restraints
int paramsGood(char name[], int class, int priority) {
  int returnVal;
  returnVal = strlen(name) < 11;
  returnVal = returnVal && (class == SYS || class == APP);
  returnVal = returnVal && (priority < 128 && priority > -129);
  return returnVal;
}

//FINDPCB 
//Author: Billy Hardy
//Input: Name (character pointer the the PCB name to be searched for)
//Output: PCB matching the passed name, or NULL
//Searches for the PCB with the passed name, and returns the PCB, or NULL if not found
pcb *findPCB(char *name) {
  pcb *returnVal = NULL;
  returnVal = (strcmp(name, running->name) == 0) ? running : find(name, ready); //if not running, check ready queue
  returnVal = (returnVal != NULL) ? returnVal : find(name, blocked);   //if NULL, check blocked queue
  return returnVal;
}

//FIND
//Author: Billy Hardy
//Input: Name, character pointer to a name to be found
//       pcb_queue, the queue to be searched
//Output: The PCB if found, NULL otherwise
//Helper function to findpcb.  Performs the actual searching of the queue.
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

//Authors:  Billy & Robert
//This was rewritten probably 10 times.  It's a joint effort at this point
//Input: PCB(the PCB to be inserted into queue)
//Output: LOOP (1)
//Inserts the passed PCB into the appropriate position in the PCBs respective queue.
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
    if(ready->head == NULL && ready->tail == NULL) { //Queue is empty
      ready->head = toInsert;
      ready->tail = toInsert; //May want this to just start out NULL
      ready->count++;
      return SUCCESS; //Bad I know
    } else {
      curr = ready->head;
      temp = NULL;
      while(curr !=NULL && curr->priority >= toInsert->priority) {
	curr = curr->next;
      }
      if(curr == NULL) { //passed Tail, make the new Tail
	if(ready->tail == ready->head){
	  temp = ready->head;
	  temp->next = toInsert;
	  toInsert->prev = temp;
	  ready->tail = toInsert;
	  ready->count++;	
	  return SUCCESS;
	} else {
	  ready->tail->next = toInsert;
	  toInsert->prev = ready->tail;
	  ready->tail = toInsert;
	  ready->count++;
	  return SUCCESS;
	}
      }
      if(curr == ready->head){ //Make new Head
	if(ready->tail ==ready->head) {
	  temp = ready->head;
	  toInsert->next = temp;
	  temp->prev = toInsert;
	  ready->head = toInsert;
	  ready->count++;
	  return SUCCESS;
	} else {
	  toInsert->next = ready->head;
	  ready->head->prev = toInsert;
	  ready->head = toInsert;
	  ready->count++;
	  return SUCCESS;
	}
      }  else{
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

//Author: Robert Brown
//Input: PCB (the PCB to be removed from queue)
//Output: SUCCESS (1)
//Removes the passed PCB from its queue.
int removePCB(pcb *toRemove){
  pcb *temp;
  pcb_queue *queue;
	
  if(toRemove->state == BLOCKED) {
    queue = blocked;
  } else {
    queue = ready;
  }
  if(toRemove == queue->head && toRemove == queue->tail){
    queue->head = NULL;
    queue->tail = NULL;
    queue->count--;
  } else if(toRemove == queue->head){
    queue->head = queue->head->next;
    queue->head->prev = NULL;
    queue->count--;
  } else if(toRemove == queue->tail){
    queue->tail = queue->tail->prev;
    queue->tail->next = NULL;
    queue->count--;
  } else{
    toRemove->prev->next = toRemove->next;
    toRemove->next->prev = toRemove->prev;
    queue->count--;		
  }	
  return SUCCESS;
}

//Authors:  Robert Brown, Billy Hardy
//Input: Respective Error Code
//Output: void
//Displays the error information for a passed Error Code
//This is used to avoid duplication with all the error handling messages.
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
	case RES_SYS_PROC:
		strcpy(buffer, "\nUnable to resume system procedures.\n\n");
		break;
  case ERR_SUP_NAMLNG: //-114
    strcpy(buffer, "\nName too long for buffer.\n\n");
    break;
	case CHG_PRI_SYS_PROC:
		strcpy(buffer, "\nUnable to change priority of system procedures.\n\n");
		break;
  case TERM_SYS_PROC:
		strcpy(buffer, "\nUnable to terminate system procedures.\n\n");
		break;
	/*case INV_COM_ALIAS:
		strcpy(buffer, "\nGiven command to alias is invalid.\n\n");
		break;
  case ALIAS_TOO_LONG:
		strcpy(buffer, "\nGiven alias is too long. Max size is 20 characters.\n\n");
		break;*/
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

//Author: Billy Hardy
//Input: void
//Output: PCB(the next running)
//Gets the next PCB in queue to be 'ran'
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

void unblockPCB(pcb *toUnblock) {
  removePCB(toUnblock);
  toUnblock->state = READY;
  insertPCB(toUnblock);
}

