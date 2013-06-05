#ifndef __EO_ESP3_H
#define __EO_ESP3_H

#include "tools.h"
#include "HWlib.h"
 
//compute the CRC8
#define proc_crc8(u8CRC, u8Data) (u8CRC8Table[u8CRC ^ u8Data]) 

//Enocean synchronization byte
#define ESP3_SYNC_BYTE 0x55
//size of the packet header defined in the ESP3 protocole
#define ESP3_HEADER_SIZE 4

extern UINT8 ID_learn[4];
extern BOOL ID_LEARN_OK;

extern BOOL flag_LEARN;
extern BOOL flag_CLEAR;
extern BOOL flag_new_data;
BOOL compareID(UINT8 *pBuffer, UINT8 *ID);
extern UINT8 temperatureHexa;

/**
* Packet types as describe in ESP3 protocol (§1.6.2)
*/
typedef enum
{
	RADIO = 0x01,
	RESPONSE = 0x02,
	RADIO_SUB_TEL =0x03,
	EVENT =0x04,
	COMMON_COMMAND = 0x05,
	SMART_ACK_COMMAND = 0x06,
	REMOTE_MAN_COMMAND = 0x07
}ESP3_PACKET_TYPE;

/*
typedef enum
{
	//TODO
}ESP3_DATA_LENGHT;
*/

typedef enum
{
	OPTDATA_RPS = 0x07
}ESP3_OPTDATA_LENGHT;


//Packet Type 2: RESPONSE
//list of return codes
//ESP3 page 15
/*
typedef enum
{
	RET_OK = 0x00,
	RET_ERROR = 0x01, 
	RET_NOT_SUPPORTED = 0x02,
	RET_WRONG_PARAM = 0x03,
	RET_OPERATION_DENIED = 0x04
}ESP3_RESPONSE;
*/

typedef enum
{
	RORG_RPS = 0xF6,
	RORG_1BS = 0xD5,
	RORG_4BS = 0xA5,
	RORG_VLD = 0xD2,
	RORG_MSC = 0xD1,
	RORG_ADT = 0xA6,
	RORG_SM_LRN_REQ = 0xC6,
	RORG_SM_LRN_ANS = 0xC7, 
	RORG_SM_REC = 0xA7,
	RORG_SYS_EX = 0xC5
}ESP3_RORG;

/**
* Return Type for Enocean function
*/
/*
typedef enum
{
	OUT_OF_RANGE = 0,
	OK = 1,
	BAD_CRC = 2,
	SYNC_ERROR =3,
}RETURN_TYPE;
*/

typedef UINT8 uint8  ;
typedef UINT16 uint16;

/*****************FROM EO3000I_API.h*****************/
#define SER_SYNCH_CODE 0x55
#define SER_HEADER_NR_BYTES 0x04

//! Packet structure (ESP3)
typedef struct
{
	uint16	u16DataLength;	         //! Amount of raw data bytes to be received. The most significant byte is sent/received first
	uint8	u8OptionLength;			 //! Amount of optional data bytes to be received
	uint8	u8Type;					 //! Packe type code
	uint8	*u8DataBuffer;			 //! Packe type code

} PACKET_SERIAL_TYPE;

//! Packet type (ESP3)
typedef enum
{
	PACKET_RESERVED 			= 0x00,	//! Reserved
	PACKET_RADIO 				= 0x01,	//! Radio telegram
	PACKET_RESPONSE				= 0x02,	//! Response to any packet
	PACKET_RADIO_SUB_TEL		= 0x03,	//! Radio subtelegram (EnOcean internal function )
	PACKET_EVENT 				= 0x04,	//! Event message
	PACKET_COMMON_COMMAND 		= 0x05,	//! Common command
	PACKET_SMART_ACK_COMMAND	= 0x06,	//! Smart Ack command
	PACKET_REMOTE_MAN_COMMAND	= 0x07,	//! Remote management command
	PACKET_PRODUCTION_COMMAND	= 0x08,	//! Production command
	PACKET_RADIO_MESSAGE		= 0x09	//! Radio message (chained radio telegrams)
} PACKET_TYPE;

//! Response type
typedef enum
{
	RET_OK 					= 0x00, //! OK ... command is understood and triggered
	RET_ERROR 				= 0x01, //! There is an error occured
	RET_NOT_SUPPORTED 		= 0x02, //! The functionality is not supported by that implementation
	RET_WRONG_PARAM 		= 0x03, //! There was a wrong parameter in the command
	RET_OPERATION_DENIED 	= 0x04, //! Example: memory access denied (code-protected)
	RET_USER				= 0x80	//! Return codes greater than 0x80 are used for commands with special return information, not commonly useable.
} RESPONSE_TYPE;

//! Common command enum
typedef enum
{
	CO_WR_SLEEP			= 1,	//! Order to enter in energy saving mode
	CO_WR_RESET			= 2,	//! Order to reset the device
	CO_RD_VERSION		= 3,	//! Read the device (SW) version / (HW) version, chip ID etc.
	CO_RD_SYS_LOG		= 4,	//! Read system log from device databank
	CO_WR_SYS_LOG		= 5,	//! Reset System log from device databank
	CO_WR_BIST			= 6,	//! Perform Flash BIST operation
	CO_WR_IDBASE		= 7,	//! Write ID range base number
	CO_RD_IDBASE		= 8,	//! Read ID range base number
	CO_WR_REPEATER		= 9,	//! Write Repeater Level off,1,2
	CO_RD_REPEATER		= 10,	//! Read Repeater Level off,1,2
	CO_WR_FILTER_ADD	= 11,	//! Add filter to filter list
	CO_WR_FILTER_DEL	= 12,	//! Delete filter from filter list
	CO_WR_FILTER_DEL_ALL= 13,	//! Delete filters
	CO_WR_FILTER_ENABLE	= 14,	//! Enable/Disable supplied filters
	CO_RD_FILTER		= 15,	//! Read supplied filters
	CO_WR_WAIT_MATURITY	= 16,	//! Waiting till end of maturity time before received radio telegrams will transmitted
	CO_WR_SUBTEL		= 17,	//! Enable/Disable transmitting additional subtelegram info
	CO_WR_MEM			= 18,	//! Write x bytes of the Flash, XRAM, RAM0 ….
	CO_RD_MEM			= 19,	//! Read x bytes of the Flash, XRAM, RAM0 ….
	CO_RD_MEM_ADDRESS	= 20,	//! Feedback about the used address and length of the config area and the Smart Ack Table
	CO_RD_SECURITY		= 21,	//! Read security informations (level, keys)
	CO_WR_SECURITY		= 22,	//! Write security informations (level, keys)
} COMMON_COMMAND_TYPE; 

//! Function return codes
typedef enum
{
	//! <b>0</b> - Action performed. No problem detected
	OK=0,							
	//! <b>1</b> - Action couldn't be carried out within a certain time.  
	TIME_OUT,		
	//! <b>2</b> - The write/erase/verify process failed, the flash page seems to be corrupted
	FLASH_HW_ERROR,				
	//! <b>3</b> - A new UART/SPI byte received
	NEW_RX_BYTE,				
	//! <b>4</b> - No new UART/SPI byte received	
	NO_RX_BYTE,					
	//! <b>5</b> - New telegram received
	NEW_RX_TEL,	  
	//! <b>6</b> - No new telegram received
	NO_RX_TEL,	  
	//! <b>7</b> - Checksum not valid
	NOT_VALID_CHKSUM,
	//! <b>8</b> - Telegram not valid  
	NOT_VALID_TEL,
	//! <b>9</b> - Buffer full, no space in Tx or Rx buffer
	BUFF_FULL,
	//! <b>10</b> - Address is out of memory
	ADDR_OUT_OF_MEM,
	//! <b>11</b> - Invalid function parameter
	NOT_VALID_PARAM,
	//! <b>12</b> - Built in self test failed
	BIST_FAILED,
	//! <b>13</b> - Before entering power down, the short term timer had timed out.	
	ST_TIMEOUT_BEFORE_SLEEP,
	//! <b>14</b> - Maximum number of filters reached, no more filter possible
	MAX_FILTER_REACHED,
	//! <b>15</b> - Filter to delete not found
	FILTER_NOT_FOUND,
	//! <b>16</b> - BaseID out of range
	BASEID_OUT_OF_RANGE,
	//! <b>17</b> - BaseID was changed 10 times, no more changes are allowed
	BASEID_MAX_REACHED,
	//! <b>18</b> - XTAL is not stable
	XTAL_NOT_STABLE,
	//! <b>19</b> - No telegram for transmission in queue  
	NO_TX_TEL,
	//!	<b>20</b> - Waiting before sending broadcast message
	TELEGRAM_WAIT,
	//!	<b>21</b> - Generic out of range return code
	OUT_OF_RANGE,
	//!	<b>22</b> - Function was not executed due to sending lock
	LOCK_SET,
	//! <b>23</b> - New telegram transmitted
	NEW_TX_TEL
} RETURN_TYPE;

/*********************************************/

RETURN_TYPE UARTWriteBuffer(UINT8 *buf, UINT32 length);
RETURN_TYPE UARTSendPacket(UINT8 *pBuffer);
RETURN_TYPE getPacket(UINT8 *pBuffer, UINT32 length) ;

BOOL compareID(UINT8 *pBuffer, UINT8 *ID);

RETURN_TYPE parseRadio(UINT8 *pBuffer, UINT32 length);
RETURN_TYPE parseResponse(UINT8 *pBuffer, UINT32 length);
RETURN_TYPE parseRadioSubTel(UINT8 *pBuffer, UINT32 length);
RETURN_TYPE parseEvent(UINT8 *pBuffer, UINT32 length);
RETURN_TYPE parseCommonCommand(UINT8 *pBuffer, UINT32 length);
RETURN_TYPE parseSmartAckCommand(UINT8 *pBuffer, UINT32 length);
RETURN_TYPE parseRemoteManCommand(UINT8 *pBuffer, UINT32 length);


#endif 
