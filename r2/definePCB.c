#include "r2.h"


//Parameters: name, class and priority
// name must be unique
// class and priority must be valid
int createPCB(int argc, char **argv) {
  pcb *tempPCB = sys_alloc_mem(sizeof(pcb));
  int classVal, priorityVal;
  int bufferSize;
  char messageBuffer[128];
  if(argc != 4) {
    invalidArgs(argv[0]);
  }
  else {
		if(findPCB(argv[1]) == NULL){
			if((classVal= parseClass(argv[2])) != INV_CLASS){
				if((priorityVal = priorityCheck(argv[3])) != INVALID_PRIOR){
					if (paramsGood(argv[1], classVal, priorityVal)){
						setupPCB(tempPCB, argv[1], classVal, priorityVal); 
						if(tempPCB != NULL){
							tempPCB->next = NULL;
							tempPCB->prev = NULL;
							insertPCB(tempPCB);
						}							
					} else {
						//Invalid Parameters
						printError(INVALID_PARAMS);
					}
				}	else{
				//invalid Priority Value (not an int)
					printError(INVALID_PRIOR);
				}			
			}else{
		//invalid Class Value (not an int)
				printError(INVALID_CLASS);
				}		
		}	else{
		//Duplicate Name
			printError(DUP_PCB);
		}
	}
  return LOOP;	
}

int deletePCB(int argc, char **argv) {  
	pcb *tempPCB;
  if(argc != 2) {
    invalidArgs(argv[0]);
  }
  else {  
		if((tempPCB = findPCB(argv[1]))!= NULL){
			removePCB(tempPCB);
			sys_free_mem(tempPCB->load_address);
			freePCB(tempPCB);
		}
		else{
			printError(PCB_NOT_FOUND);
		}
  }
  return LOOP;
}



int priorityCheck(char *arg){
  int returnVal;
  sscanf(arg, "%i", &returnVal);
	if((-128>returnVal || returnVal >127))
		returnVal = INVALID_PRIOR;
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