#include <dos.h>

unsigned char* MK_FP;
unsigned char FP_SEG();
unsigned char FP_OFF();
//removed the void* from seg and off


//Prototypes
void interrupt dispatch();
void interrupt sys_call();
//?
void loadProcess();
void callDispatch();
void loadTestProcess();


#define SYS_STACK_SIZE 1024

typedef struct params{
	int op_code;
	int device_id;
	unsigned char *buff_addr;
	int *count_addr;
}params;

