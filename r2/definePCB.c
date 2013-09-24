#include "r2.h"

extern pcb *running;
extern pcb_queue ready;
extern pcb_queue blocked;

//Parameters: name, class and priority
// name must be unique
// class and priority must be valid

//THIS HAS NOT BEEN COMPILED OR TESTED YET
int createPCB(int argc, char **argv) {
	pcb *tempPCB;
	int classVal, priorityVal;
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
					//Add to Queue here...which queue?
				}
				else{
				//Parameters invalid (strlen, out of bounds ints)
				}
			}
			else{
			//invalid Priority Value (not an int)
			}
				
		}
		else{
		//invalid Class Value (not an int)
		}
		
	}
	else{
	//Duplicate Name
	}
		
	
	
  }
  return LOOP;
}

//Parameters: name
// name must be valid
int deletePCB(int argc, char **argv) {
  if(argc != 2) {
    invalidArgs(argv[0]);
  } else {
    //TODO: stuff
  }
  return LOOP;
}


void integerCheck(char *in){
	double checkVal;
	int returnVal = NULL;
	checkVal = atof(in);
	if(checkVal%1 > 0);
	else{
		returnVal = (int)checkVal;
	}
	return returnVal;
}
