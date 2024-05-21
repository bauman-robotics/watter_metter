#include "main.h"
#include "uartProcessing.h"
#include <stdlib.h>

#define CMD_CODE_STRING_LEN 3
#define CMD_CODE_LEN_MAX 20
#define CMD_PARAM_LEN sizeof(float);

cmd_s_type rx_cmd;
lastByteDefStype lastByteCheck;

extern watter_type watter;
extern UART_HandleTypeDef huart2;

static char buf[RX_CMD_BUFF_SIZE + 10];

int CheckCmdCodeLen(cmd_s_type * rx_cmd);
uint32_t GetCmdParam(char * cmdParam);
bool CheckIsParamValid(uint8_t * cmdParam, int cmdParamlen);
void Send_Cmd_Code_Zero_Len_Answer(void);
void Send_Unknown_Cmd_Code_Answer(char * cmdCode);
void StartUartLog(cmd_s_type * rx_cmd);

void SetDate(uint32_t iDate);
void Send_Time(void); 

char log_str0[70];
char log_str1[70]; 
char log_str2[130];
char log_str3[70];
char log_str4[70];
char log_str5[70];
char log_str6[70];
char log_str7[70];
char log_str8[70];
char log_str9[70];
char log_str10[70];
char log_str11[70];
char log_str12[70];
char log_str13[70];
char log_str14[70];
char log_str15[70];
char log_str16[70];
char log_str17[70];
char log_str18[70];
char log_str19[70];
char log_str20[70];
char log_str21[70];
char log_str22[70];
char log_str23[70];
char log_str24[70];
char log_str25[70];
char log_str26[70];
char log_str27[70];
char log_str28[70];
char log_str29[70];
char log_str30[70];
char log_str31[70];
char log_str32[70];
char log_str33[70];
char log_str34[70];
char log_str35[70];
char log_str36[70];
char log_str37[70];
char log_str38[70];
char log_str39[70];
char log_str40[70];
char log_str41[70];

// char strWho[] = "Who"; // identification of port Cmd
// char strDsk[] = "Dsk"; // Ask Start Count

char strSetTime[]      = "set_time";  
char strSetDate[]      = "set_date"; 
char strSetAlarmTime[] = "set_alarm_time";
char strSetAlarmDate[] = "set_alarm_date";

char strGetTime[]      = "get_time";
char strGetDate[]      = "get_date"; 
char strGetAlarmTime[] = "get_alarm_time";
char strGetAlarmDate[] = "get_alarm_date";

//====================================
void Send_Ok_Answer_Param(char * cmdCode, int iCmdParam) {
	sprintf(buf, "%s%s%d%c" , "Cmd Ok -- ",  cmdCode, iCmdParam, 13);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
} 
//====================================

void Send_Ok_Answer(char * cmdCode) {	
	sprintf(buf, "%s%s%c" , "Cmd Ok -- ",  cmdCode, 13);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
} 
//====================================

void Send_Cmd_Code_Zero_Len_Answer(void) {
	sprintf(buf, "%s%c" , "The command code has zero length", 13);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
} 
//====================================

void Send_Unknown_Cmd_Code_Answer(char * cmdCode) {
	if (strlen(cmdCode) > 0) {
		sprintf(buf, "%s%s%c" , "Unknown Cmd code --",  cmdCode, 13);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
} 
//====================================

void CmdRxProcessing(cmd_s_type * rx_cmd, watter_type *wattere) {    
	char cmdCode[CMD_CODE_LEN_MAX] = {0};
	char cmdParam[RX_CMD_BUFF_SIZE] = {0};
	uint32_t iCmdParam = 0;
	int cmdCodeLen = 0;
	uint8_t packetLen = 0;
	//bool err_flag = 1;
	uint8_t * pCmdParam = 0;
	int cmdParamLen = 0;
	bool cmdParamValid = 0;

	if (rx_cmd->f_cmd_received) {
		
       //=== CMD code ===============================================
		
		packetLen = rx_cmd->num_bytes_received;	
		
		cmdCodeLen = CheckCmdCodeLen(rx_cmd);	                       //======
		if (cmdCodeLen == 0)  {
			Send_Cmd_Code_Zero_Len_Answer();
			rx_cmd->f_cmd_received = 0;	
			return;
		} else {
			memcpy(cmdCode, rx_cmd->rx_ready_buffer, cmdCodeLen);		
			cmdCode[cmdCodeLen]	= 0;		
		}
		// cmdCode[]
		// cmdCodeLen
		// ===========================================================
		
		//=== CMD Param ============
		
		pCmdParam = rx_cmd->rx_ready_buffer + cmdCodeLen;
		cmdParamLen = packetLen - cmdCodeLen;
		if (cmdParamLen > 0) {
			cmdParamValid = CheckIsParamValid(pCmdParam, cmdParamLen); //======
		}
		if ( (cmdParamLen > 0) && (cmdParamValid)) {
			memcpy(cmdParam, rx_cmd->rx_ready_buffer + cmdCodeLen, cmdParamLen);
			iCmdParam = GetCmdParam(cmdParam);		
		}
		// cmdParam[]
		// cmdParamLen					
		// ===========================================================
		
		if ((strncmp(cmdCode, strSetTime, cmdCodeLen) == 0) && (cmdParamValid == 1)) { 
			
			Send_Ok_Answer_Param(cmdCode, iCmdParam);
			rx_cmd->f_cmd_received = 0;	
			return;
		} 
		//==========	
		if ((strncmp(cmdCode, strSetDate, cmdCodeLen) == 0) && (cmdParamValid == 1)) { 
		
			Send_Ok_Answer_Param(cmdCode, iCmdParam);
			rx_cmd->f_cmd_received = 0;	
			SetDate(iCmdParam);
			return;
		} 
		//========== No Param ====
		if ((strncmp(cmdCode, strGetTime, cmdCodeLen) == 0) && (cmdParamLen == 0)) { 
		
			Send_Ok_Answer(cmdCode);
			Send_Time();
			
			rx_cmd->f_cmd_received = 0;	
			return;
		}
		//========== No Param ====	
		if ((strncmp(cmdCode, strGetDate, cmdCodeLen) == 0) && (cmdParamLen == 0)) { 
		
			Send_Ok_Answer(cmdCode);
			rx_cmd->f_cmd_received = 0;	
			return;	
		}
		Send_Unknown_Cmd_Code_Answer(cmdCode);			
		rx_cmd->f_cmd_received = 0;	
	}
	DefinitionLastRxByte(rx_cmd);
}
//====================================

void StartUartLog(cmd_s_type * rx_cmd) {
	sprintf(log_str1,  "%s%c" 							,"--------------------", 13);
	sprintf(log_str2,  "%s%c" 							, "CMD available:", 13);
	sprintf(log_str3,  "%s%c" 							,"--------------------", 13);	
	sprintf(log_str4,  "%s%c" 						, "set_date{year mounth day}", 13);		
	sprintf(log_str5,  "%s%c" 						, "example: set_date240521", 13);		
	sprintf(log_str6,  "%s%c" 							,"--------------------", 13);
	sprintf(log_str7,  "%s%c" 						, "set_time{hour min sec}", 13);	
	sprintf(log_str8,  "%s%c" 						, "example: set_time183630", 13);		
	sprintf(log_str9,  "%s%c" 							,"--------------------", 13);
	sprintf(log_str10, "%s%c" 						, "get_date", 13);	
	sprintf(log_str11, "%s%c" 						, "get_time", 13);	
	sprintf(log_str12, "%s%c" 							,"--------------------", 13);
	sprintf(rx_cmd->txBuff, "%s%s%s%s%s%s%s%s%s%s%s%s",
		log_str1,
		log_str2,
		log_str3,
		log_str4,	
		log_str5,
		log_str6,
		log_str7,
		log_str8,
		log_str9,
		log_str10,	
		log_str11,
		log_str12
	);
	rx_cmd->txBuffLen = strlen(rx_cmd->txBuff);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)rx_cmd->txBuff, rx_cmd->txBuffLen);	
}

//====================================

int CheckCmdCodeLen(cmd_s_type * rx_cmd) {
	uint8_t packetLen = rx_cmd->num_bytes_received;
	uint8_t charCmd = 0;
	uint8_t cmdCodeLen = 0;
	for (int n = 0; n < packetLen; n ++) {
		charCmd = rx_cmd->rx_ready_buffer[n];
		if ((charCmd >= '_') &&  (charCmd <= 'z')) {
			cmdCodeLen ++;
		} else {
			break;
		}
	}
return (int)cmdCodeLen;
}
//====================================

bool CheckIsParamValid(uint8_t * cmdParam, int cmdParamlen) {

	uint8_t charCmdParam = 0;
	bool cmdParamValid = 1;
	
	for (int n = 0; n < cmdParamlen; n ++) {
		charCmdParam = cmdParam[n];
		if ((charCmdParam < '0') && (charCmdParam > '9')) {
			cmdParamValid = 0;
		} 
	}
return cmdParamValid;
}
//====================================

uint32_t GetCmdParam(char * cmdParam) {
	uint32_t iParam = 0;
	iParam = atoi(cmdParam);
	
return iParam;
}
//====================================

void ReadNextUartByte(cmd_s_type * rx_cmd) {		
	rx_cmd->f_next_byte_received = 1; 
	rx_cmd->rx_ready_buffer[rx_cmd->rx_byte_counter] = USART2->DR;
	if (rx_cmd->rx_byte_counter > RX_CMD_BUFF_SIZE) {
			rx_cmd->rx_byte_counter = 0;
			memset(rx_cmd->rx_ready_buffer, 0, RX_CMD_BUFF_SIZE);
			return;
	}
	rx_cmd->rx_byte_counter++;
	lastByteCheck.lastCounter = HAL_GetTick(); //sysCount;
	lastByteCheck.startFindLastByte = 1;
}
//====================================

void DefinitionLastRxByte(cmd_s_type * rx_cmd) {
	lastByteCheck.currentCounter = HAL_GetTick(); //sysCount;
	if ((lastByteCheck.startFindLastByte) && (lastByteCheck.currentCounter > lastByteCheck.lastCounter + TIME_FOR_DEFINE_LAST_BYTE_MS)) {
			rx_cmd->f_cmd_received = 1;
			rx_cmd->num_bytes_received = rx_cmd->rx_byte_counter; 
			memset(rx_cmd->rx_ready_buffer + rx_cmd->num_bytes_received, 0, RX_CMD_BUFF_SIZE - rx_cmd->num_bytes_received);
			rx_cmd->f_next_byte_received = 0;
			rx_cmd->rx_byte_counter = 0;	
			lastByteCheck.startFindLastByte = 0;
		  HAL_UART_Receive_IT(&huart2, (uint8_t*)rx_cmd->rx_buff, 15);	
	}
	//HAL_UART_Receive_IT(&huart2, (uint8_t*)rx_cmd->rx_buff, 15);
}
//====================================