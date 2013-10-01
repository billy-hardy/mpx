#include "r2.h"

extern pcb *running;
extern pcb_queue *ready;
extern pcb_queue *blocked;

//Parameters: name, class and priority
// name must be unique
// class and priority must be valid

//THIS HAS NOT BEEN COMPILED OR TESTED YET
int createPCB(int argc, char **argv) {
  pcb *tempPCB;
  int classVal, priorityVal;
  int bufferSize;
  char messageBuffer[60];
  if(argc != 4) {
    invalidArgs(argv[0]);
  }
  else {
		if(findPCB(argv[1]) == NULL){
			if((classVal= parseClass(argv[2])) != INV_CLASS){
				if((priorityVal = priorityCheck(argv[3])) != INV_PRIORITY){
					if (paramsGood(argv[1], classVal, priorityVal)){
						setupPCB(tempPCB, argv[1], classVal, priorityVal); 
						if(tempPCB != NULL)
							insertPCB(tempPCB);	
					//********************Are there error codes for these somewhere.....
					} else {
						//Parameters invalid (strlen, out of bounds ints)
						strcpy(messageBuffer, "Invalid Parameters!  Use \"help\" for more information.\n");
						bufferSize = strlen(messageBuffer);
						sys_req(WRITE, TERMINAL, messageBuffer, &bufferSize);
					}
				}
				else{
				//invalid Priority Value (not an int)
					strcpy(messageBuffer, "Invalid Priority Value!  Use \"help\" for more information.\n");
					bufferSize = strlen(messageBuffer);
					sys_req(WRITE, TERMINAL, messageBuffer, &bufferSize);
				}			
			}
			else{
		//invalid Class Value (not an int)
				strcpy(messageBuffer, "Invalid Class Value!  Use \"help\" for more information.\n");
				bufferSize = strlen(messageBuffer);
				sys_req(WRITE, TERMINAL, messageBuffer, &bufferSize);
				}
		
		}
		else{
		//Duplicate Name
			strcpy(messageBuffer, "Invalid Name! Process of this name already exists! Use \"help\" for more information.\n");
			bufferSize = strlen(messageBuffer);
			sys_req(WRITE, TERMINAL, messageBuffer, &bufferSize);
		}
	}
  return LOOP;	
}

int deletePCB(int argc, char **argv) {  //Handle a PCB that is currently running??
	pcb *tempPCB;
  if(argc != 2) {
    invalidArgs(argv[0]);
  }
  else {  
	if((tempPCB = findPCB(argv[1]))!= NULL){
		removePCB(tempPCB);
		freePCB(tempPCB);
	}
	else{
		printError(PCB_NOT_FOUND);
	}
  }
  return LOOP;
}
	
void queueInit() {
	ready = (pcb_queue *) sys_alloc_mem(sizeof(pcb_queue));
	ready->head = ready->tail = NULL;
	ready->count = 0;
	blocked = (pcb_queue *) sys_alloc_mem(sizeof(pcb_queue));
	blocked->head = blocked->tail = NULL;
	blocked->count = 0;
}

int priorityCheck(char *arg){
  double checkVal;
  int returnVal;
  sscanf(arg, "%lf", &checkVal);
  if(fmod(checkVal,1) == 0)
    returnVal = (int)(checkVal/1);
  else{
	returnVal = INV_PRIORITY;
  }
  return returnVal;
}

int parseClass(char *c) {
	int returnVal;
	if(strcmp(c, "sys") == 0) {
		returnVal = SYS;
	} else if(strcmp(c, "app") == 0) {
		returnVal = APP;
	} else {
		returnVal = INV_CLASS;
	}
	return returnVal;
}

