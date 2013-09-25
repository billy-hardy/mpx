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
		if((tempPCB=findPCB(argv[1])) == NULL){
			if((classVal= integerCheck(argv[2])) != NULL){
				if((priority = integerCheck(argv[3]))!= NULL){
					if (paramsGood(argv[1], classVal, priorityVal)){
						tempPCB = allocatePCB();
						tempPCB = setupPCB(argv[1], classVal, priorityVal); 
						insertPCB(tempPCB);	
					//********************Are there error codes for these somewhere.....
					}
					else{
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
		removePCB(tempPCB); //This will work once the function is changed.
		freePCB(tempPCB);
	}
	else{
		printError(PCB_NOT_FOUND);
	}
  }
  return LOOP;
}
	

int integerCheck(char *in){
  double checkVal;
  int returnVal = NULL;
  checkVal = atof(in);
  if(checkVal%1 > 0);
  else{
    returnVal = (int)checkVal;
  }
  return returnVal;
}
