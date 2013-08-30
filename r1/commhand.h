#ifndef COMMHAND_H
#define COMMHAND_H

//std libs and mpx
#include <stdio.h>
#include <string.h>
#include "mpx_supt.h"

//our files
#include "common.h"
#include "help.h"
#include "exit.h"
#include "ls.h"
#include "date.h"
#include "version.h"

//constants
#define NUM_COMMANDS 5

//function prototypes
void tokenizer(int *, char ***, char *);
int commhand();
void rename(char ***, int, char **);

#endif
