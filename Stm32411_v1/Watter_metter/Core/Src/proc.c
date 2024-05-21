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
//====================================
	
void Systick_Proc(void) {	
	static int sysCount;
	if (sysCount %1000 == 0) {
		//watter.flag.send_time = 1;	
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
	RTC_AlarmTypeDef sAlarm;
	
	HAL_RTC_GetTime(&hrtc, &sTime,              RTC_FORMAT_BIN);//RTC_FORMAT_BIN); RTC_FORMAT_BCD
	HAL_RTC_GetDate(&hrtc, &sDate,              RTC_FORMAT_BIN);
	HAL_RTC_GetAlarm(&hrtc,&sAlarm,RTC_ALARM_A, RTC_FORMAT_BIN);
	
//	sprintf(buf,"Date %d-%d-20%d Time %d:%d:%d  Alarm Time %d:%d:%d%c", 
//						sDate.Date, sDate.Month, sDate.Year,
//						sTime.Hours, sTime.Minutes, sTime.Seconds, 
//						sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds, 13);

	sprintf(buf,"Date %d-%d-20%d Time %d:%d:%d  Alarm Time %d:%d:%d    Hot:%d    Cold:%d%c", 
						sDate.Date, sDate.Month, sDate.Year,
						sTime.Hours, sTime.Minutes, sTime.Seconds, 
						sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds,
						watter.input.hot_count, watter.input.cold_count,
						13);
	
	HAL_UART_Transmit(&huart2, (uint8_t *)&buf, strlen(buf), 1000);	
}
//====================================

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) { 
  RTC_AlarmTypeDef sAlarm;
  HAL_RTC_GetAlarm(hrtc,&sAlarm,RTC_ALARM_A,FORMAT_BIN);
  if(sAlarm.AlarmTime.Seconds>58) {
    sAlarm.AlarmTime.Seconds=0;
  }else{
    sAlarm.AlarmTime.Seconds=sAlarm.AlarmTime.Seconds + 1;
  }
  while(HAL_RTC_SetAlarm_IT(hrtc, &sAlarm, FORMAT_BIN)!=HAL_OK){}
}