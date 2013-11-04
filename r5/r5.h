#ifndef R5_H
#define R5_H

#include <dos.h>
int com_open(int *, int);
int com_close();
int com_read(char *, int *);
int com_write(char *, int *);
void interrupt shit();
void interrupt shit2();

#define RING_BUFFER_SIZE 2048 //This may need to be changed (I'm unsure of the size)

//DCB STATUSES
#define OPENED 0
#define CLOSED 1

#define IDLE 0
#define READING -1
#define WRITING 1

struct dcbStruct {
	int status; //OPENED or CLOSED
	int *event_flag; //IDLE, READING, WRITING
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

//Symbolic Constants
#define INV_E_FLAG_POINTER -101
#define INV_B_RATE_DIVISOR -102
#define PORT_OPEN -102

#endif

