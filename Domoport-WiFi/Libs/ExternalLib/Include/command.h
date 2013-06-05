#include "HWlib.h"
#include "console.h"

#ifndef _COMMAND_H
#define _COMMAND_H	


typedef enum
{
	CMD_RPS_ON = 0x01,
	CMD_RPS_OFF = 0x02,
	CMD_FW_VERSION =0x03,
	CMD_PING =0x04,
	CMD_GET_ID= 0x05,
}UART_CMD_TYPE;


extern UART_CMD_TYPE CheckCmds();

#endif //_COMMAND_H
