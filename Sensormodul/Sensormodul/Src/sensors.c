/*
 * sensors.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "sensors.h"

extern osMessageQId TemperatureQueueHandle;
extern osMessageQId HumidityQueueHandle;

uint8_t TemperatureBuffer[2];
uint8_t HumidityBuffer[2];

uint8_t *pTemperature;
uint8_t *pHumidity;


void SensorsTriggerMeasurement(void)
{
	HDC2010_TriggerMeasurement();
	osDelay(100);
}

void SensorsGetTemperature(void)
{
	HDC2010_GetTemperature(TemperatureBuffer);
	if (xQueueSendToBack(TemperatureQueueHandle, (void *) &pTemperature, (TickType_t)10) != pdPASS)
	{
		/* Failed to post the message, even after 10 ticks. */
		while(1);
	}
}

void SensorsGetHumidity(void)
{
	HDC2010_GetHumidity(HumidityBuffer);
	if (xQueueSendToBack(HumidityQueueHandle, (void *) &pHumidity, (TickType_t)10) != pdPASS)
	{
		/* Failed to post the message, even after 10 ticks. */
		while(1);
	}
}

void SensorsGetData(void)
{
	SensorsTriggerMeasurement();
	SensorsGetTemperature();
	SensorsGetHumidity();
}

void SensorsTaskFunction(void const * argument)
{
	osDelay(3000);

	pTemperature	= TemperatureBuffer;
	pHumidity		= HumidityBuffer;

	for(;;)
	{
		SensorsGetData();
		osDelay(1000);
	}
}
