#ifndef __UART_PROCESSING_H__
#define __UART_PROCESSING_H__

#include "main.h"

void ReadNextUartByte(cmd_s_type * rx_cmd);
void DefinitionLastRxByte(cmd_s_type * rx_cmd);
void CmdRxProcessing(cmd_s_type * rx_cmd, watter_type *watter);
void StartUartLog(cmd_s_type * rx_cmd);
#endif