#include <dos.h>
#include "r2.h"


typedef struct context {
  unsigned int BP, DI, SI, DS, ES;
  unsigned int DX, CX, BX, AX;
  unsigned int IP, CS, FLAGS;
} context;

//Prototypes
void interrupt dispatch();
void interrupt sys_call();
//?
void load_procs(pcb *, context *, void (*func)(void));
void r3Init();

void test1_R3();
void test2_R3();
void test3_R3();
void test4_R3();
void test5_R3();

//temp commands
int callDispatch(int, char **);
int loadTestProcess(int, char **);

void mkFPStackTop(unsigned char *);

#define SYS_STACK_SIZE 200

typedef struct params {
  int op_code;
  int device_id;
  unsigned char *buff_addr;
  int *count_addr;
} params;

