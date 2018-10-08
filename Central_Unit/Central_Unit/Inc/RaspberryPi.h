/*
 * RaspberryPi.h
 *
 *  Created on: 2018. okt. 7.
 *      Author: Kiss Márton Titusz
 */

#ifndef RASPBERRYPI_H_
#define RASPBERRYPI_H_

#include "main.h"
#include "SX1278.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "radio.h"


void RaspberryPiFillI2CBuffer(void);
void RaspberryPiTaskFunction(void const * argument);


#endif /* RASPBERRYPI_H_ */
