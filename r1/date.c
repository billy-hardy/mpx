/* -- Name:			Date.c																					*
 * -- Author:		Robert Brown																			*
 * -- Created:		August 30, 2013																			*
 * -- Last Edited: 	September 7, 2013																		*
 * -- Pre Cond:		commhand.c matched on date and 0 < argc <= 2											*
 * -- Post Cond: 	argc = 1: returns system date.  argc = 2: changes system date given an acceptable date	*/

#include "r1.h"

int date(int argc, char **argv) {
	
	date_rec *today = (date_rec*) sys_alloc_mem((size_t) sizeof(date_rec));
	char buffer[75];
	int bufferSize;
	int month=-1, day=-1, year=-1; 
	int scanReturn;
		
	if(today == NULL){
		strcpy(buffer, "Memory Allocation Failed!\n");
		bufferSize = strlen(buffer);
		sys_req(WRITE, TERMINAL, buffer, &bufferSize);
	}
	else{
		if(argc == 1){ //Display current system date if only [date] is entered.

			sys_get_date(today);
			sprintf(buffer, "\nThe current system date is: %d-%d-%d\n\n", today->month, today->day, today->year); 
			bufferSize = strlen(buffer);
			sys_req(WRITE, TERMINAL, buffer, &bufferSize);
		
		}
		else if(argc == 2){ //Prompt and change system date if [date MM-DD-YYY] is entered
		
			scanReturn = sscanf(argv[1], "%d-%d-%d", &month, &day, &year);
			
			if(scanReturn != 3){ //Invalid date format
				strcpy(buffer, "Error Scanning Date.  Type \"help\" for help.\n\n");
				bufferSize = strlen(buffer);
				sys_req(WRITE, TERMINAL, buffer, &bufferSize);
			}
			else{
				if(!checkDays(month, day, year)){ //Call checkDays function to ensure date validity
					buffer[0] = '\0';
					strcpy(buffer, "Invalid Date Parameters!  Please see help for functionality.\n\n");
					bufferSize = strlen(buffer);
					sys_req(WRITE, TERMINAL, buffer, &bufferSize);
				}
				else{
					today->month = month;
					today->day = day;
					today->year = year;
					sys_set_date(today);
				}
			}
		}
		else{ //Invalid Args Catch
			invalidArgs(argv[0]); 
		}
		
		sys_free_mem(today);
	}
	return 1; //True
}

// Checks if the current year is a leap year.
// Pre-cond: 	an integer year given
// Post-cond:	True(1):  year is a Leap-Year, FalsE(0): otherwise
int isLeapYear(int year){
	int returnVal = 0;
	if(((year%4 == 0) && (year%100 != 0) || (year%400 == 0)))
		returnVal = 1;
	return returnVal;
}

// Checks if entered day is a valid number for given month
// Pre-Cond:	Integer month, day, and year are passed.
// Post-Condition: True(1) If day is valid for the month
//				   False(0) and error message if invalid day.
int checkDays(int month, int day, int year){
	int returnVal = 0;
	char buffer[100];
	int bufferSize;

	if(month > 0 && day > 0 && year > 0){
	//Should there be an upper bounds on the year? 9999ish?
		if(month < 13){
			if(day < 32){
				switch(month){ //Months Jan, Mar, May, July, Aug, Oct, Dec (31 Days)
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						returnVal = 1;
					break;
				
					case 4:    //Months April, June, July, November (30 Days)
					case 6:
					case 9:
					case 11:
						if(day < 31)
							returnVal = 1;
						else{ //Invalid Day Error
							returnVal = 0;
							buffer[0]='\0';
							sprintf(buffer, "Current Month \"%d\" does not support day values greater than 30\n\n", month);
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
							else{ //Invalid year (not a leap year)
								returnVal = 0;
								buffer[0]= '\0';
								strcpy(buffer, "Current Year is not a 'Leap Year', Day value must be less than 29\n\n");
								bufferSize = strlen(buffer);
								sys_req(WRITE, TERMINAL, buffer, &bufferSize);
							}
						}
						else{ //Invalid day (greater than 29)
							returnVal = 0;
							buffer[0] = '\0';
							sprintf(buffer, "Current Month \"%d\" does not support values greater than 29\n\n", month);
							bufferSize = strlen(buffer);
							sys_req(WRITE, TERMINAL, buffer, &bufferSize);
						}
					break;
						
					default:
					break;
				}
				
			}
			else{ //Invalid day (greater than 31 catch)
				returnVal =0;
				buffer[0]='\0';
				strcpy(buffer, "Day value must be between 1 and 31\n\n");
				bufferSize = strlen(buffer);
				sys_req(WRITE, TERMINAL, buffer, &bufferSize);
			}			
		}
		else{ //Invalid Month (greater than 12)
			returnVal = 0;
			buffer[0] = '\0';
			strcpy(buffer, "Month value must be between 1 and 12\n\n");
			bufferSize = strlen(buffer);
			sys_req(WRITE, TERMINAL, buffer, &bufferSize);
		}
	}
	else{
		//This is actually kind of wasted space/code atm but it will be moved and used if a cap is put on year
		buffer[0]='\0';
		strcpy(buffer, "Invalid Date Parameters!  See help for details\n\n");
		bufferSize = strlen(buffer);
		sys_req(WRITE, TERMINAL, buffer, &bufferSize);
	}
	
return returnVal;
}
