/*
 * I2C_polled.c
 *
 *  Created on: Apr 25, 2016
 *      Author: nbergman
 */


#include "I2C_polled.h"

typedef struct {
	uint8_t addr;
	uint8_t	subAddr;
	uint8_t *data;
	uint8_t length;
	uint8_t curPos;
	uint8_t *flags;
}i2c_frame_t;

i2c_status_t i2c_currentStatus = NOT_BUSY;

//Initialize I2C for 100kHz
//USCI_B0
//SDA = 3.1
//SCL = 3.2
void i2c_masterInit(void)
{

	//Master, I2C, Synchronous
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 |= UCMST | UCMODE1 | UCMODE0 | UCSYNC;

	//SMCLK, Transmitter,
	UCB0CTL1 |= UCSSEL1 | UCTR;
	UCB0BR0 = 10; //Divide by 10 prescalar -- 1MHZ SMCLK

	//config ports
	P3SEL |= BIT1 | BIT2; //See device specific datasheet for selecting alternate pin functions

	//Undo Reset
	UCB0CTL1 &= ~UCSWRST;

	//Interrupt Enable
	//UCB0I2CIE |=  UCNACKIE;	//Start Condition and Nack IE
	//IE2 |= UCB0TXIE | UCB0RXIE;			//TX ISR -- TX/RX Buffer
										//RX ISR -- I2C Status information
	//TODO: Enable Interupts

}

//Send data over I2C. Busy flag is polled per cycle until data is sent.
//Returns 1 if something went wrong.
int8_t i2c_sendPolledData(uint8_t address, uint8_t data[], uint8_t numBytes)
{
	int i = 0;
	int busyCount = 0;

	while(1)
	{
		if(!(UCB0STAT & UCBBUSY)) break;
		else if(busyCount > 20)
		{
			return -1;
		}
		timer_waitMicro(20);
		busyCount++;


	}

	//Set device Address
	UCB0I2CSA = address;

	//Generate start condition
	UCB0CTL1 |= UCTXSTT;



	for(i = 0; i < numBytes; i++)
	{

		//Wait for start flag to go high
		//Put data in TXBUF
		UCB0TXBUF = data[i]; //Send data if ACK
		//Busy and wait
		//flag = (uint8_t) IFG2;
		while(!(IFG2 & UCB0TXIFG))
		{
			if(UCB0STAT & UCNACKIFG)
				{
					UCB0CTL1 |= UCTXSTP;
					return I2C_NO_ACK;
				}
				//timer_waitMicro(1);
				//P3OUT ^= BIT4;

		}
	}
	UCB0CTL1 |= UCTXSTP; // Stop Condition

	if(UCB0STAT & UCNACKIFG) return I2C_NO_ACK; //Check for I2C ERROR
	else return I2C_PASS;


}

