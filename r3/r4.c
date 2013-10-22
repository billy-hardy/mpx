#include "r3.h"
#include "r2.h"
//None of this has been compiled or tested at the moment

//REMEMBER NULCH = '\0'

//Accepts program name, and optional int priority parameter
void load(int argc, char **argv){
	int priorityVal;
	
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
			//Call to LoadProgram here
			loadProgram(argv, priorityVal);
		}
	}
	else
		invalidArgs(argv[0]);
}

void loadProgram(char **argv, int priorityVal){
	int sysCheckReturnVal = 0, sysLoadProgramReturnVal = 0;
	int programLength, programStartOffset;
	pcb *programPCB;
	context *cPCB;
	
	if(findPCB(argv[1]==NULL){ //No Duplicate Process Name
		sysCheckReturnVal = sys_check_program(NULCH, argv[1], &programLength, &programStartOffset);
		if(sysCheckReturnVal==0){ //This may need adjustments
			setupPCB(programPCB, argv[1], APP, priorityVal);
			programPCB->suspended = TRUE;
			programPCB->memory_size = programLength;
			programPCB->load_address = (unsigned char*)sys_allocmem(programLength); //Or should this include PCB size
			programPCB->exec_address = programPCB->load_address + programStartOffset);

			//Load Program and check return value of function
			sysLoadProgramReturnVal = sys_load_program(programPCB->load_address, programPCB->memory_size, NULCH, argv[1]);
			if(sysLoadProgramReturnVal == 0){
				//Program is loaded.  Manually set some registers.
				cPCB = (context *) programPCB->stack_top;
				cPCB->IP = FP_OFF(programPCB->exec_address);
				cPCB->CS = FP_SEG(programPCB->exec_address);
				cPCB->FLAGS = 0x200;
				cPCB->DS = _DS;
				cPCB->ES = _ES;
				insertPCB(programPCB);  //If here, Program can be inserted into the ready queue w/ suspended status.
			}
			else{//Program Load Error
				printError(sysLoadProgramReturnVal);
				return;					
			}				
		}
		else{//Check Program Error
			printError(sysCheckReturnVal);
			return;
		}
	}
	else{//Duplicated PCB Name Error
		printError(DUP_PCB);
		return;
	}
}

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




