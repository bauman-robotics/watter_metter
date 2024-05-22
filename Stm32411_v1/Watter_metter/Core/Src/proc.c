#include <math.h>
#include "main.h"
#include "uartProcessing.h"

extern watter_type watter;
extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;

static char buf[MAX_LEN_COM_PORT_SEND] = {};
	
extern cmd_s_type rx_cmd;

void Processing(void);
uint32_t IntToHex(uint32_t iDate);
void Send_Date_Time(void); 
void Systick_Proc(void);
void ButtonsProcessing(void);
void SetDate(uint32_t iDate);
void SetTime(uint32_t iTime);
void Set_Alarm_Time(uint32_t iTime);
void Set_ESP_On_Time_ms(uint32_t iTime);

void Get_ESP_On_Time_ms(void);
void Send_Hot_Ticks(void);
void Send_Cold_Ticks(void);
void Send_Alarm_Time(void);	
	
void Esp_Power_On_For_N_ms(void);
void Esp_Controller(void); 
void ESP_Force_On(void);
void ESP_Force_Off(void);
//====================================
	
RTC_AlarmTypeDef sAlarm;  // have to be Global  !!!!!

//====================================

uint32_t IntToHex(uint32_t iDate) {
	char buf[10];
	uint8_t byte = 0;
	uint32_t power = 0;
	uint32_t strHex = 0;
	int num_byte = 0;
	int digit = 0;
	sprintf(buf, "%d", iDate);
	num_byte = strlen(buf);
	for (int n = 0; n < num_byte; n ++) {
		byte = buf[num_byte - n - 1] - '0';
		power = pow(16, n);
		digit = byte *  power;
		strHex = strHex + digit;		
	}
	return strHex;
}
//====================================
	
void Systick_Proc(void) {	
	static int sysCount;
	//========	
	if (sysCount %1000 == 0) {
		//watter.flag.send_time = 1;	
	}
	//========	
	if ((sysCount % UART_CMD_PROCESSING_TIME_MS) == 0) {
		watter.flag.uart_check_rx = 1;	
	}
	//========	
	if ((sysCount % 1000) == 0) {
		watter.flag.esp_ctrl = 1;	
	}	
	//========
	sysCount++;
}
//====================================

void Processing(void) {
	//========	
	if (watter.flag.send_time) {
		watter.flag.send_time = 0;	
		Send_Date_Time();			
	}
	//========	
	if (watter.flag.uart_check_rx) {
		watter.flag.uart_check_rx = 0;
		CmdRxProcessing(&rx_cmd, &watter);
	}
	//========	
	if (watter.flag.esp_ctrl) {
		watter.flag.esp_ctrl = 0;
		Esp_Controller();
	}
	//========		
	ButtonsProcessing();
}
//====================================

void Send_Date_Time(void) {
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	
	HAL_RTC_GetTime(&hrtc, &sTime,              RTC_FORMAT_BIN);// RTC_FORMAT_BCD
	HAL_RTC_GetDate(&hrtc, &sDate,              RTC_FORMAT_BIN);                  
	HAL_RTC_GetAlarm(&hrtc,&sAlarm,RTC_ALARM_A, RTC_FORMAT_BIN);    
	
//  Not Correct Year
//	sprintf(buf,"Date %02d-%02d-20%d Time %02d:%02d:%02d  Alarm Time %02d:%02d:%02d    Hot:%d    Cold:%d%c", 
//						sDate.Date, sDate.Month, sDate.Year,
//						sTime.Hours, sTime.Minutes, sTime.Seconds, 
//						sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds,
//						watter.input.hot_count, watter.input.cold_count,
//						13);
	
	sprintf(buf,"Date %02d-%02d Time %02d:%02d:%02d  Alarm Time %02d:%02d:%02d    Hot:%d    Cold:%d%c", 
						sDate.Date, sDate.Month, //sDate.Year,
						sTime.Hours, sTime.Minutes, sTime.Seconds, 
						sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds,
						watter.input.hot_count, watter.input.cold_count,
						13);	
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
}
//====================================

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) { 
	/*
	HAL_RTC_GetAlarm(hrtc, &sAlarm, RTC_ALARM_A,FORMAT_BIN);	

	if(sAlarm.AlarmTime.Seconds > 58) {
		sAlarm.AlarmTime.Seconds=0;
	} 
	else {
		sAlarm.AlarmTime.Seconds=sAlarm.AlarmTime.Seconds + 1;
	}
	while(HAL_RTC_SetAlarm_IT(hrtc, &sAlarm, FORMAT_BIN)!=HAL_OK){}
	*/
	
	Esp_Power_On_For_N_ms();
}
//====================================

void SetDate(uint32_t iDate) {
	
	RTC_DateTypeDef sDate;
	RTC_TimeTypeDef sTime;

	uint8_t year = 0;
	uint8_t mounth = 0;
	uint8_t date = 0;	

	year = (uint8_t)(iDate / 10000); 
	mounth = (uint8_t)((iDate - year * 10000) / 100);
	date = (uint8_t)((iDate - year * 10000) - mounth * 100);

	/* //=== For RTC_FORMAT_BCD ===
	uint32_t strHex = IntToHex(iDate);
	uint32_t strHex = iDate; 
	uint32_t Year   = (strHex & 0xff0000) >> (8*2); 
	uint32_t Mounth = (strHex & 0xff00)   >> (8*1); 
	uint32_t Date   = (strHex & 0x00ff)   >> (8*0); 
	
	year =   (uint8_t)((strHex & 0xff0000) >> (8*2)); 
	mounth = (uint8_t)((strHex & 0xff00)   >> (8*1)); 
	date =   (uint8_t)((strHex & 0x00ff)   >> (8*0)); 
	*/
	
	sDate.Year = year; 
	sDate.Month = mounth;
	sDate.Date = date;
	//==========
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN)) {      //RTC_FORMAT_BCD);
	  Error_Handler();
	}	
	//==========	
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
	  Error_Handler();
	}
}
//====================================

void SetTime(uint32_t iTime) {
	
	RTC_DateTypeDef sDate;
	RTC_TimeTypeDef sTime;

	sTime.Hours   = (uint8_t)(iTime / 10000); 
	sTime.Minutes = (uint8_t)((iTime - sTime.Hours * 10000) / 100);
	sTime.Seconds = (uint8_t)((iTime - sTime.Hours * 10000) - sTime.Minutes * 100);
	//==========
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); 
	//==========
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN)) {         //RTC_FORMAT_BCD);
	  Error_Handler();
	}	
	//==========	
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
	  Error_Handler();
	}
	//==========	
}
//====================================

uint32_t Set_Cold_Ticks(void) {
	
	return watter.input.cold_count;
}
//====================================

void Send_Hot_Ticks(void) {
	
	sprintf(buf,"Hot_Ticks: %000000d%c", watter.input.hot_count, 13);	
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
}
//====================================

void Send_Cold_Ticks(void) {
	
	sprintf(buf,"Cold_Ticks: %000000d%c", watter.input.cold_count, 13);	
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
}
//====================================

void Esp_Power_On_For_N_ms(void) {
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	//=======
	ESP_ON;
	watter.esp.timeOn = HAL_GetTick();	
	watter.esp.isOn = 1;	
	//=======
	HAL_RTC_GetTime(&hrtc, &sTime,              RTC_FORMAT_BIN);// RTC_FORMAT_BCD
	HAL_RTC_GetDate(&hrtc, &sDate,              RTC_FORMAT_BIN);                  
	HAL_RTC_GetAlarm(&hrtc,&sAlarm,RTC_ALARM_A, RTC_FORMAT_BIN);  	
	
	sprintf(buf,"ESP_ON  Date %02d-%02d Time %02d:%02d:%02d  Alarm Time %02d:%02d:%02d    Hot:%d    Cold:%d%c", 
						sDate.Date, sDate.Month, //sDate.Year,
						sTime.Hours, sTime.Minutes, sTime.Seconds, 
						sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds,
						watter.input.hot_count, watter.input.cold_count,
						13);
						
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));		
}
//====================================

void Esp_Controller(void) {
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	
	if ((watter.esp.isOn) && (HAL_GetTick() > watter.esp.timeOn + watter.esp.workMs)) {
		watter.esp.isOn = 0;		
		ESP_OFF;	
		
		//=======
		HAL_RTC_GetTime(&hrtc, &sTime,              RTC_FORMAT_BIN);// RTC_FORMAT_BCD
		HAL_RTC_GetDate(&hrtc, &sDate,              RTC_FORMAT_BIN);                  
		HAL_RTC_GetAlarm(&hrtc,&sAlarm,RTC_ALARM_A, RTC_FORMAT_BIN); 
			
		sprintf(buf,"ESP_OFF  Date %02d-%02d Time %02d:%02d:%02d  Alarm Time %02d:%02d:%02d    Hot:%d    Cold:%d%c", 
						sDate.Date, sDate.Month, //sDate.Year,
						sTime.Hours, sTime.Minutes, sTime.Seconds, 
						sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds,
						watter.input.hot_count, watter.input.cold_count,
						13);
						
		HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));		
		//=======
	}	
}
//====================================

void Set_Alarm_Time(uint32_t iTime) {
	
	sAlarm.AlarmTime.Hours   = (uint8_t)(iTime / 10000); 
	sAlarm.AlarmTime.Minutes = (uint8_t)((iTime - sAlarm.AlarmTime.Hours * 10000) / 100);
	sAlarm.AlarmTime.Seconds = (uint8_t)((iTime - sAlarm.AlarmTime.Hours * 10000) - sAlarm.AlarmTime.Minutes * 100);
	//==========
	while(HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, FORMAT_BIN)!=HAL_OK){}
	//==========	
}
//====================================

void Send_Alarm_Time(void) {
	
	HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);  
	
	sprintf(buf,"Alarm Time %02d:%02d:%02d%c", 
		sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds,	13);	
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
}
//====================================
void Set_ESP_On_Time_ms(uint32_t iTime) {	
	watter.esp.workMs = iTime;
}
//====================================

void Get_ESP_On_Time_ms(void) {	
	sprintf(buf,"Esp On Time, ms = %d%c", watter.esp.workMs, 13);	
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
}
//====================================

void ESP_Force_On(void) {	
	ESP_ON;
	sprintf(buf,"Esp Force On %c", 13);	
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
}
//====================================

void ESP_Force_Off(void) {	
	ESP_OFF;
	sprintf(buf,"Esp Force Off %c", 13);	
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, strlen(buf));	
}
//====================================

