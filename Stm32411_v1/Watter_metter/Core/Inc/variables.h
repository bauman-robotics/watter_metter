#ifndef __VARIABLES_H__
	#define __VARIABLES_H__

	#include "main.h"
	#include "defines.h"
	
//====================================	
typedef struct{
	bool send_time;
	bool send_alarm;
	bool uart_check_rx;
	bool esp_ctrl;
} flag_type;
//====================================
	
typedef struct{
	uint32_t cold_count;
	uint32_t hot_count;
	int hot_state;
	int cold_state;
	
	int old_hot_state;
	int old_cold_state;
	
} input_type;
//====================================
	
typedef struct {
	uint32_t currentCounter;
	uint32_t lastCounter;		
	bool startFindLastByte;		
} lastByteDefStype;
//====================================
	
typedef struct {
	uint32_t timeOn;
	uint32_t workMs;
	bool 	 isOn;
} espPowerType;
//====================================

typedef struct {
	uint8_t cmd_code;
	uint16_t cmd_params;
	uint8_t f_next_byte_received;	
	uint8_t Buf_size;
	uint8_t rx_buff[RX_CMD_BUFF_SIZE];
	uint8_t rx_ready_buffer[RX_CMD_BUFF_SIZE];
	uint8_t rx_byte_counter; 	
	uint8_t f_cmd_received;
	uint8_t num_bytes_received;
	char txBuff[TX_ANS_BUFF_SIZE];
	int16_t txBuffLen;
} cmd_s_type;	
//====================================

typedef struct{
	flag_type flag;
	input_type input;
	espPowerType esp;
} watter_type;
//====================================
#endif 	