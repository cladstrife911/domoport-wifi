#include "command.h"

UART_CMD_TYPE CheckCmds()
{	
	if(cmdNew)
	{
		// Clear the Flag
		cmdNew = FALSE;
		
		//	Command "rps_on"
		if(strstr(u_cmd, "rps_on")!=NULL)
			return CMD_RPS_ON;
		// Command "rps_off": 
		else if(strstr(u_cmd, "rps_off")!=NULL)
			return CMD_RPS_OFF;
		// Command "version": get the firmware version of the EnOcean module
		else if(strstr(u_cmd, "version")!=NULL)
			return CMD_FW_VERSION;
		// Command "ping"
		else if(strstr(u_cmd, "ping")!=NULL)
			return CMD_PING;
		// Command "getID"
		else if(strstr(u_cmd, "getID")!=NULL)
			return CMD_GET_ID;
		else
		{
			ConsoleWrite("Unkown command: ");
			ConsoleWrite(u_cmd);
			return -1; //error
		}
		
		ConsoleWrite("\r\n");
	}
	
	return -1;
}
