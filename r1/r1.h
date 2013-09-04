#ifndef R1_H
#define R1_H

//std libs and mpx
#include <stdio.h>
#include <string.h>
#include "mpx_supt.h"

//constants
#define TRUE 1
#define LOOP 1 //If returned, repl will continue. Return !LOOP to quit
#define NUM_COMMANDS 5
#define VERSION 1 //version number
#define DAY 1     //date of last change
#define MONTH 9
#define YEAR 2013

//function prototypes
void tokenize(int *, char ***, char *);
int commhand();
void invalidArgs(char *);
int exit(int, char **);
int ls(int, char **);
int help(int, char **);
int date(int, char **);
int isLeapYear(int);
int checkDays(int, int, int);
int version(int, char **);

#endif