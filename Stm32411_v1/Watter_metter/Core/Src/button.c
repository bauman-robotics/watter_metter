//button.c

#include "button.h"
#include "defines.h"

extern watter_type watter;
//====================================

int BtnDebounce1(GPIO_TypeDef *GPIO_PORT, uint16_t GPIO_PIN) {
	int debounceTime = BUTTONS_DEBOUNCE_TIME_MS;
	static int triggerPressed;
	static int triggerNotPressed;
	static int remTickPressed;
	static int remTickNotPressed;
	static int pressed;
	static int notPressed;
	static int pressedTrue;
	static int notPressedTrue;
	static int trigger;

	if (!HAL_GPIO_ReadPin(GPIO_PORT, GPIO_PIN) && (!triggerPressed)) {  // Bot  pressed
		triggerPressed = 1;
		triggerNotPressed = 0;
		remTickPressed = HAL_GetTick();
		pressed = 1;
		notPressed = 0;
	}

	if (HAL_GPIO_ReadPin(GPIO_PORT, GPIO_PIN)  &&  (!triggerNotPressed)) {  // Bot not pressed
		triggerNotPressed = 1;
		triggerPressed = 0;
		remTickNotPressed = HAL_GetTick();
		notPressed = 1;
		pressed = 0;
	}

	if ((pressed) && (HAL_GetTick() - remTickPressed > debounceTime)) {  // pressedTrue
		triggerPressed = 0;
		pressedTrue = 1;
		notPressedTrue = 0;
	}

	if ((notPressed) && (HAL_GetTick() - remTickNotPressed > debounceTime)) {   // notPressedTrue
		pressedTrue = 0;
		notPressedTrue = 1;
		trigger = 0;
	}

	if (pressedTrue) return 0;
	else if (notPressedTrue) return 1;
	else return -1;
}
//====================================

int BtnDebounce2(GPIO_TypeDef *GPIO_PORT, uint16_t GPIO_PIN) {
	int debounceTime = BUTTONS_DEBOUNCE_TIME_MS;
	static int triggerPressed;
	static int triggerNotPressed;
	static int remTickPressed;
	static int remTickNotPressed;
	static int pressed;
	static int notPressed;
	static int pressedTrue;
	static int notPressedTrue;
	static int trigger;

	if (!HAL_GPIO_ReadPin(GPIO_PORT, GPIO_PIN) && (!triggerPressed)) {  // Bot  pressed
		triggerPressed = 1;
		triggerNotPressed = 0;
		remTickPressed = HAL_GetTick();
		pressed = 1;
		notPressed = 0;
	}

	if (HAL_GPIO_ReadPin(GPIO_PORT, GPIO_PIN)  &&  (!triggerNotPressed)) {  // Bot not pressed
		triggerNotPressed = 1;
		triggerPressed = 0;
		remTickNotPressed = HAL_GetTick();
		notPressed = 1;
		pressed = 0;
	}

	if ((pressed) && (HAL_GetTick() - remTickPressed > debounceTime)) {  // pressedTrue
		triggerPressed = 0;
		pressedTrue = 1;
		notPressedTrue = 0;
	}

	if ((notPressed) && (HAL_GetTick() - remTickNotPressed > debounceTime)) {   // notPressedTrue
		pressedTrue = 0;
		notPressedTrue = 1;
		trigger = 0;
	}

	if (pressedTrue) return 0;
	else if (notPressedTrue) return 1;
	else return -1;
}
//====================================

void ButtonsProcessing(void) {
	watter.input.hot_state  = BtnDebounce1(HOT_IN_GPIO_Port, HOT_IN_Pin);     //pressedTrue => return 0;  // notPressedTrue => return 1; // notDefinedState => return -1
	watter.input.cold_state = BtnDebounce2(COLD_IN_GPIO_Port, COLD_IN_Pin);  //pressedTrue => return 0;  // notPressedTrue => return 1; // notDefinedState => return -1
	//========================================
	if ((watter.input.hot_state == 0) && (watter.input.old_hot_state == 1)){   
		 watter.input.hot_count ++;
	}
//	if ((watter.input.hot_state == 1) && (watter.input.old_hot_state == 0)){   
//		 watter.input.hot_count ++;
//	}
	
	if (watter.input.hot_state == 0) {
		watter.input.old_hot_state = 0;
	}
	if (watter.input.hot_state == 1) {
		watter.input.old_hot_state = 1;
	}
	//========================================
	if ((watter.input.cold_state == 0) && (watter.input.old_cold_state == 1)){   
		 watter.input.cold_count ++;
	}
//	if ((watter.input.cold_state == 1) && (watter.input.old_cold_state == 0)){   
//		 watter.input.cold_count ++;
//	}
	
	if (watter.input.cold_state == 0) {
		watter.input.old_cold_state = 0;
	}
	if (watter.input.cold_state == 1) {
		watter.input.old_cold_state = 1;
	}
	//========================================
}
//====================================
