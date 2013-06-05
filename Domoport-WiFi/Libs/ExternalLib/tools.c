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
 *
 **************************************************************************/
 
#ifndef __TOOLS_C_
#define __TOOLS_C_

#include "tools.h"
#include "HWlib.h"

//Display in the UART, the size of the type
void UART_SizeOf()
{
	char buf[8];
	
	UARTWrite(1,"\r\nchar = ");
    sprintf(buf, "%d  ", sizeof(char));
    UARTWrite(1,buf);
	
	UARTWrite(1,"\r\nunsigned char = ");
    sprintf(buf, "%d  ", sizeof(UINT8));
    UARTWrite(1,buf);
	
	UARTWrite(1,"\r\nint = ");
    sprintf(buf, "%d  ", sizeof(int));
    UARTWrite(1,buf);
	
	UARTWrite(1,"\r\nlong = ");
    sprintf(buf, "%d  ", sizeof(long));
    UARTWrite(1,buf);
	
	UARTWrite(1,"\r\nunsigned int = ");
    sprintf(buf, "%d  ", sizeof(UINT));
    UARTWrite(1,buf);
	
	UARTWrite(1,"\r\nunsigned short int = ");
    sprintf(buf, "%d  ", sizeof(UINT16));
    UARTWrite(1,buf);
	
	UARTWrite(1,"\r\n");
}

/**
* This function display an unsigned integer in the UART in hexadecimal
* \param str - String to display in the UART
* \param val - UINT value to display
*/
void UARTDebug(char* str, unsigned int val)
{
	UINT8 buf[4];
	
	UARTWrite(1,str);
	sprintf(buf, "%#x", val);
	//sprintf(buf, "%d  ",  val);
	UARTWrite(1,buf);
	
}

/**
* This function display an unsigned integer in the UART in hexadecimal
* \param val - UINT value to display in Hexa
*/
void UARTDebugHexa(unsigned int val)
{
	if(val == 0)
	{
		UARTWrite(1,"0x00 ");
	}
	else
	{
		UINT8 buf[4];
		sprintf(buf, "%#x ", val);
		UARTWrite(1,buf);
	}
}

/**
* This function will send a string and a value in the UART1
*/
void UARTDebug32(char* str, unsigned long int val)
{
	UINT32 buf[4];
	
	UARTWrite(1,str);
	vTaskDelay(2);
	sprintf(buf, " %ld  ",  val);
	UARTWrite(1,buf);
	
}

/*
* Generate a random integer between a min and max value
* @param _iMin: minimum integer value
* @param _iMax: maximum integer value
* @return random integer
*/
int Random (int _iMin, int _iMax)
{
	return (_iMin + (rand () % (_iMax-_iMin+1)));
} 


#endif 
