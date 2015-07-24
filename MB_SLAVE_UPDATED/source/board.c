
/*
*------------------------------------------------------------------------------
* device.c
*
* Board specipic drivers module(BSP)
*
* (C)2009 Sam's Logic.
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: device.c
* Created by		: Sam
* Last changed by	: Sam
* Last changed		: 11/12/2010 
*------------------------------------------------------------------------------
* Revision 1.3 11/12/2010  Sam
* Demo Release
* Revision 1.2 15/11/2010  Sam
* Updated according to the current project requirement
* Revision 1.1 07/07/2010 Sam
* First Release
* Revision 1.0 14/07/2009 Sam
* Initial revision
*
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/


#include "board.h"
#include "config.h"
#include "typedefs.h"




/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

static BOOL ledState;

/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void InitializeBoard(void)

* Summary	: This function configures all i/o pin directions
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void BRD_init(void)
{
	// set all anolog channels as Digital I/O
	ADCON1 = 0x0F;
	ADCON0 = 0X00;		//Turn off ADC

	HEART_BEAT_DIRECTION = PORT_OUT;

    // Enable internal PORTB pull-ups
    //INTCON2bits.RBPU = 0;

	TX_EN_DIR = PORT_OUT;		// Rs485 Direction Control
	TX_EN = 0;					//initialized for RX 

	
	SER_TX_DIR  = PORT_OUT;		//USART Tx 
	SER_RX_DIR	= PORT_IN;		//USART Rx


	KEYPAD_PORT_0_DIRECTION	= PORT_IN;

	// MMD Configuration
	DISPLAY_CONTROL_DIRECTION = PORT_OUT;
	DISPLAY_CONTROL			  = DISPLAY_DISABLE;


	DATA_PORT_DIR = 0x00;
	DIGIT_PORT_DIR = 0x00;
	DIGIT_PORT = 0x00;
	

	MMD_CONTROL_DIR = PORT_OUT;
	MMD_CONTROL = SWITCH_OFF;


	DIGIT_SEL_A_DIR= PORT_OUT;
	DIGIT_SEL_A = SWITCH_OFF;
	DIGIT_SEL_B_DIR= PORT_OUT;
	DIGIT_SEL_B = SWITCH_OFF;
	DIGIT_SEL_C_DIR= PORT_OUT;
	DIGIT_SEL_C = SWITCH_OFF;
	
}



/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*  End of device.c
*/
