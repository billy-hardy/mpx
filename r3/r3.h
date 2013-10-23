#include <dos.h>
#include "r2.h"
#include "r1.h"
//Prototypes
void interrupt dispatch();
void interrupt sys_call();
//?
void load_procs(char *, int, int, void *);
void r3Init();

//R4 Prototypes
int load(int, char **);
int terminate(int, char **);
void loadProgram(char **, int); 
void terminateMemory(pcb *);

//**********************************************Don't forget to make it so SYS PCBs can't be suspended.

//temp commands
int callDispatch(int, char **);
int loadTestProcess(int, char **);

void mkFPStackTop(unsigned char *);

#define SYS_STACK_SIZE 1024

typedef struct params {
  int op_code;
  int device_id;
  unsigned char *buff_addr;
  int *count_addr;
} params;

typedef struct context {
  unsigned int BP, DI, SI, DS, ES;
  unsigned int DX, CX, BX, AX;
  unsigned int IP, CS, FLAGS;
} context;


