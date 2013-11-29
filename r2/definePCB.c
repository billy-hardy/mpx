#include "r2.h"

//CREATEPCB
//Author: Robert Brown
//Input: argc (the int number of argument tokens passed)
//       argv (the char pointers to the argument tokens)
//Output: LOOP (1)
//Parameters: name, class and priority
// name must be unique
// class and priority must be valid
//Provided the paremeters are correct, creates a PCB structure 
//using setupPCB, and all the functions called down this chain.
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

//DELETEPCB
//Author: Robert Brown
//Input: argc, argv (the number of tokens, and tokens themselves)
//Output: LOOP (1)
//Finds the PCB with the passed name (via argv) or returns an error
//Removes the PCB from its queue, and frees the memory associated with the PCB
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


//PRIORITYCHECK
//Author: Robert Brown
//Input: priority (character pointer to the string value indicating priority value)
//Output: 0 (Priority correct), INVALID_PRIOR error code otherwise.
//Checks that the priority is between -128 and 127, and returns true if so, and Error code otherwise
int priorityCheck(char *arg){
  int returnVal;
  sscanf(arg, "%i", &returnVal);
	if((-128>returnVal || returnVal >127))
		returnVal = INVALID_PRIOR;
  return returnVal;
}

//PARSECLASS
//Author: Billy Hardy
//Input: Class (char pointer)
//Ouput: Integer value representation of SYS or APP, or INV_CLASS error code otherwise
//Checks if the passed character pointer resolves to sys or app, or neither.
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