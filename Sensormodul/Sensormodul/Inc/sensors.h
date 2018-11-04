/*
 * sensors.h
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include "main.h"
#include "HDC2010.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stdbool.h"


void SensorsTriggerMeasurement(void);
void SensorsGetTemperature(void);
void SensorsGetHumidity(void);
void SensorsGetData(void);
void SensorsTaskFunction(void const * argument);

#endif /* SENSORS_H_ */
