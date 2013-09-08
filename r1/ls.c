/* -- Name:			ls.c																					*
 * -- Author:		Trent Roling																			*
 * -- Created:		September 8 2013																		*
 * -- Last Edited: 	September 8, 2013																		*
 * -- Pre Cond:		commhand.c matches on ls and argc == 1 | 2 --> if 2, argv[1] contains a valid directory *										*
 * -- Post Cond: 	directory listing is printed to screen													*/
 
 #include "r1.h"
 
int ls(int argc, char **argv) {
  
  char dir[512];
  char file_name[512];
  long file_size;
  
  if(argc == 1) {
  
    getcwd(*dir, 512);
	sys_open_dir(dir);
	sys_get_entry(file_name, 512, *file_size);
	printf("%s  %d \n", file_name, &file_size);
	sys_close_dir();

	}
  
  else if(argc == 2) {
    //do other shit
	}
	
  else {
    invalidArgs(argv[0]);
	}
	
  return LOOP;
}
