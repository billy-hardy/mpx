/* -- Name:			ls.c																					*
 * -- Author:		Trent Roling																			*
 * -- Created:		September 8 2013																		*
 * -- Last Edited: 	September 8, 2013																		*
 * -- Pre Cond:		commhand.c matches on ls and argc == 1 | 2 --> if 2, argv[1] contains a valid directory *										*
 * -- Post Cond: 	directory listing is printed to screen													*/
 
 #include "r1.h"
 
int ls(int argc, char **argv) {
  
  if(argc == 1) {
    //do shit
  } else if(argc == 2) {
    //do other shit
  } else {
    invalidArgs(argv[0]);
  }
  return LOOP;
}
