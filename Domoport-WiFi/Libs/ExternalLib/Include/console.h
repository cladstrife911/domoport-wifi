#include "HWlib.h"
#include "TCPlib.h"

#ifndef _CONSOLE_H
#define _CONSOLE_H

#define UART_CONSOLE
#define TCP_CONSOLE

extern char u_cmd[];
extern BOOL cmdNew;
extern BOOL cmdTCP;
extern int len;

extern TCP_SOCKET tcpSocket;
extern char tcpPort[];
extern BOOL tcpConn;
extern void CheckTCP();
extern void CheckUART1();

extern void AppDebug(char* txt);
extern void ConsoleWrite(char* txt);

#endif //_CONSOLE_H
