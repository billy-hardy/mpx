#include "r3.h"
#include "r2.h"
//REMEMBER NULCH = '\0'

//LOAD
//Author: Robert Brown
//Input: argc, and argv (the number of tokens, and the tokens themselves)
//Output: LOOP(1) (and possible error message)
//Performs the load operations by passing arguments to the loadProgram function

//Accepts program name, and optional int priority parameter
int load(int argc, char **argv){
	int priorityVal;
	//Check arguments
	if(argc == 2 || argc ==3){
		if(argc == 2)
			priorityVal = 0;
		else{
			priorityVal = priorityCheck(argv[2]);
			if(priorityVal == INVALID_PRIOR){
				printError(priorityVal);
				return LOOP;
			}
		}
		loadProgram(argv[1], priorityVal);
	}
	else
		invalidArgs(argv[0]);
	return LOOP;
}

//TERMINATE
//Author: Robert Brown
//Input: argc, and argv (the usual number of tokens, and the tokens themselves)
//Output: LOOP (1), and possible Error Message
//Removes a process from queues, and frees the memory associated with it.
//Accepts Program Name to be terminated.  PCBName = Program Name -.mpx
int terminate(int argc, char **argv){
	pcb *tempPCB;
	//check arguments
	if(argc == 2){
		tempPCB = findPCB(argv[1]);
		if(tempPCB->class != SYS) {
			if(tempPCB != NULL)
				terminateMemory(tempPCB);
			else
				printError(PCB_NOT_FOUND);
		} else {
			printError(TERM_SYS_PROC);
		}
	}
	else
		invalidArgs(argv[0]);
	return LOOP;
}

//LOADPROGRAM
//Author: Robert Brown
//Input: Process Name (char pointer)
//       Priority (int)
//Ouput: void (Possible error messages)
//Checks the arguments passed, checks that the program is valid, creates and sets up
//a PCB (allocating memory for the program), and then loads the program.
//Otherwise returns the appropriate error code.
//Loads program into PCB memory, and sets appropriate registers.
void loadProgram(char *processName, int priorityVal){
	int sysCheckReturnVal = 0, sysLoadProgramReturnVal = 0;
	int programLength, programStartOffset;
	pcb *programPCB;
	context *cPCB;
	
	if((findPCB(processName))==NULL){ //No Duplicate Process Name
		sysCheckReturnVal = sys_check_program("\0", processName, &programLength, &programStartOffset);
		if(sysCheckReturnVal==0){ //This may need adjustments
			programPCB = (pcb*)sys_alloc_mem(sizeof(pcb));
			setupPCB(programPCB, processName, APP, priorityVal);
			programPCB->suspended = TRUE;
			programPCB->memory_size = programLength;
			programPCB->load_address = (unsigned char*)sys_alloc_mem(programLength); //Or should this include PCB size
			programPCB->exec_address = programPCB->load_address + programStartOffset;

			//Load Program and check return value of function
			sysLoadProgramReturnVal = sys_load_program(programPCB->load_address, programPCB->memory_size, "\0", processName);
			if(sysLoadProgramReturnVal == 0){
				//Program is loaded.  Manually set some registers.
				cPCB = (context *) programPCB->top;
				cPCB->IP = FP_OFF(programPCB->exec_address);
				cPCB->CS = FP_SEG(programPCB->exec_address);
				cPCB->FLAGS = 0x200;
				cPCB->DS = _DS;
				cPCB->ES = _ES;
				insertPCB(programPCB);  //If here, Program can be inserted into the ready queue w/ suspended status.
			} else{//Program Load Error
				printError(sysLoadProgramReturnVal);
				return;					
			}				
		} else{//Check Program Error
			printError(sysCheckReturnVal);
			return;
		}
	} else{//Duplicated PCB Name Error
		printError(DUP_PCB);
		return;
	}
}

//TERMINATEMEMORY
//Author: Robert Brown
//Input: PCB to be freed
//Output: void (possible error messages)
//Frees memory associated with PCB.
void terminateMemory(pcb *pcbName){
	removePCB(pcbName); //Remove from Queue
	//sys_free_mem(pcbName->load_address); //Free Load Address
	freePCB(pcbName); //Free remaining PCB memory
}




