#ifndef __VARIABLES_H__
	#define __VARIABLES_H__

	#include "main.h"
	#include "defines.h"
	
//====================================	
	typedef struct{
		bool send_time;
		bool send_alarm;
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
	typedef struct{
		flag_type flag;
		input_type input;
	} watter_type;
//====================================
#endif 	