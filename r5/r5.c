#include "r5.h"


int com_open(int *eflag_p, int baud_rate) {
  int returnVal = 0;
  //Check Parameters
  if(eflag_p != NULL){
	if(
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
