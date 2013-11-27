#ifndef R5_H
#define R5_H

#include <dos.h>
#include "mpx_supt.h"

int com_open(int *, int);
int com_close();
int com_read(char *, int *);
int com_write(char *, int *);
void interrupt LVL1_INT_HANDLER();
void interrupt LVL2_INT_INPUT();
void interrupt LVL2_INT_OUTPUT();
void interrupt LVL2_INT_MS();
void interrupt LVL2_INT_LS();

#define RING_BUFFER_SIZE 100 //This may need to be changed (I'm unsure of the size)

//DCB STATUSES
#define OPENED -9999
#define CLOSED -9998

#define IDLING -9997
#define READING -9996
#define WRITING -9995

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
	
	char ring_buffer[RING_BUFFER_SIZE];
	int ring_buffer_in;//index to write next character
	int ring_buffer_out;//index where the next character will be removed
	int ring_buffer_count; //count of # characters stored but not read from buffer
	
	
};
typedef struct dcbStruct dcb;

#define calcBaudRate(x) (115200 / (long) x)

//Symbolic Constants
//Open Stuff
#define O_INV_E_FLAG_POINTER -101
#define O_INV_B_RATE_DIVISOR -102
#define O_PORT_OPEN -103
//Close Stuff
#define C_SERIAL_PORT_NOT_OPEN -201
//Read Stuff
#define R_PORT_NOT_OPEN -301
#define R_INVALID_BUFFER_ADDRESS -302
#define R_INVALID_COUNT_ADDRESS -303
#define R_DEVICE_BUSY -304
//Write Stuff
#define W_SERIAL_PORT_NOT_OPEN -401
#define W_INVALID_BUFFER_ADDRESS -402
#define W_INVALID_COUNT_ADDRESS -403
#define W_DEVICE_BUSY -404 

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
#define IE_BIT_ONE 0x02

#endif
