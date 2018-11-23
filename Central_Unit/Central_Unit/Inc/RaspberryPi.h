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

#define I2CLAZY	0
#define I2CBUSY	1


uint8_t RaspberryPiGetNumberOfAvailablePackets(void);
void RaspberryPiSetNumberOfAvailablePackets(uint8_t new);

uint8_t RaspberryPiGetI2CState(void);
void RaspberryPiSetI2CState(uint8_t new);

void RaspberryPiFillI2CBuffer(void);

void RaspberryPiTaskFunction(void const * argument);

#endif /* RASPBERRYPI_H_ */
