
/*
*------------------------------------------------------------------------------
* main.c
*
*------------------------------------------------------------------------------
*/



/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <timers.h>				// Timer library functions
#include <delays.h>				// Dely library functions
#include <string.h>				// String library functions
#include "board.h"				// board setup
#include "timer.h"
#include "heartbeat.h"
#include "mmd.h"
#include "app.h"
#include "mb.h"
#include "digit_driver.h"
#include "rtc_driver.h"
#include "linearkeypad.h"
/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Processor config bits
*------------------------------------------------------------------------------
*/

#pragma config OSC      = HSPLL
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOREN    = ON
#pragma config BORV     = 3
#pragma config WDT      = OFF
#pragma config WDTPS    = 512	
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
#pragma config STVREN   = ON
#pragma config LVP      = OFF
//#pragma config ICPRT  = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
//#pragma config WRT3   = OFF
#pragma config WRTB     = OFF//N       // Boot Block Write Protection
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF
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
extern UINT16 keypadUpdate_count;

/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/
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
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void main(void)

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/

#define MMD_REFRESH_PERIOD	(65535 - 15000)
#define TICK_PERIOD	(65535 - 10000)

void main(void)
{
	UINT8 i,j,k;
	UINT8 count = 0;

#ifdef MMD_TEST
	MMD_Config mmdConfig= {0};
	//UINT8 line[50] ="Ideonics ideas and electronics, subhrajyoti biswal "; 
	UINT8 line[] = "      ";
	UINT8 data[] = "IDEONICS1 ";

#endif
	BRD_init();			//board initialization


#ifdef __DIGIT_DISPLAY_TEST__

	for(i = 6; i < 32 ; i++ )
	{
		for( k = 0; k < 11 ; k++)
			{
				DDR_loadDigit(i,k);
				DelayMs(75);
			}
	}

#endif
	
	HB_init();			//initialize heart beat module
	MMD_init();			//initialize mmd module
	RTC_Init();

	//modbus initialization
	eMBInit( MB_RTU, ( UCHAR )SLAVE_ID, 0, BAUD_RATE, MB_PAR_NONE);
	eMBEnable(  );	/* Enable the Modbus Protocol Stack. */

	APP_init();

	TMR0_init(TICK_PERIOD,0);		//initialize timer0
	TMR1_init(MMD_REFRESH_PERIOD,MMD_refreshDisplay);		//initialize timer1					//initialize timer1

	EnableInterrupts( );

#ifdef MMD_TEST
	MMD_clearSegment(0);
	mmdConfig.startAddress = 0;
	mmdConfig.length = 6;
	mmdConfig.symbolCount = strlen(line);
	mmdConfig.symbolBuffer = line;
	mmdConfig.scrollSpeed = 0; //SCROLL_SPEED_MEDIUM;	
	MMD_configSegment( 0 , &mmdConfig);


#endif

	//Turn ON green lamp 
	DDR_loadDigit( GREEN_LED, 8 );
	DelayMs(1);	

	//Turn OFF red lamp
	DDR_loadDigit( RED_LED, 10 );
	DelayMs(1);	

	
   	while(LOOP_FOREVER)
    {
		//Heart Beat to blink at every 500ms
	`	if(heartBeatCount >= 500 )
		{			
			HB_task();
			heartBeatCount = 0;
		}

		if( mmdUpdateCount >= 10 )
		{
			MMD_task();
			mmdUpdateCount = 0;
		}

		eMBPoll();

		if(keypadUpdate_count >= 10)
		{
			LinearKeyPad_scan();
			keypadUpdate_count = 0;
			count++;
		}
		
		if(count >= 2)
		{
			APP_task();	
			count = 0;
		}

		ClrWdt();				// Kick the dog
    }



}

/*
*  End of main.c
*/
