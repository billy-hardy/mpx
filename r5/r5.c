#include "r5.h"
void interrupt (*oldfunc)(void);
dcb *serialPort;

int com_open(int *eflag_p, int baud_rate) {
	int bRateVal = 0;
	int returnVal = 0; 
	int picMaskVal;
	serialPort = (dcb *)sys_alloc_mem(sizeof(dcb));
	//Check Parameters
	if(eflag_p != NULL){
		if(baud_rate > 0){
			if(serialPort->flag != OPENED){
				serialPort->flag = OPENED;
				//Save the event flag
				serialPort->event_flag = eflag_p;
				serialPort->status = IDLING;
				//Set Ring Buffer
				serialPort->ring_buffer_in = 0;
				serialPort->ring_buffer_out = 0;
				serialPort->ring_buffer_count = 0;
				//Save Address of current interrupt handler
				oldfunc = getvect(COM1_INT_ID);
				//Install new handler into the interrupt vector
				setvect(COM1_INT_ID, &LVL1_INT_HANDLER);
				//Calculate Baud Rate Divisor
				bRateVal = calcBaudRate(baud_rate);
				//Store 0x80 in LC Register
				outportb(COM1_LC, LNCTRLVAL);
				//Store high order in MSB register
				outportb(COM1_BRD_MSB, ((bRateVal >> 8) & 0xFF)); //I think the 0xFF is only needed in certain compilation cases
				//Store low order in LSB register
				outportb(COM1_BRD_LSB, (bRateVal & 0xFF));
				//Store 0x04 in LC Register
				outport(COM1_LC, LNCTRLVAL2);
				//Enable appropriate level in PIC mask Register (level 4 for COM1)
				disable(); //Disable maskables
				picMaskVal = inportb(PIC_MASK); //0x21 ... This may also need to be cast to int as it is unsigned char
				picMaskVal = picMaskVal & ~0x10; //(enables level 4)
				outportb(PIC_MASK, picMaskVal);
				enable(); //Enable maskables
				//Enable overall serial port interrupts (0x08 in MC Register)
				outportb(COM1_MC, INT_OVERALL_SERIAL);
				//Enable input ready interrupts only (0x01 in IE Register)
				outportb(COM1_INT_EN, INT_INPUT_READY);			
			} else returnVal = O_PORT_OPEN;
		} else returnVal = O_INV_B_RATE_DIVISOR;
	} else returnVal = O_INV_E_FLAG_POINTER;  
	return returnVal; 
}

int com_close() {
	return 0;
}

int com_read(char *buf_p, int *count_p) {
	int returnVal = 0;
	//Check Parameters (I don't really like this arrow of Ifs)
	if(serialPort->flag == OPENED){
		if(serialPort->status == IDLING){
			if(buf_p != NULL){
				if(count_p != NULL){
					//Initialize input buffer variables
					serialPort->in_buff = buf_p;
					serialPort->in_count = count_p;
					serialPort->in_done = 0;
					serialPort->status = READING;
					//Clear Event Flag
					*serialPort->event_flag = 0;
					//Disable interrupts, and copy from ring buffer
					disable();
					//Stuff exists in ring buffer, the required amount has not yet been placed in buffer, and not a linefeed
					if((serialPort-> ring_buffer_count) > 0){
					while((serialPort->in_done < *(serialPort->in_count)) && ((serialPort->ring_buffer[serialPort->ring_buffer_out]) != '\r')){
						//Copy to requester's buffer from ring buffer
						serialPort->in_buff[serialPort->in_done] = serialPort->ring_buffer[serialPort->ring_buffer_out];
						serialPort->ring_buffer_in++;
						//Check if loop around is needed
						if(serialPort->ring_buffer_in >= RING_BUFFER_SIZE){
							serialPort->ring_buffer_in = 0;
						}
						serialPort->in_done++;
						serialPort->ring_buffer_count--;
					}
					}
					enable();
					//If more characters are needed, return
					if(((serialPort ->in_done) < *(serialPort-> in_count))&&((serialPort->ring_buffer[serialPort->ring_buffer_out]) != '\r')){
						return returnVal; //Need to find a way to move this ... I don't like the multiple return				
					}
					//If here, we're done ...
					//Set DCB status to idle
					serialPort->status = IDLING;
					//Set the event flag
					*serialPort->event_flag = 1;
					serialPort->in_buff[serialPort->in_done] = '\0';
					//Return the actual count to the requester's variable
					*serialPort->in_count = serialPort->in_done;  //I think
					*count_p = serialPort->in_done;
				}else returnVal = R_INVALID_COUNT_ADDRESS;			
			}else returnVal = R_INVALID_BUFFER_ADDRESS;
		}else returnVal = R_DEVICE_BUSY;	
	}else returnVal = R_PORT_NOT_OPEN;	
	return returnVal;
}

int com_write(char * buf_p, int *count_p) {
	int returnVal = 0;
	unsigned char picMask;
	//Check Parameters
	if(serialPort->flag == OPENED){
		if(serialPort->status == IDLING){
			if(buf_p != NULL){
				if(count_p != NULL){
					//Everything checks out ... Install the buffer pointers
					serialPort->out_buff = buf_p;
					serialPort->out_count = count_p;
					serialPort->out_done = 0;
					serialPort->status = WRITING;
					//Clear the event Flag
					*(serialPort->event_flag) = 0;
					//Get first character from requester's buffer and store in the output register
					outportb(COM1_BASE, *(serialPort->out_buff));
					serialPort->out_buff++;
					serialPort->out_done++;
					//Enable write interrupts by setting bit 1 of the IE register
					//Logical or of contents and 0x02 (bit 1)
					disable();
					picMask = inportb(COM1_INT_EN);
					picMask = picMask | IE_BIT_ONE;
					//reset value
					outportb(COM1_INT_EN, picMask);
					enable();
				}else returnVal = W_INVALID_COUNT_ADDRESS;
			}else returnVal = W_INVALID_BUFFER_ADDRESS;
		}else returnVal = W_DEVICE_BUSY;
	}else returnVal = W_SERIAL_PORT_NOT_OPEN;
	return returnVal;
}

void interrupt LVL1_INT_HANDLER() {
  unsigned char value;
  if (serialPort->flag == OPENED){
	//Read Interrupt ID Register
	value = inportb(COM1_INT_ID_REG);
	switch (value){
		case 0: //Modem Status Interrupt(Not sure if this is needed)
			LVL2_INT_MS();
			break;
		case 2: //Output Interrupt
			LVL2_INT_OUTPUT();
			break;
		case 4: //Input Interrupt
			LVL2_INT_INPUT();
			break;
		case 6: //Line Status Interrupt (Not sure if this is needed)
			LVL2_INT_LS();
			break;
		default:
			break;
	}
  }
  outportb(PIC_CMD, EOI);
}

void interrupt LVL2_INT_INPUT() { //Read
	unsigned char input;
	input = inportb(COM1_BASE); //read a character from the input register
	//check DCB status
	if(serialPort->status == READING){
		//Store character in requester's buffer
		serialPort->in_buff[serialPort->in_done] = input;
		serialPort->in_done ++;
		//Check for completion
		if((serialPort->in_done < *serialPort->in_count) && (input != '\r'))
			return;
		else{
			serialPort->in_buff[serialPort->in_done] = '\0';
			serialPort->status = IDLING;
			*serialPort->event_flag = 1;
			*serialPort->in_count = serialPort->in_done;
		}
	}else{
		//Check for Ring Buffer Storage
		if(serialPort->ring_buffer_count >= RING_BUFFER_SIZE)
			return;
		//Store character in ring buffer
		serialPort->ring_buffer[serialPort->ring_buffer_in] = input;
		serialPort->ring_buffer_in ++;
		//Check for loop around
		if(serialPort->ring_buffer_in >= RING_BUFFER_SIZE)
			serialPort->ring_buffer_in = 0;
		serialPort->ring_buffer_count ++;		
	}
}

void interrupt LVL2_INT_OUTPUT(){ //Write
	int picMask;
	if(serialPort->status == WRITING){
		//check for completion
		if(serialPort->in_done < *(serialPort->out_count)){
			outportb(COM1_BASE, *serialPort->out_buff);
			serialPort->out_done ++;
			serialPort->out_buff ++;
		}else{
			//Finished so...
			serialPort->status = IDLING;
			*serialPort->event_flag = 1;
			//This might be incorrect
			*serialPort->out_count = serialPort->out_done;
			//Disable write interrupts (clear bit 1 in IE Register)
			disable();
			picMask = inportb(COM1_INT_EN);
			picMask &= IE_BIT_ONE;
			outportb(COM1_INT_EN, picMask);
			enable();
		}		
	}
}

void interrupt LVL2_INT_LS(){ //Shouldn't happen, but just in case
	unsigned char garbage;
	garbage = inportb(COM1_LS);
}

void interrupt LVL2_INT_MS(){ //Shouldn't happen, but just in case
	unsigned char garbage;
	garbage = inportb(COM1_MS);
}
