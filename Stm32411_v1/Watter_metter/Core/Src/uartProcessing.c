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
void SetTime(uint32_t iTime);
void Set_Alarm_Time(uint32_t iTime);

void Set_ESP_On_Time_ms(uint32_t iTime);
void Get_ESP_On_Time_ms(void);

void Send_Hot_Ticks(void);
void Send_Cold_Ticks(void);

void Send_Date_Time(void); 
void Send_Alarm_Time(void);	


char log_str[30][80];

char strSetTime[]       = "set_time";  
char strSetDate[]       = "set_date"; 
char strSetAlarmTime[]  = "set_alarm_time";
char strSetAlarmDate[]  = "set_alarm_date";
char strSetEspOnTime[]  = "set_esp_on_ms";	
			    
char strGetDateTime[]   = "get_time";
char strGetAlarmTime[]  = "get_alarm_time";
char strGetAlarmDate[]  = "get_alarm_date";
char strGetEspOnTime[]  = "get_esp_on_ms";	

char strGetHotTicks[]   = "get_hot"; 
char strGetColdTicks[] = "get_cold"; 
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
		
		// ==== Set Time =======================================================		
		if ((strncmp(cmdCode, strSetTime, cmdCodeLen) == 0) && (cmdParamValid == 1)) { 			
			Send_Ok_Answer_Param(cmdCode, iCmdParam);
			rx_cmd->f_cmd_received = 0;
			SetTime(iCmdParam);	
			//Send_Date_Time();	
			return;
		} 
		
		// ==== Set Date =======================================================
		if ((strncmp(cmdCode, strSetDate, cmdCodeLen) == 0) && (cmdParamValid == 1)) { 
			Send_Ok_Answer_Param(cmdCode, iCmdParam);
			rx_cmd->f_cmd_received = 0;	
			SetDate(iCmdParam);
			return;
		} 
		
		// ==== Set Alarm Time =================================================
		if ((strncmp(cmdCode, strSetAlarmTime, cmdCodeLen) == 0) && (cmdParamValid == 1)) { 

			Send_Ok_Answer_Param(cmdCode, iCmdParam);
			rx_cmd->f_cmd_received = 0;	
			Set_Alarm_Time(iCmdParam);
			return;
		} 		
		// ==== Set ESP ON Time =================================================
		if ((strncmp(cmdCode, strSetEspOnTime, cmdCodeLen) == 0) && (cmdParamValid == 1)) { 

			Send_Ok_Answer_Param(cmdCode, iCmdParam);
			rx_cmd->f_cmd_received = 0;	
			Set_ESP_On_Time_ms(iCmdParam);
			return;
		} 	

		// ==== Get ESP ON Time === No Param ====================================
		if ((strncmp(cmdCode, strGetEspOnTime, cmdCodeLen) == 0) && (cmdParamValid == 0)) { 

			rx_cmd->f_cmd_received = 0;	
			Get_ESP_On_Time_ms();
			return;
		} 		
		
		//=== Get Time ======= No Param ========================================
		if ((strncmp(cmdCode, strGetDateTime, cmdCodeLen) == 0) && (cmdParamLen == 0)) { 
			Send_Date_Time();			
			rx_cmd->f_cmd_received = 0;	
			return;
		}
		//=== Get Alarm Time ======= No Param ========================================
		if ((strncmp(cmdCode, strGetAlarmTime, cmdCodeLen) == 0) && (cmdParamLen == 0)) { 
			Send_Alarm_Time();			
			rx_cmd->f_cmd_received = 0;	
			return;
		}				
		//=== Get Hot Ticks == No Param ========================================
		if ((strncmp(cmdCode, strGetHotTicks, cmdCodeLen) == 0) && (cmdParamLen == 0)) { 
			Send_Hot_Ticks();
			rx_cmd->f_cmd_received = 0;	
			return;	
		}	
		//=== Get Cold Ticks == No Param ========================================
		if ((strncmp(cmdCode, strGetColdTicks, cmdCodeLen) == 0) && (cmdParamLen == 0)) { 
			Send_Cold_Ticks();
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
	int n = 0;
	sprintf(log_str[n++],  "%s%c"   ,"--------------------", 13);          //  1
	sprintf(log_str[n++],  "%s%c"   , "CMD available:", 13);               //  2
	sprintf(log_str[n++],  "%s%c"   ,"--------------------", 13);          //  3
	sprintf(log_str[n++],  "%s%c"   , "set_date{year mounth day}", 13);    //  4
	sprintf(log_str[n++],  "%s%c"   , "example: set_date240521", 13);      //  5
	sprintf(log_str[n++],  "%s%c"   ,"--------------------", 13);          //  6
	sprintf(log_str[n++],  "%s%c"   , "set_time{hour min sec}", 13);       //  7
	sprintf(log_str[n++],  "%s%c"   , "example: set_time183630", 13);      //  8
	sprintf(log_str[n++],  "%s%c"   ,"--------------------", 13);          //  9
	sprintf(log_str[n++],  "%s%c"   , "set_alarm_time{hour min sec}", 13); // 10
	sprintf(log_str[n++],  "%s%c"   , "example: set_alarm_time22301", 13); // 11	
	sprintf(log_str[n++],  "%s%c"   ,"--------------------", 13);          // 12
	sprintf(log_str[n++],  "%s%c"   , "set_esp_on_ms", 13);                // 13
	sprintf(log_str[n++],  "%s%c"   , "example: set_esp_on_ms5000", 13);   // 14	
	sprintf(log_str[n++],  "%s%c"   ,"--------------------", 13);          // 15	
	sprintf(log_str[n++], "%s%c"   , "get_time", 13);                      // 16
	sprintf(log_str[n++], "%s%c"   ,"--------------------", 13);           // 17
	sprintf(log_str[n++], "%s%c"   , "get_hot", 13);                       // 18
	sprintf(log_str[n++], "%s%c"   ,"--------------------", 13);           // 19
	sprintf(log_str[n++], "%s%c"   , "get_cold", 13);                      // 20
	sprintf(log_str[n++], "%s%c"   ,"--------------------", 13);           // 21
	sprintf(log_str[n++], "%s%c"   , "get_alarm_time", 13);                // 22
	sprintf(log_str[n++], "%s%c"   ,"--------------------", 13);           // 23
	sprintf(log_str[n++], "%s%c"   , "get_esp_on_ms", 13);                 // 24	
	sprintf(log_str[n++], "%s%c"   ,"--------------------", 13);           // 25
	n = 0;                                                                
	sprintf(rx_cmd->txBuff, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",// 25 inch       
   
		log_str[n++],    //  1                                                    
		log_str[n++],    //  2                                                   
		log_str[n++],    //  3                                                   
		log_str[n++],    //  4
		log_str[n++],    //  5
		log_str[n++],    //  6
		log_str[n++],    //  7
		log_str[n++],    //  8
		log_str[n++],    //  9
		log_str[n++],    // 10
		log_str[n++],    // 11
		log_str[n++],    // 12
		log_str[n++],    // 13
		log_str[n++],    // 14
		log_str[n++],    // 15
		log_str[n++],    // 16
		log_str[n++],    // 17
		log_str[n++],    // 18
		log_str[n++],    // 19
		log_str[n++],    // 20
		log_str[n++],    // 21
		log_str[n++],    // 22
		log_str[n++],    // 23
		log_str[n++],    // 24
		log_str[n++]     // 25
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
}
//====================================