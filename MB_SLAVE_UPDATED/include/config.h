#ifndef CONFIG_H
#define CONFIG_H

/*
*------------------------------------------------------------------------------
* config.h
*
*/

#define __74LS374__


#define DEVICE_ADDRESS			0x01


#define __FACTORY_CONFIGURATION__

//TIMER Configuration
#define TIMESTAMP_DURATION 		(200)

//MMD module configuration

#define MMD_MAX_CHARS		8
#define MMD_MAX_SEGMENTS	1
#define MMD_MAX_ADDRESS		128//(MMD_MAX_CHARS * MMD_MAX_SEGMENTS)
#define MMD_TEST
//#define __DIGIT_DISPLAY_TEST__


//COM module configuration
#define __BCC_XOR__
//#define __TRANSMITTER_ENABLED__
//#define __LOOP_BACK__
#define BROADCAST_ADDRESS		0xFA
#define CMD_SOP	0xAA
#define CMD_EOP 0xBB
#define RESP_SOP	0xCC
#define RESP_EOP	0xDD


enum
{
	RX_PACKET_SIZE = 60,
	TX_PACKET_SIZE = 60
};

#define 	RECEIVER_MAX_PAKET_SIZE		(60)	
#define 	TRANSMITTER_MAX_PAKET_SIZE	(60)


// Enable for external eeprom access
// Comment out this line if internal eeprom access required
//#define EEP_EXTERRNAL
// For uart enable
//#define EN_UART
// Enable for hardware i2c module usage
//#define USE_MSSP_I2C


//RTC CONFIGURATION
//#define TIME_DEBUG


/*--------------------------------
* MODBUS slave configuration
*-------------------------------*/

#define SLAVE_ID		1
#define BAUD_RATE		19200


//#define RTC_TEST


/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables (extern)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Constants (extern)
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Public Function Prototypes (extern)
*------------------------------------------------------------------------------
*/

#endif
/*
*  End of config.h
*/



