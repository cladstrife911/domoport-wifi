#include "console.h"

char u_cmd[125];
BOOL cmdNew = FALSE;
BOOL cmdTCP = FALSE;
int len = 0;

TCP_SOCKET tcpSocket = INVALID_SOCKET;
char tcpPort[] = "50800";
BOOL tcpConn = FALSE;
void CheckTCP();
void CheckUART1();

void AppDebug(char* txt)
{
	#ifdef STACK_USE_UART
	UARTWrite(1,txt);
	#endif
}

void ConsoleWrite(char* txt)
{
	#ifdef UART_CONSOLE
	UARTWrite(1,txt);
	#endif
	
	#ifdef TCP_CONSOLE
	if(tcpConn == TRUE)
	{
		TCPWrite(tcpSocket, txt, strlen(txt));
	}
	#endif
}

void CheckTCP()
{
	// Check TCP Server connection state
	if(tcpSocket == INVALID_SOCKET)
	{
		tcpSocket = TCPServerOpen(tcpPort);
		IOPut(p19, off);
		cmdTCP = FALSE;
	}
	else
	{
		tcpConn = TCPisConn(tcpSocket);
		IOPut(p19, on);
		cmdTCP = TRUE;
	}
	
	// Check TCPRxLen...
	if(tcpConn == TRUE)
	{
		if(TCPRxLen(tcpSocket) > 2)
		{
			AppDebug("TCP received:");
			len = TCPRxLen(tcpSocket);
			TCPRead(tcpSocket, u_cmd, len);
			u_cmd[len] = '\0';
			AppDebug(u_cmd);
			AppDebug("\r\n");
			cmdNew = TRUE;
		}
	}
}

void CheckUART1()
{
	// Check UART1 Commands
	int tmpLen = UARTBufferSize(1);
	if(tmpLen > 3)
	{
		vTaskDelay(5);
		len = UARTBufferSize(1);
		UARTRead(1, u_cmd, len);
		u_cmd[len] = '\0';
		cmdNew = TRUE;
		UARTFlush(1);
	}
}
