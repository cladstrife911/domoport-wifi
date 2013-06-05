/* **************************************************************************																					
 *  Software License Agreement
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License (version 2) as published by 
 *  the Free Software Foundation AND MODIFIED BY OpenPicus team.
 *  
 *  ***NOTE*** The exception to the GPL is included to allow you to distribute
 *  a combined work that includes OpenPicus code without being obliged to 
 *  provide the source code for proprietary components outside of the OpenPicus
 *  code. 
 *  OpenPicus software is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details. 
 * 
 * 
 * Warranty
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *$Date&
 *$Author&
 *$Rev$
 *
 **************************************************************************/
#include "taskFlyport.h"
#include "EnOcean_ESP3.h"
#include "tools.h"
#include "console.h"
#include "command.h"

#define UART_DEBUG

BOOL ID_LEARN_OK = FALSE; //This boolean will be used to know if an ID is already stored or not
BOOL flag_ON = FALSE;
BOOL flag_OFF = FALSE;
BOOL flag_new_data = FALSE; //becomes true when new data are available
UINT8 ID_learn[4] = {0x00,0x00,0x00,0x00}; //EnOcean ID are 32bits longs
UINT8 temperatureHexa = 0x00;	//this variable will be updated when a new EnOcean telegram from a learned temperature sensor will be received

void FlyportTask()
{
	UART_CMD_TYPE uartCommand;
	char uread[257];
	int toRead = 0;
	RETURN_TYPE result;
	
	int i=0;
	
							/*|dataLenght| opt |        		 type   	    | 					data			 	   |		optional data					*/
	UINT8 TelegramRPS_OFF[18] ={0x00, 0x07, 0x07,(UINT8)((ESP3_PACKET_TYPE)RADIO), 0xF6, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x2D, 0x00 };
							/*|dataLenght| opt |       			 type   		| 				data					   |		optional data					*/
	UINT8 TelegramRPS_ON[18] ={0x00, 0x07, 0x07,(UINT8)((ESP3_PACKET_TYPE)RADIO), 0xF6, 0x10, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x2D, 0x00 };
	
	//A5-02-05
	/*
	UINT8 Telegram4BS_Temperature[21] ={0x00, 0x0A, 0x07,(UINT8)((ESP3_PACKET_TYPE)RADIO), 
										(UINT8)((ESP3_RORG)RORG_4BS), 0x00, 0x00,
										0x79, //temperature
										0x08, //learn or data
										0x00, 0x00, 0x00, 0x00, 0x00,
										0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x2D, 0x00 };
	*/
	
	vTaskDelay(10);
	
	//Init of the UART for EnOcean module when using the EnOcean Nest
	IOInit(p5,UART2RX); //ADIO7 of TCM310
	IOInit(p4, UART2TX); //ADIO6 of TCM310
	UARTInit(2,57600);
	UARTOn(2);
	IOInit(d4out, out);
	
	//Connect to WIFI network
	WFConnect(WF_DEFAULT);
	while(WFStatus != CONNECTED);
	vTaskDelay(100);
	
	#ifdef UART_DEBUG
	UARTWrite(1,"\r\n - getPacket Test -");
	UINT8 testPacket[8] ={ESP3_SYNC_BYTE,0x00,0x01,0x00,(ESP3_PACKET_TYPE)COMMON_COMMAND,0x70,0x03,0x09};
	result = getPacket(testPacket,8);
	UARTDebug("\r\n result = ", result);
	vTaskDelay(20);

	UARTWrite(1,"\r\n - UARTSendPacket Test -");
						/*|dataLenght| opt | 		 type  		    		         | data |*/
	UINT8 testPacket2[5] ={0x00, 0x01, 0x00,(UINT8)((ESP3_PACKET_TYPE)COMMON_COMMAND), 0x03 };	//command 0x03 to get firmware version
	result = UARTSendPacket(testPacket2);
	UARTDebug("\r\n return = ", result);
	vTaskDelay(20);
	#endif

	while(1)
	{
		vTaskDelay(50);
		
		/****************************************
		* Handle data from TCM310
		****************************************/
		if(UARTBufferSize(2)>1)
		{
			toRead = UARTBufferSize(2);
		
			UARTRead(2,uread,toRead);
			
			#ifdef UART_DEBUG
			for(i=0; i< toRead-1;i++)
				UARTDebugHexa(uread[i]);
			#endif
			
			result = getPacket(&uread,toRead);
			UARTDebug("\r\n return = ", (int)result);
			UARTWrite(1,"\r\n");
			
			UARTFlush(2);
		}
		
		
		/****************************************
		* handle command from UART debug
		****************************************/
		CheckUART1();
		if(cmdNew == TRUE)
		{
			uartCommand = CheckCmds();
			if(uartCommand!=-1) //not valid command
			{
				switch(uartCommand)
				{
					case CMD_RPS_ON:	//"rps_on"
						flag_ON = TRUE;
						break;
					case CMD_RPS_OFF:	//"rps_off"
						flag_OFF = TRUE;
						break;
					case CMD_FW_VERSION: //"version"
						IOPut(d4out, toggle);
						UARTWrite(1,"\r\n - CO_RD_VERSION -");
						/*|dataLenght| opt |                      type               | data |*/
						UINT8 testPacket3[5] ={0x00, 0x01, 0x00,(UINT8)((ESP3_PACKET_TYPE)COMMON_COMMAND), 0x03 };
						result = UARTSendPacket(testPacket3);
						UARTDebug("\r\n return = ", result);
						break;
					case CMD_PING:	//ping
						ConsoleWrite("pong\r\n");
						break;
					case CMD_GET_ID:	//getID
						if(ID_LEARN_OK)
						{
							UARTWrite(1,"ID learned: ");
							for(i=0;i<4;i++)UARTDebugHexa(ID_learn[i]);
							UARTWrite(1,"\r\n");
						}else UARTWrite(1,"No ID in memory\r\n");
						break;
					default:
						break;
				}
			}
		}
		
		
		
		/****************************************
		* Flag ON
		* send an RPS telegram to switch ON the remote device
		****************************************/
		if(flag_ON)
		{
			flag_ON = FALSE;
			UARTSendPacket(TelegramRPS_ON);
			UARTWrite(1,"ON!\r\n");
		}//endif flag_ON
		
		/****************************************
		* Flag OFF
		* send an RPS telegram to switch OFF the remote device
		****************************************/
		if(flag_OFF)
		{
			flag_OFF = FALSE;
			UARTSendPacket(TelegramRPS_OFF);
			UARTWrite(1,"OFF!\r\n");
		}//endif flag_OFF
		
		/****************************************
		* Flag LEARN
		* enter in learn mode
		****************************************/
		if(flag_LEARN)	
		{
			if(ID_LEARN_OK)
			{
				UARTWrite(1,"An ID is already learned\r\n");
				flag_LEARN = FALSE; //TODO à changer
			}else
				UARTWrite(1,"Learnning...\r\n");
			
		} //endif flag_LEARN
		
		/****************************************
		* Flag CLEAR
		* clear the learned device
		****************************************/
		if(flag_CLEAR)
		{
			if(ID_LEARN_OK)	//if an ID is stored
			{
				UARTWrite(1,"Clear the ID: ");
				for(i=0;i<4;i++)
				{
					UARTDebugHexa(ID_learn[i]);
					ID_learn[i] = 0x00;	//reset the ID
				}
				UARTWrite(1,"\r\n");
				
				temperatureHexa = 0x00;
				
				ID_LEARN_OK = FALSE;
			}
			else
			{
				UARTWrite(1,"Nothing to CLEAR\r\n");
			}
			
			if(flag_LEARN)
			{
				UARTWrite(1,"STOP Learn\r\n");
				flag_LEARN = FALSE;
			}
			
			flag_CLEAR = FALSE;
		} //endif flag_CLEAR
		
	} //WHILE
} //taskFlyport



