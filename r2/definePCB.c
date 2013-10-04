#include "r2.h"


//Parameters: name, class and priority
// name must be unique
// class and priority must be valid

//THIS HAS NOT BEEN COMPILED OR TESTED YET
int createPCB(int argc, char **argv) {
  pcb *tempPCB = (pcb *)sys_alloc_mem(sizeof(pcb));
	//pcb tempPCB2;
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
						//tempPCB = &tempPCB2;
						if(tempPCB != NULL)
							tempPCB->next = NULL;
							tempPCB->prev = NULL;
							insertPCB(tempPCB);	
					//********************Are there error codes for these somewhere.....
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
	


int priorityCheck(char *arg){
  double checkVal;
  int returnVal;
  sscanf(arg, "%lf", &checkVal);
  if(fmod(checkVal,1) == 0)
    returnVal = (int)(checkVal/1);
  else{
	returnVal = INVALID_PRIOR;
  }
	if(!(-128<=returnVal && returnVal <=127))
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

