/*
 * sensors.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "sensors.h"

//extern float Temperature;
//extern float Humidity;


void SensorsTaskFunction(void const * argument)
{
	for(;;)
	{
//		HDC2010_TriggerMeasurement();
//		HAL_Delay(100);
//		Temperature	= HDC2010_GetTemperature();
//		Humidity	= HDC2010_GetHumidity();

		osDelay(10);
	}
}
