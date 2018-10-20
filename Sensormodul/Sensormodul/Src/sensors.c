/*
 * sensors.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "sensors.h"

extern uint8_t TemperatureBuffer[2];
extern uint8_t HumidityBuffer[2];
extern bool dataAvailable;

extern osSemaphoreId TemperatureSemaphoreHandle;
extern osSemaphoreId HumiditySemaphoreHandle;

void SensorsTaskFunction(void const * argument)
{
	osDelay(3000);

	for(;;)
	{
		HDC2010_TriggerMeasurement();
		osDelay(100);

		if (xSemaphoreTake(TemperatureSemaphoreHandle, 100))
		{
			HDC2010_GetTemperature(TemperatureBuffer);
			xSemaphoreGive(TemperatureSemaphoreHandle);
		}

		if (xSemaphoreTake(HumiditySemaphoreHandle, 100))
		{
			HDC2010_GetHumidity(HumidityBuffer);
			xSemaphoreGive(HumiditySemaphoreHandle);
		}

		if (dataAvailable == false)
		{
			dataAvailable = true;
		}
		osDelay(1000);
	}
}
