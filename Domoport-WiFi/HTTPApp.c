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
 
 
/****************************************************************************
  SECTION 	Include
****************************************************************************/

#include "TCPIP Stack/TCPIP.h"
#include "TCPIPConfig.h"
#if defined(STACK_USE_HTTP2_SERVER)
#define __HTTPAPP_C

extern BOOL flag_ON;
extern BOOL flag_OFF;
BOOL flag_LED = FALSE;
BOOL flag_LEARN = FALSE;
BOOL flag_CLEAR = FALSE;
extern BOOL ID_LEARN_OK;
extern BOOL flag_new_data;
extern UINT8 temperatureHexa;
 
/****************************************************************************
  FUNCTION	HTTP_IO_RESULT HTTPExecuteGet(void)
	
  This function processes every GET request from the pages. In the example, 
  it processes only the leds.cgi function, but you can add code to process 
  other GET requests.
*****************************************************************************/
HTTP_IO_RESULT HTTPExecuteGet(void)
{
	BYTE *ptr;
	BYTE filename[20];
	
	// STEP #1:
	// The function MPFSGetFilename retrieves the name of the requested cgi,
	// in this case "leds.cgi" and puts it inside the filename variable.
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, filename, 20);

	// STEP #2:
	// Handling of the cgi requests, in this case we have only "leds.cgi" but
	// it would be possible to have any other cgi request, depending on the webpage
	
	if(!memcmp(filename, "leds.cgi", 8))		// Is the requested file name "leds.cgi"?
	{
		// STEP #3:
		// The complete request is contained inside the system variable curHTTP.data.
		// Using the function HTTPGetArg is possible to read the arguments
		// of the cgi request from curHTTP.data. In this case we are reading the 
		// argument "led" from the request "leds.cgi?led=x" and we assign it to ptr.
		
		ptr = HTTPGetArg(curHTTP.data, (BYTE *)"led");
		
		// The requested led is toggled
		switch(*ptr) 
		{
			case '0':
				if(flag_LED) flag_LED = FALSE; else flag_LED = TRUE;
				if(flag_LED) flag_ON = TRUE; else flag_OFF = TRUE;
				break;
			default:
				break;
		}
		
	}
	
	
	//	Checks if the filename is "buttons.cgi"
	if(!memcmppgm2ram(filename, "buttons.cgi",11))
	{
		//	Now reads the value of the btn request
		ptr = HTTPGetROMArg(curHTTP.data, (ROM BYTE *)"btn");

		switch(*ptr) 
		{
			case '1': //the button LEARN has been pressed
				flag_LEARN = TRUE;
				break;
			case '2':
				flag_CLEAR = TRUE;	//the button CLEAR has been pressed
				break;
			default:
				break;
		}		
	}
	
	
	return HTTP_IO_DONE;
}


/****************************************************************************
  Section:
	POST Form Handlers
  ***************************************************************************/
#if defined(HTTP_USE_POST)

/*****************************************************************************
  Function:
	HTTP_IO_RESULT HTTPExecutePost(void)

  	This function processes every GET request from the pages. 
  ***************************************************************************/
HTTP_IO_RESULT HTTPExecutePost(void)
{
	// Resolve which function to use and pass along
	BYTE filename[20];

	// Load the file name
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, filename, sizeof(filename));

	return HTTP_IO_DONE;
}

#endif 


/***************************************************************************
  SECTION	Dynamic Variable Callback Functions
  
  In this section are managed the "dynamic variables" of the webserver.
  Dynamic variables are contained in the status.xml file requested by the 
  webpage. 
  For each dynamic variable a callback function named HTTPPrint_varname 
  must be created.
****************************************************************************/

// Callback function for the dynamic variable ~pot(num)~. Anytime the browser
// asks for ~pot(num)~, this function is executed.
void HTTPPrint_pot(WORD num)
{
	char AN0String[4];
	float ADval;

	// Analog values reading according to the webpage request.
	switch(num)
	{
		case 0:
			if(temperatureHexa == 0x00)
				ADval = 0;
			else
			{
				ADval = 40-0.16* temperatureHexa; //40/255
			}
			if(!ID_LEARN_OK) ADval = 0.0; //set temperature to 0°C when no device learned
			
			sprintf(AN0String, "%2.2f", ADval); //format exemple: 17.16; 15.44; 9.12; ...
			#ifdef UART_DEBUG
			if(flag_new_data)
			{
				UARTDebug("HEXA TEMP: ",temperatureHexa);
				UARTWrite(1, "tempHTTP: ");
				UARTWrite(1, AN0String);
				UARTWrite(1, "\r\n");
				flag_new_data = FALSE;
			}
			#endif
			break;
		default: 
			break;
	}

	// After the analog value is read, it is sent to the webpage
	// using the function TCPPutString. sktHTTP is the standard
	// socket for the HTTP communication.
   	TCPPutString(sktHTTP, AN0String);
}


// Callback function for the dynamic variable ~learn(num)~.
void HTTPPrint_learn(WORD num)
{
	// Determine which LED
	switch(num)
	{
		case 0:
			num = flag_LEARN;
			break;
		case 1:
			num = ID_LEARN_OK;
			break;
		default:
			num = 0;
			break;
	}

	// Sending back the led status to the webpage.
	TCPPut(sktHTTP, (num?'1':'0'));
	return;
}
	
// Callback function for the dynamic variable ~led(num)~.
void HTTPPrint_led(WORD num)
{
	// Determine which LED
	switch(num)
	{
		case 0:
			num = flag_LED;
			break;
		default:
			num = 0;
			break;
	}

	// Sending back the led status to the webpage.
	TCPPut(sktHTTP, (num?'1':'0'));
	return;
}

// Callback function for the dynamic variable ~btn(num)~.
void HTTPPrint_btn(WORD num)
{
	return;
}

/****************************************************************************
  SECTION 	Authorization Handlers
****************************************************************************/
 

/*****************************************************************************
  FUNCTION	BYTE HTTPNeedsAuth(BYTE* cFile)

  This function is used by the stack to decide if a page is access protected.
  If the function returns 0x00, the page is protected, if returns 0x80, no 
  authentication is required
*****************************************************************************/
#if defined(HTTP_USE_AUTHENTICATION)
BYTE HTTPNeedsAuth(BYTE* cFile)
{
	//	If you want to restrict the access to some page, include it in the folder "protect"
	//	here you can change the folder, or add others
	if(memcmp(cFile, (void*)"protect", 7) == 0)
		return 0x00;		// Authentication will be needed later

	return 0x80;			// No authentication required
}
#endif

/*****************************************************************************
  FUNCTION	BYTE HTTPCheckAuth(BYTE* cUser, BYTE* cPass)
	
  This function checks if username and password inserted are acceptable

  ***************************************************************************/
#if defined(HTTP_USE_AUTHENTICATION)
BYTE HTTPCheckAuth(BYTE* cUser, BYTE* cPass)
{
	if(strcmp((char *)cUser,(char *)"admin") == 0
		&& strcmp((char *)cPass, (char *)"flyport") == 0)
		return 0x80;		// We accept this combination

	return 0x00;			// Provided user/pass is invalid
}
#endif

#endif
