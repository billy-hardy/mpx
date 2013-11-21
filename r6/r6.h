#ifndef R6_H
#define R6_H

#include "r2.h"

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
