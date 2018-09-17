/*
 * led.h
 *
 *  Created on: 2018. szept. 11.
 *      Author: Kiss Márton Titusz
 */

#ifndef LED_H_
#define LED_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

void LedTaskFunction(void const * argument);

#endif /* LED_H_ */
