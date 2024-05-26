#ifndef __DEFINES_H__
#define __DEFINES_H__
 
#include "main.h"


#define MAX_LEN_COM_PORT_SEND (250)

//-----------
//BUTTONS
#define BUTTONS_DEBOUNCE_TIME_MS (3)
//-----------
#define ESP_ON_TIME_MS (30000)

//UART CMD
#define RX_CMD_BUFF_SIZE (100)
#define TX_ANS_BUFF_SIZE (2400)
#define TIME_FOR_DEFINE_LAST_BYTE_MS (2)  
#define UART_CMD_PROCESSING_TIME_MS  (1) 
//-----------

#define DEBUG_ESP_ALL_TIME_WORK (0)

#define REPIAT_INTERVAL (100) // 100 => 1 min, 00 sec 
#define DEBUG_INCREMENT_MODE (1)

#endif 