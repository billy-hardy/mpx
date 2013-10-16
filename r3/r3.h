#include <dos.h>

//Prototypes
void interrupt dispatch();
void interrupt sys_call();
//?
void load_procs(char *, int, int, void *);
void r3Init();

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

extern pcb *running;
