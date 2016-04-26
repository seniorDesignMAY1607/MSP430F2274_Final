/*
 * I2C_polled.h
 *
 *  Created on: Apr 25, 2016
 *      Author: nbergman
 */

#ifndef I2C_POLLED_H_
#define I2C_POLLED_H_

#include <msp430f2274.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"
#include "timer.h"


//Flag defines
#define I2C_UNSENT 		0x00
#define I2C_IN_BUFFER	BIT0
#define I2C_SENT		BIT1
#define I2C_BUFFER_OVF	BIT2
#define I2C_NACK		BIT3
#define I2C_TX_MODE		BIT4

//Used for polledSend
//TODO: Change out for flag variables
#define I2C_BUSY 1
#define I2C_NO_ACK	2
#define I2C_PASS	3


typedef enum {
	NOT_BUSY,
	IS_BUSY,
	NACK
}i2c_status_t;




//Initialize I2C for 100kHz
//USCI_B0
//SDA = 3.1
//SCL = 3.2
void i2c_masterInit(void);

//Send data over I2C. Busy flag is polled per cycle until data is sent.
//Returns 1 if something went wrong.
int8_t i2c_sendPolledData(uint8_t address, uint8_t data[], uint8_t numBytes);


#endif /* I2C_POLLED_H_ */
