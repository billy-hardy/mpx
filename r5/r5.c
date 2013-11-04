#include "r5.h"


int com_open(int *eflag_p, int baud_rate) {
	//baud_rate_div = 115200 / (long) baud_rate
  int returnVal = 0;
  dcb serialPort;
  //Check Parameters
  if(eflag_p != NULL){
	if(baud_rate > 0){
		if(serialPort->flag != OPEN){
			serialPort->flag = OPEN;
			serialPort->event_flag = eflag_p;
			serialPort->status = IDLE;
			serialPort->ring_buffer_in = 0;
			serialPort->ring_buffer_out = 0;
			serialPort->ring_buffer_count = 0;
		
		} else returnVal = PORT_OPEN;
	} else returnVal = INV_B_RATE_DIVISOR;
  } else returnVal = INV_E_FLAG_POINTER;
  
 return returnVal; 
}

int com_close() {
  
}

int com_read(char * buf_p, int *count_p) {
  
}

int com_write(char * buf_p, int *count_p) {
  
}

void interrupt shit() {
  
}

void interrupt shit2() {
  
}
