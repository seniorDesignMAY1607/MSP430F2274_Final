/*
 * button.c
 *
 *  Created on: Apr 8, 2016
 *      Author: nbergman
 */

#include "button.h"

//Navigation buttons -- Port 4
#define PB_UP	BIT3
#define PB_DN	BIT4
#define PB_LF	BIT5
#define PB_RT	BIT6
#define PB_SEL	BIT7

//Power switch -- Port 4
#define PWR_SW_PIN	BIT2

//#define PWR_ON_LED 	BIT4 //Port 3

//Initialize ports for buttons, and proj_on switch
void button_initPorts(void)
{
	P4DIR &= ~(PB_UP | PB_DN | PB_LF | PB_RT | PB_SEL);
	//P2IE |= (PB_UP | PB_DN | PB_LF | PB_RT | PB_SEL);
	P4DIR &= ~(PWR_SW_PIN);


}


//Check for change in power switch and toggle current state
void button_checkPowerKey(powerState_t *currentState)
{
	//TODO: change to pin change interrupt



	switch(*currentState)
	{
		case OFF:
			if(P4IN & PWR_SW_PIN) *currentState = TURN_ON;
			//Else Nothing
			break;
		case TURN_ON:
			*currentState = ON;
			break;

		case ON:
			if(!(P4IN & PWR_SW_PIN)) *currentState = TURN_OFF;
			//Do Nothing
			break;

		case TURN_OFF:
			*currentState = OFF;
			//P3OUT |= PWR_ON_LED;
			break;

		default:
			//Error undefined state
			//P3OUT |= BIT5; //Error led
			break;
	}
}

