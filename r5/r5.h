#ifndef R5_H
#define R5_H

#include <dos.h>
int com_open(int *, int);
int com_close();
int com_read(char *, int *);
int com_write(char *, int *);
void interrupt LVL1_INT_HANDLER();
void interrupt LVL2_INT_INPUT();
void interrupt LVL2_INT_OUTPUT();
void interrupt LVL2_INT_MS();
void interrupt LVL2_INT_LS();

#define RING_BUFFER_SIZE 2048 //This may need to be changed (I'm unsure of the size)

//DCB STATUSES
#define OPENED 0
#define CLOSED 1

#define IDLE 0
#define READING -1
#define WRITING 1

struct dcbStruct {
	int flag; //OPENED or CLOSED
	int *event_flag; 
	int status;//IDLE, READING, WRITING
	char *in_buff; //pointer to the requestor's buffer
	int *in_count; //counter of max num chars that can be in buffer
	int in_done; //counter for how many characters have been placed in buffer
	
	char *out_buff;
	int *out_count;
	int out_done;
	
	char[RING_BUFFER_SIZE] ring_buffer;
	int ring_buffer_in;//index to write next character
	int ring_buffer_out;//index where the next character will be removed
	int ring_buffer_count; //count of # characters stored but not read from buffer
	
	
};
typedef struct dcbStruct dcb;

#define calcBaudRate(x) (115200 / (long) x)

//Symbolic Constants
#define INV_E_FLAG_POINTER -101
#define INV_B_RATE_DIVISOR -102
#define PORT_OPEN -102

#define COM1_INT_ID 0x0c
#define COM1_BASE 0x3F8
#define COM1_INT_EN COM1_BASE+1
#define COM1_BRD_LSB COM1_BASE
#define COM1_BRD_MSB COM1_BASE+1
#define COM1_INT_ID_REG COM1_BASE+2
#define COM1_LC COM1_BASE+3
#define COM1_MC COM1_BASE+4
#define COM1_LS COM1_BASE+5
#define COM1_MS COM1_BASE+6
#define PIC_MASK 0x21
#define PIC_CMD 0x20
#define EOI 0x20
#define LNCTRLVAL 0x80
#define LNCTRLVAL2 0x03
#define PIC_MASK 0x21
#define INT_OVERALL_SERIAL 0x08
#define INT_INPUT_READY 0x01

#endif

