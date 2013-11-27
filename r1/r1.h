#ifndef R1_H
#define R1_H

//std libs and mpx
#include <stdio.h>
#include <string.h>
#include "mpx_supt.h"

//constants
#define TRUE 1
#define LOOP 1 //If returned, repl will continue. Return !LOOP to quit
#define NUM_COMMANDS 24
#define VERSION 6 //version number

#define printMPX(x,y) do { y = strlen(x); sys_req(WRITE, TERMINAL, x, &y); } while(0)

//function prototypes
void tokenize(int *, char *[], char *, const char *);
void commhand();
int eval(char *);
void invalidArgs(char *);
int exitMPX(int, char **);
int ls(int, char **);
int help(int, char **);
void displayHelp(int, char *);
int date(int, char **);
int isLeapYear(int);
int checkDays(int, int, int);
int version(int, char **);
int history(int, char **);
void printCommandToFile(char []);
void cleanUpHistory();
void queueFree();
int exec(int, char **);
int clearScreen(int, char **);
int changePrompt(int, char **);
void r5Init();
void r6Init();
void empty_pcb_queue(pcb_queue *);

#endif
