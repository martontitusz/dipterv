/*
 * led.c
 *
 *  Created on: 2018. szept. 11.
 *      Author: Kiss Márton Titusz
 */

#include "led.h"

extern uint8_t		NumberOfAvailablePackets;
extern radioState_t	RadioState;

void LedTaskFunction(void const * argument)
{
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

	for(;;)
	{
//		if ((NumberOfAvailablePackets % 2) == 1)
//		{
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
//		}
//		else
//		{
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
//		}
//
//		if ((NumberOfAvailablePackets % 4) > 1)
//		{
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
//		}
//		else
//		{
//			HAL_GPIO_WritePin(LED0_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
//		}

//		switch (RadioState)
//		{
//			case Tx:
//			{
//				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
//				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
//				break;
//			}
//			case Rx:
//			{
//				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
//				break;
//			}
//			case PacketReceived:
//			{
//				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
//				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
//				break;
//			}
//			case ChangeChannel:
//			{
//				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
//				break;
//			}
//			default:
//			{
//				HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//				HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//				osDelay(100);
//				break;
//			}
//		}




		osDelay(1);
	}
}
