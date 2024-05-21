#ifndef __BUTTON_H
#define __BUTTON_H
#include "main.h"

int BtnDebounce1(GPIO_TypeDef *GPIO_PORT, uint16_t GPIO_PIN);  //pressedTrue => return 0;  // notPressedTrue => return 1; // notDefinedState => return -1
int BtnDebounce2(GPIO_TypeDef *GPIO_PORT, uint16_t GPIO_PIN);  //pressedTrue => return 0;  // notPressedTrue => return 1; // notDefinedState => return -1
void ButtonsProcessing(void);
#endif  
