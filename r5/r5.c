#include "r5.h"
void interrupt (*oldfunc)(void);
 dcb serialPort;

int com_open(int *eflag_p, int baud_rate) {
	//baud_rate_div = 115200 / (long) baud_rate
  int bRateVal = 0;
  int returnVal = 0; 
  int picMaskVal;
  //Check Parameters
  if(eflag_p != NULL){
	if(baud_rate > 0){
		if(serialPort->flag != OPEN){
			serialPort->flag = OPEN;
			//Save the event flag
			serialPort->event_flag = eflag_p;
			serialPort->status = IDLE;
			//Set Ring Buffer
			serialPort->ring_buffer_in = 0;
			serialPort->ring_buffer_out = 0;
			serialPort->ring_buffer_count = 0;
			//Save Address of current interrupt handler
			oldfunc = getvect(COM1_INT_ID);
			//Install new handler into the interrupt vector
			setvect(COM1_INT_ID, &LVL1_INT_HANDLER);
			//Calculate Baud Rate Divisior
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
			picMaskVal &= ~0x10; //(enables level 4)
			ourtportb(PIC_MASK, picMaskVal);
			enable(); //Enable maskables
			//Enable overall serial port interrupts (0x08 in MC Register)
			outportb(COM1_MC, INT_OVERALL_SERIAL);
			//Enable input ready interrupts only (0x01 in IE Register)
			outportb(COM1_INT_EN, INT_INPUT_READY);			
		} else returnVal = PORT_OPEN;
	} else returnVal = INV_B_RATE_DIVISOR;
  } else returnVal = INV_E_FLAG_POINTER;
  
 return returnVal; 
}

int com_close() {
  
}

int com_read(char * buf_p, int *count_p) {
  
}

int com_write(char * buf_p, int *count_p) {
  
}

void interrupt LVL1_INT_HANDLER() {
  unsigned char value;
  if (serialPort->flag == OPEN){
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

void interrupt LVL2_INT_INPUT() {
  
}

void interrupt LVL2_INT_OUTPUT(){

}

void interrupt LVL2_INT_LS(){
	unsigned char garbage;
	garbage = inportb(COM1_LS);
}

void interrupt LVL2_INT_MS(){
	unsigned char garbage;
	garbage = inportb(COM1_MS);
}
