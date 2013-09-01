/* -- Name:			Date.c																					*
 * -- Author:		Robert Brown																			*
 * -- Created:		August 30, 2013																			*
 * -- Last Edited: 	August 30, 2013																			*
 * -- Pre Cond:		commhand.c matched on date and 0 < argc <= 2											*
 * -- Post Cond: 	argc = 1: returns system date.  argc = 2: changes system date given an acceptable date	*/

#include "date.h"

int date(int argc, char **argv) {
	
	date_rec today_p = (date_rec*) sys_alloc_mem((size_t) sizeof(date_rec));
	char buffer[75];
	int bufferSize;
	int month=-1, day=-1, year=-1; // This might need to be moved to the top.  Will know during compile in TurboC
	int scanReturn;
	if(argc == 1){ 
		
		if(today_p == NULL){
			strcpy(buffer, "Memory Allocation Failed!\n");
			bufferSize = strlen(buffer);
			sys_req(WRITE, TERMINAL, buffer, &bufferSize);
		}
		
		else{
			sys_get_date(today);
			strcpy(buffer, "The current system date is: %d-%d-%d\n", today->month, today->day, today->year); //FIX THIS FORMATTED STRING COPY
			bufferSize = strlen(buffer);
			sys_req(WRITE, TERMINAL, buffer, &bufferSize);
		}
	}
	else if(argc == 2){ 

		
		scanReturn = sscanf(argv[1], "%d-%d-%d", &month, &day, &year);
		if(scanReturn != 3){
			strcpy(buffer, "Error Scanning Date.  Type \"help\" for help.\n");
			bufferSize = strlen(buffer);
			sys_req(WRITE, TERMINAL, buffer, &bufferSize);
		}
		else{
			//Moved all that mess to a separate function
			if(!checkDays(month, day, year)){
				buffer[0] = '\0';
				strcpy(buffer, "Invalid Date Parameters!  Please see help for functionality.\n")
				bufferSize = strlen(buffer);
				sys_req(WRITE, TERMINAL, buffer, &bufferSize);
			}
			else{
				today->month = month;
				today->day = day;
				today->year = year;
			}
		}
	}
	else{
		invalidArgs(argv[0]); 
	}
	
	sys_free_mem(today);
	return 1; //True
}

int isLeapYear(int year){
	returnVal = 0;
	if(((year%4 == 0) && (year%100 != 0) || (year%400 == 0)))
		returnVal = 1;
	return returnVal;
}	

int checkDays(int month, int day, int year){
	int returnVal = 0;
	
	if(month > 0 && day > 0 && year > 0){
	//Should there be an upper bounds on the year? 9999ish?
		if(month < 13){
			if(day < 32){
				switch(month){
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						returnVal = 1;
					break;
				
					case 4: 
					case 6:
					case 9:
					case 11:
						if(day < 31)
							returnVal = 1;
						else{
							returnVal = 0;
							buffer[0]='\0';
							strcpy(buffer, "Current Month \"%d\" does not support day values greater than 30\n", month);
							bufferSize = strlen(buffer);
							sys_req(WRITE, TERMINAL, buffer, &bufferSize);
						}
					break;
				
					case 2:
						if(day < 29)
							returnVal = 1;
						else if(day == 29){
						//Function here to check for leap year
							if(isLeapYear(year)){
								returnVal = 1;
							}
							else{
								returnVal = 0;
								buffer[0]= '\0';
								strcpy(buffer, "Current Year is not a 'Leap Year', Day value must be less than 29\n");
								bufferSize = strlen(buffer);
								sys_req(WRITE, TERMINAL, buffer, &bufferSize);
							}
						}
						else{
							returnVal = 0;
							buffer[0] = '\0';
							strcpy(buffer, "Current Month \"%d\" does not support values greater than 29\n", month);
							bufferSize = strlen(buffer);
							sys_req(WRITE, TERMINAL, buffer, &bufferSize);
						}
					break;
						
					default:
					break;
				}
				
			}
			else{
				returnVal =0;
				buffer[0]='\0';
				strcpy(buffer, "Month value must be between 1 and 12\n");
				buffernSize = strlen(buffer);
				sys_req(WRITE, TERMINAL, buffer, &bufferSize);
			}			
		}
	}
	else{
		//This is actually kind of wasted space/code atm but it will be moved and used if a cap is put on year
		buffer[0]='\0';
		strcpy(buffer, "Invalid Date Parameters!  See help for details\n");
		bufferSize = strlen(buffer);
		sys_req(WRITE, TERMINAL, buffer, &bufferSize);
	}
return returnVal;
}
