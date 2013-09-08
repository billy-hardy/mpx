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
  char buffer[256];
  int buffer_size, i, action;
  
  printEmpty();
  
  if(argc == 1) { //display all files with .MPX extensions in the working directory
  
	sys_open_dir(0);
	
	for (i=0; i<22; i++){
		action = sys_get_entry(file_name, 512, &file_size);
		if (action == ERR_SUP_NOENTR){ //If it runs out of unique entries, break
			printEmpty();
			break;
		}
		else{
			sprintf(buffer, "%s\t%d bytes\n", file_name, file_size);
			buffer_size = strlen(buffer);
			sys_req(WRITE, TERMINAL, buffer, &buffer_size);
		}
	}

	sys_close_dir();

	}
  
  else if(argc == 2) { //display all files with .MPX extensions in the specified directory
	
	sys_open_dir(argv[1]);
	
	for (i=0; i<22; i++){
		action = sys_get_entry(file_name, 512, &file_size);
		if (action == ERR_SUP_NOENTR){
			printEmpty();
			break;
		}
		else{
			sprintf(buffer, "%s\t%d bytes\n", file_name, file_size);
			buffer_size = strlen(buffer);
			sys_req(WRITE, TERMINAL, buffer, &buffer_size);
		}
	}

	sys_close_dir();
	}
	
  else {
    invalidArgs(argv[0]);
	}
	
  return LOOP;
}

int printEmpty(){ //Quick function to print an empty line, makes the code a little cleaner
	char buffer[2];
    int buffer_size;
	sprintf(buffer, "\n");
	buffer_size = strlen(buffer);
	sys_req(WRITE, TERMINAL, buffer, &buffer_size);
return 0;
}