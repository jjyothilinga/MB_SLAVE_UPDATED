#include "config.h"
#include "string.h"


typedef enum
{
	CMD_START_TIME		= 0X80,
	CMD_TARGET_QTY		= 0X81,
	CMD_TARGET_TIME		= 0X82
}CMD;

enum
{
	INPUT_KEY = 0
};

enum
{
	TURN_ON_GREEN_LAMP = 0,
	TURN_ON_RED_LAMP
};

typedef enum
{
	TARGET_TIME_START		= 6,
	TARGET_TIME_END			= 7,
	TARGET_QTY_START		= 9,
	TARGET_QTY_END			= 12,
	ACTUAL_QTY_START 		= 13,
	ACTUAL_QTY_END			= 16,
	TARGET_HR_RATE_START	= 17,
	TARGET_HR_RATE_END		= 21,
	ACTUAL_HR_RATE_START	= 22,
	ACTUAL_HR_RATE_END		= 26,
	RED_LED					= 30,
	GREEN_LED 				= 31			
}DD_SRC_DST;

void APP_init(void);
void APP_task(void);



