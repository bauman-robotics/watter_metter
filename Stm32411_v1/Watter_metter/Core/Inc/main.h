/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "variables.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ESP_POWER_Pin GPIO_PIN_13
#define ESP_POWER_GPIO_Port GPIOC
#define HOT_IN_Pin GPIO_PIN_0
#define HOT_IN_GPIO_Port GPIOA
#define COLD_IN_Pin GPIO_PIN_1
#define COLD_IN_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

//=== ESP_POWER ===
#define ESP_OFF     (HAL_GPIO_WritePin(ESP_POWER_GPIO_Port, ESP_POWER_Pin, 1))
#define ESP_ON      (HAL_GPIO_WritePin(ESP_POWER_GPIO_Port, ESP_POWER_Pin, 0))
#define ESP_TOGGLE  (HAL_GPIO_TogglePin(ESP_POWER_GPIO_Port, ESP_POWER_Pin))
//=================
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
