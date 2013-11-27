#include <dos.h>
#include <time.h>
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

//r4 Prototypes
int load(int, char**);
int terminate(int, char**);
void loadProgram(char*, int);
void terminateMemory(pcb *);


typedef struct params {
  int op_code;
  int device_id;
  unsigned char *buff_addr;
  int *count_addr;
} params;

//r6 stuff
#define WR_TIME_LIMIT 5
#define RD_TIME_LIMIT 50

typedef struct iod {
  char name[20];
  pcb *curr;
  int request; //IDLE, READ, WRITE, or CLEAR (op_code from r3)
  char *trans_buff; //buffer_address from r3
  int *count; //count_address from r3
  struct iod *next; //next iod in queue
} iod;

typedef struct {
  int event_flag; //current event flag for device
  int count; //number of IOD's in queue
  iod *head; //front of IOD queue
  iod *tail; //end of IOD queue
} iocb;

int io_scheduler();
void insertIOD(iocb *, iod *);
void io_init();
void io_tear_down();
void empty_iocb(iocb *);

#endif
