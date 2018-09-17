/*
 * led.c
 *
 *  Created on: 2018. szept. 11.
 *      Author: Kiss M�rton Titusz
 */

#include "led.h"

extern uint8_t NumberOfAvailablePackets;

void LedTaskFunction(void const * argument)
{
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

	for(;;)
	{
		if (NumberOfAvailablePackets)
		{
			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
		}

		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

		osDelay(500);
	}
}
