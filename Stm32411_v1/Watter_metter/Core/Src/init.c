#include "main.h"

#include "uartProcessing.h"

extern watter_type watter;
extern cmd_s_type rx_cmd;
extern UART_HandleTypeDef huart2;

void ESP_Force_On(void);
void Set_Alarm_Time(uint32_t iTime);

void InitAll(void) {	
	watter.input.old_cold_state = 1;
	watter.input.old_hot_state = 1;
	
	ESP_OFF;
	HAL_Delay(200);
	
	HAL_UART_Receive_IT(&huart2, (uint8_t*)rx_cmd.rx_buff, RX_CMD_BUFF_SIZE);
	
	//StartUartLog(&rx_cmd);
	
	watter.esp.workMs = ESP_ON_TIME_MS;
	
	if (DEBUG_INCREMENT_MODE) {
		watter.input.cold_count = 5;
		watter.input.hot_count = 8;
	}
	
	watter.esp.repeat_interval_sec = REPIAT_INTERVAL; // 100=> 1 min; 00 sec 
	//Set_Alarm_Time(watter.esp.repeat_interval_sec);
	
}

