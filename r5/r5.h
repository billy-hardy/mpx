#ifndef R5_H
#define R5_H

int com_open(int *, int);
int com_close();
int com_read(char *, int *);
int com_write(char *, int *);
void interrupt shit();
void interrupt shit2();

#endif

