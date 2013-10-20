#include "r3.h"
#include "r2.h"
//None of this has been compiled or tested at the moment

//REMEMBER NULCH = '\0'

//Accepts program name, and optional int priority parameter
void loadProgram(int argc, char **argv){
	int priorityVal;
	char buffer[256];
	//SYS_CHECK Variables
	int sysCheckReturnVal = 0, sysLoadProgramReturnVal = 0;
	int programLength, programStartOffset;
	pcb *programPCB;
	
	//Check arguments
	if(argc != 2 || argc !=3){
		if(argc == 2)
			priorityVal = 0;
		else{
			priorityVal = checkPriority(argv[2]);
			if(priorityVal == INVALID_PRIOR){
				printError(priorityVal);
				return; //I think I can do this.
			}
		}
		sprintf(buffer, "create %s APP %d\n", argv[1], priorityVal); //Reconfigure argc and argv for CreatePCB
		tokenize(buffer, &argc, argv);
		//This is going to be a little duplication...I don't feel like pulling extracting the check inside createPCB
		//It will just check if it exists twice ATM
		if(findPCB(argv[1]==NULL){ //No Duplicate Process Name
			sysCheckReturnVal = sys_check_program(NULCH, argv[1], &programLength, &programStartOffset);
			if(sysCheckReturnVal==0){ //This may need adjustments
				setupPCB(programPCB, argv[1], APP, priorityVal);
				programPCB->suspended = TRUE;
				programPCB->memory_size = programLength;
				programPCB->load_address = (unsigned char*)sys_allocmem(programLength); //Or should this include PCB size
				programPCB->exec_address = programPCB->load_address + programStartOffset);
				//I need to do a lot of stuff here that is a lot like load_proc
				//but I can't based on what's going on here...should I just duplicate 
				//So either find a way to use Billy's function or do some more duplication
				
				//Implement Error Checking
				sysLoadProgramReturnVal = sys_load_program(programPCB->load_address, programPCB->memory_size, NULCH, argv[1]);
				if(sysLoadProgramReturnVal == 0){
					insertPCB(programPCB);  //If here, Program can be inserted into the ready queue w/ suspended status.
				}
				else{
					printError(sysLoadProgramReturnVal);
					return;					
				}
				
			}
			else{
				printError(sysCheckReturnVal);
				return;//I think I can do this.
			}
		}
		else{
			printError(DUP_PCB);
			return; //I think I can do this
		}

	}//Invalid Args
	else{
		invalidArgs(argv[0]);
	}
}

//Do I need to create another Load Command?

//I believe that this should work
void terminate(char *pcbName){
	pcb *tempPCB;
	tempPCB = findPCB(pcbName);
	if(tempPCB != NULL){
		removePCB(tempPCB);
		sys_free_mem(tempPCB->load_address); //I think <-----****
		freePCB(tempPCB);
	}	
}




