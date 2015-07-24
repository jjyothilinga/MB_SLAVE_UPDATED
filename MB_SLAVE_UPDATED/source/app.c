#include "app.h"
#include "typedefs.h"
#include "mb.h"
#include "mmd.h"
#include "linearkeypad.h"
#include "digit_driver.h"
#include "rtc_driver.h"
#include "string.h"
#include "stdlib.h"

/*---------------------------------------------------------------
*
*---------------------------------------------------------------*/
#define REG_INPUT_START 1
#define REG_INPUT_NREGS 17



// APP Structure
typedef struct _APP
{
	//used to store the application data
	UINT8 mbData[10]; 

	//used to store start time of the operations set by the user
	UINT16 startTime;

	//used to store actual qty.
	UINT16 actualCount; 

	//used to store target qty set by the user
	UINT16 targetQty;

	//used to store time set by the user
	UINT16 targetTime;

	//target hourly rate = ( targetQty / targetTime )
	UINT32 targetHrlyRate;

	//actual hourly rate = ( actualQty / (presentTime-startTime) )
	UINT32 actualHrlyRate;

	//used to switch on status lamps ( green and red )
	BOOL status;
	BOOL previousState;
	
	//mb data received flag
	BOOL dataReceived;

	//used to store current time from RTC
	UINT16 currentTime;

	BOOL pbPressed;


}APP;

/*---------------------------------------------------------------
* Global variables
*---------------------------------------------------------------*/
#pragma idata APP_DATA
APP app = {{0},0};
MMD_Config mmdConfig = {0}; 
#pragma idata

UINT8 writeTimeDateBuffer[] = {0X00, 0X52, 0X10, 0X01, 0x01, 0X01, 0X14};

/*---------------------------------------------------------------
* Private functions
*---------------------------------------------------------------*/
void handleMBData( void );
void updateActual( void );
void calculateTargetHrlyRate( void );
void calculateActualHrlyRate( void );
void getCurrentTime( void );
void updateTargetHrlyRate( void );
void updateActualHrlyRate( void );
#ifdef RTC_TEST
void displayRTCData( void );
#endif
/*---------------------------------------------------------------
* void APP_init(void)
*---------------------------------------------------------------*/


void APP_init(void)
{
	UINT8 i;

	//app.actualCount = 10;

	//WriteRtcTimeAndDate(writeTimeDateBuffer);
#ifdef RTC_TEST
	WriteRtcTimeAndDate(writeTimeDateBuffer);
	displayRTCData();	
#endif

/*
	app.targetQty = 1500;
	app.targetTime = 40;
	calculateTargetHrlyRate();
	updateTargetHrlyRate();

	app.actualCount = 93;
	app.startTime = 900; // 12hr 30 minutes
	calculateActualHrlyRate();
	updateActualHrlyRate();
*/



	app.previousState = app.status = TURN_ON_GREEN_LAMP;
}


/*---------------------------------------------------------------
* void APP_task(void)app
*---------------------------------------------------------------*/
void APP_task(void)
{
	
#ifdef RTC_TEST
	displayRTCData();
	
#else
	handleMBData();

	
	if ( LinearKeyPad_getKeyState(INPUT_KEY) == TRUE && app.pbPressed == TRUE)
	{
		//INCREMENT ACTUAL COUNT
		app.actualCount++;
		updateActual();

		//CALCULATE PARAMETERS
		calculateTargetHrlyRate();
		calculateActualHrlyRate();

		//UPDATE DISPLAY
		updateTargetHrlyRate();
		updateActualHrlyRate();

		if( app.actualHrlyRate > app.targetHrlyRate )
			app.status = TURN_ON_RED_LAMP;
		else if ( app.actualHrlyRate < app.targetHrlyRate )
			app.status = TURN_ON_GREEN_LAMP;
			
		app.pbPressed = FALSE;
	}
	else if ( LinearKeyPad_getKeyState(INPUT_KEY) == FALSE && app.pbPressed == FALSE )
		app.pbPressed = TRUE;

	if ( (app.status == TURN_ON_RED_LAMP) && (app.previousState != app.status) )
	{
		//Turn OFF green lamp 
		DDR_loadDigit( GREEN_LED, 10 );
		DelayMs(1);	
	
		//Turn ON red lamp
		DDR_loadDigit( RED_LED, 8 );
		DelayMs(1);	

		app.previousState = app.status;

	}
	else if ( (app.status == TURN_ON_GREEN_LAMP) && (app.previousState != app.status) )
	{
		//Turn ON green lamp 
		DDR_loadDigit( GREEN_LED, 8 );
		DelayMs(1);	
	
		//Turn OFF red lamp
		DDR_loadDigit( RED_LED, 10 );
		DelayMs(1);	

		app.previousState = app.status;
	}

#endif




}


/*---------------------------------------------------------------
* modbus callback
*---------------------------------------------------------------*/

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{

    eMBErrorCode    eStatus = MB_ENOERR;

    int             iRegIndex;
/*
    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0         {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
 
*/
    return eStatus;

}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{

	UINT8	starting_add = usAddress;
	UINT8	no_regs		 = usNRegs * 2;
	eMBErrorCode    eStatus = MB_ENOERR;
	UINT8 i = 0;
	UINT8 test[10] = "IDEONICS ";

	switch(eMode)
	{
	case MB_REG_WRITE:

    
	while( no_regs > 0)
	{

		app.mbData[i++] = * pucRegBuffer++;

		starting_add++;
		no_regs	--;
	}
	app.dataReceived = TRUE;
    break;

 	case MB_REG_READ: 

	while(no_regs > 0)
	{

			* pucRegBuffer++ =	'A';
			* pucRegBuffer++ =	'B';		
			
			* pucRegBuffer++ = 'C';
			* pucRegBuffer++ = 'D';

		starting_add++;
		no_regs	--;	
	}
   	 break;
	}

	return eStatus;
  }


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}


/*---------------------------------------------------------------
* void handleMBData( void )
*---------------------------------------------------------------*/

void handleMBData( void )
{
	UINT8 command = 0;
	UINT8 i, j = 0;
	UINT8 buffer[5];
	UINT8 startTime[6] = "     ";

	if ( app.dataReceived == TRUE )
	{
		command = app.mbData[0];

		switch ( command )
		{
			case CMD_START_TIME:

				for( i = 0; i < 5; i++ )
				{
					startTime[i] = app.mbData[i+1];
				}
			
				startTime[i] = ' ';

				MMD_clearSegment(0);
				mmdConfig.startAddress = 0;
				mmdConfig.length = 6;
				mmdConfig.symbolCount = strlen(startTime);
				mmdConfig.symbolBuffer = ( startTime );
				mmdConfig.scrollSpeed = 0;	
				MMD_configSegment( 0 , &mmdConfig );

				//store the start time into the local buffer
				//to convert into minutes
				for( i = 0, j = 0; i < 5; i++, j++ )
				{
					if( app.mbData[i+1] == '-' )
						i++;

					buffer[j] = app.mbData[i+1];
				}
				buffer[5] = '\0';
				//store minutes
				app.startTime = (UINT16)atoi( buffer+2 );
				buffer[2] = '\0';
			
				//convert hours into minutes and add with the minutes
				app.startTime += (UINT16)( atoi( buffer ) * 60 );

			break;

			case CMD_TARGET_QTY:
			
			for ( i = TARGET_QTY_START, j = 0; i <= TARGET_QTY_END; i++, j++ )
			{
				DDR_loadDigit( i, app.mbData[j+1] - '0' );
				DelayMs(1);
			} 

			//store the actual quantity
			app.targetQty = (UINT16)atoi( (app.mbData+1) );
			
			break;

			case CMD_TARGET_TIME:

			for ( i = TARGET_TIME_START, j = 0; i <= TARGET_TIME_END; i++, j++ )
			{
				DDR_loadDigit( i, app.mbData[3 - (j+1)] - '0' );
				DelayMs(1);
			} 

			//store the target time in minutes
			app.targetTime = (UINT16)atoi( (app.mbData+1) );
			
			break;

			default:
			break;
		}
		
		app.dataReceived = 0;

		//CALCULATE PARAMETERS
		//calculateTargetHrlyRate();

		//UPDATE DISPLAY
		//updateTargetHrlyRate();
	}

}


/*---------------------------------------------------------------
* void updateActual( void )
*---------------------------------------------------------------*/

void updateActual( void )
{
	UINT8 i = 0, j = 0;
	UINT8 buffer[4];
	UINT16 temp = app.actualCount;

	do
	{
		buffer[i++] = (temp % 10) + '0';		
	}while ( (temp /= 10) != 0 );

	if( i < 4 )
	{
		for( ; i < 4; i++ )
			buffer[i] = '0';
	}

	for ( i = ACTUAL_QTY_START; i <= ACTUAL_QTY_END; i++, j++ )
	{
		DDR_loadDigit( i, buffer[3 - j] - '0' );
		DelayMs(1);
	} 
			
}


/*---------------------------------------------------------------
* void updateActual( void )
*---------------------------------------------------------------*/
void calculateTargetHrlyRate( void )
{
	UINT32 count = app.targetQty;
	UINT32 tTime = app.targetTime;

	count *= 10000;

	//target hourly rate = ( targetQty / targetTime )
	app.targetHrlyRate = ((UINT32)( count/tTime ));
}


/*---------------------------------------------------------------
* void updateActual( void )
*---------------------------------------------------------------*/
void calculateActualHrlyRate( void )
{
	UINT32 count = app.actualCount;
	UINT32 cTime;  
	UINT32 sTime = app.startTime;
	count *= 10000;

	getCurrentTime();
	cTime = app.currentTime;

	//actual hourly rate = ( actualQty / (presentTime-startTime) )
	app.actualHrlyRate = (UINT32)( (count / (cTime - sTime)) );
}


/*---------------------------------------------------------------
* void getCurrentTime(void)
*---------------------------------------------------------------*/
void getCurrentTime( void )
{
	UINT8 buffer[5] = {0};

	UINT8 RTCData[6];

	ReadRtcTimeAndDate(RTCData);

	//RTCData[1] = 0x40;
	//RTCData[2] = 0x11;

	//store minutes
	buffer[3] = (RTCData[1] & 0X0F) + '0';        //Minute LSB
	buffer[2] = ((RTCData[1] & 0XF0) >> 4) + '0'; //Minute MSB

	//store time into the buffer
	buffer[1] = (RTCData[2] & 0X0F) + '0';		//Hour LSB
	buffer[0] = ((RTCData[2] & 0XF0) >> 4) + '0'; //Hour MSB 

	buffer[5] = '\0';
	app.currentTime = (UINT16)atoi( buffer+2 );
	buffer[2] = '\0';
	
	app.currentTime += (UINT16)( atoi( buffer ) * 60 ); 	
}



/*---------------------------------------------------------------
* void updateTargetHrlyRate( void )
*---------------------------------------------------------------*/
void updateTargetHrlyRate( void )
{
	UINT8 i = 0, j = 0,k;
	UINT8 buffer[10];

	UINT32 temp = app.targetHrlyRate;

	do
	{
		buffer[i++] = (temp % 10) + '0';		
	}while ( (temp /= 10) > 0 );

	if( i < 5 )
	{
		for( ; i < 5; i++ )
			buffer[i] = '0';
	}

	for ( k = TARGET_HR_RATE_START; k <= TARGET_HR_RATE_END; k++, j++ )
	{
		DDR_loadDigit( k, buffer[i - j - 1] - '0' );
		DelayMs(1);
	} 

}


/*---------------------------------------------------------------
* void updateActualHrlyRate( void )
*---------------------------------------------------------------*/
void updateActualHrlyRate( void )
{
	UINT8 i = 0, j = 0, k;
	UINT8 buffer[10] = {0};

	UINT32 temp = app.actualHrlyRate;

	do
	{
		buffer[i++] = (temp % 10) + '0';		
	}while ( (temp /= 10) > 0 );

	if( i < 5 )
	{
		for( ; i < 5; i++ )
			buffer[i] = '0';
	}

	for ( k = ACTUAL_HR_RATE_START; k <= ACTUAL_HR_RATE_END; k++, j++ )
	{
		DDR_loadDigit( k, buffer[i - j - 1] - '0' );
		DelayMs(1);
	} 
}



/*---------------------------------------------------------------
* void displayRTCData( void )
*---------------------------------------------------------------*/

void displayRTCData( void )
{
	UINT8 i, j = 0;
	UINT8 RTCData[6];
	UINT8 displayBuffer[4];
	static UINT8 count = 0;
	count++;

	ReadRtcTimeAndDate(RTCData);

	if ( count >= 100 )
	{
		for(i = 0; i < 7; i++)			
		{
			WriteUSART(RTCData[i]) ;  //store time and date 
			DelayMs(20);
		}
		count = 0;
	}


			
	displayBuffer[0] = (RTCData[1] & 0X0F) + '0';        //Minute LSB
	displayBuffer[1] = ((RTCData[1] & 0XF0) >> 4) + '0'; //Minute MSB

	displayBuffer[2] = (RTCData[2] & 0X0F) + '0';		//Hour LSB
	displayBuffer[3] = ((RTCData[2] & 0XF0) >> 4) + '0'; //Hour MSB 

	for ( i = ACTUAL_QTY_START; i <= ACTUAL_QTY_END; i++, j++ )
	{
		DDR_loadDigit( i, displayBuffer[3 - j] - '0' );
		DelayMs(1);
	} 
}