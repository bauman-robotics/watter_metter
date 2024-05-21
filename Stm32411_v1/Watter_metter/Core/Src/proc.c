#include <math.h>
#include "main.h"
#include "uartProcessing.h"

extern watter_type watter;
extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;

static char buf[MAX_LEN_COM_PORT_SEND] = {};
	
extern cmd_s_type rx_cmd;

void Processing(void);
void Send_Time(void); 
void Systick_Proc(void);
void ButtonsProcessing(void);
void SetDate(uint32_t iDate);
uint32_t IntToHex(uint32_t iDate);
	
RTC_AlarmTypeDef sAlarm;  // have to be Global  !!!!!
//====================================
	
void Systick_Proc(void) {	
	static int sysCount;
	if (sysCount %1000 == 0) {
		watter.flag.send_time = 1;	
	}
	if ((sysCount % UART_CMD_PROCESSING_TIME_MS) == 0) {
		watter.flag.uart_check_rx = 1;	
	}	
	
	sysCount++;
}
//====================================

void Processing(void) {
	if (watter.flag.send_time) {
		watter.flag.send_time = 0;	
		Send_Time();			
	}
	if (watter.flag.uart_check_rx) {
		watter.flag.uart_check_rx = 0;
		CmdRxProcessing(&rx_cmd, &watter);
	}
	ButtonsProcessing();
}
//====================================

void Send_Time(void) {
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	
	HAL_RTC_GetTime(&hrtc, &sTime,              RTC_FORMAT_BIN);// RTC_FORMAT_BCD
	HAL_RTC_GetDate(&hrtc, &sDate,              RTC_FORMAT_BIN);                  
	HAL_RTC_GetAlarm(&hrtc,&sAlarm,RTC_ALARM_A, RTC_FORMAT_BIN);                  
	
	sprintf(buf,"Date %02d-%02d-20%d Time %02d:%02d:%02d  Alarm Time %02d:%02d:%02d    Hot:%d    Cold:%d%c", 
						sDate.Date, sDate.Month, sDate.Year,
						sTime.Hours, sTime.Minutes, sTime.Seconds, 
						sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds,
						watter.input.hot_count, watter.input.cold_count,
						13);
	
	HAL_UART_Transmit(&huart2, (uint8_t *)&buf, strlen(buf), 1000);	
}
//====================================


void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) { 
  HAL_RTC_GetAlarm(hrtc, &sAlarm, RTC_ALARM_A,FORMAT_BIN);	

  if(sAlarm.AlarmTime.Seconds > 58) {
    sAlarm.AlarmTime.Seconds=0;
  } 
	else {
    sAlarm.AlarmTime.Seconds=sAlarm.AlarmTime.Seconds + 1;
  }

  while(HAL_RTC_SetAlarm_IT(hrtc, &sAlarm, FORMAT_BIN)!=HAL_OK){}
}
//====================================

void SetDate(uint32_t iDate) {
	
	RTC_DateTypeDef sDate;
	RTC_TimeTypeDef sTime;
	
	uint32_t strHex = IntToHex(iDate);
	//uint32_t strHex = iDate; 

	uint8_t year = 0;
	uint8_t mounth = 0;
	uint8_t date = 0;	
	//						sDate.Date, sDate.Month, sDate.Year,
	year = (uint8_t)(iDate / 10000); 
	mounth = (uint8_t)((iDate - year * 10000) / 100);
	date = (uint8_t)((iDate - year * 10000) - mounth * 100);
	
//	uint32_t Year   = (strHex & 0xff0000) >> (8*2); 
//	uint32_t Mounth = (strHex & 0xff00)   >> (8*1); 
//	uint32_t Date   = (strHex & 0x00ff)   >> (8*0); 
	
//	year =   (uint8_t)((strHex & 0xff0000) >> (8*2)); 
//	mounth = (uint8_t)((strHex & 0xff00)   >> (8*1)); 
//	date =   (uint8_t)((strHex & 0x00ff)   >> (8*0)); 

	
	sDate.Year = year; 
	sDate.Month = mounth;
	sDate.Date = date;

//AL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);
	
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN)) //RTC_FORMAT_BCD);//RTC_FORMAT_BIN);
	{
	  Error_Handler();
	}	
	
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
	  Error_Handler();
	}
}
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