/*
 * HDC2010.c
 *
 *  Created on: 2018. aug. 27.
 *      Author: Kiss M�rton Titusz
 */

#include "HDC2010.h"

/* Functions */

int16_t HDC2010_ConcatenateTo16Bits(int8_t highByte, int8_t lowByte)
{
	return (int16_t)(highByte << 8) | lowByte;
}

float HDC2010_CalculateTemperature(int16_t temp16)
{
	return (temp16 * 0.0025177f) - 40.0f;
}

float HDC2010_CalculateHumidity(int16_t hum16)
{
	return (hum16 * 0.001525878f);
}

int HDC2010_TriggerMeasurement(void)
{
	uint8_t localbuffer[2];
	localbuffer[0] = HDC2010_REG_MEASUREMENT_CONF;
	localbuffer[1] = 0x01;

	if (HAL_I2C_Master_Transmit(&hi2c1, HDC2010_SLAVE_ADDRESS << 1, localbuffer, 2, 100) != HAL_OK)
	{
		while(1) {}
	}
	else
	{
		return 0;
	}
}

float HDC2010_GetTemperature(void)
{
	uint8_t localbuffer[2];

	localbuffer[0] = HDC2010_REG_TEMPERATURE_LOW;

	if (HAL_I2C_Master_Transmit(&hi2c1, HDC2010_SLAVE_ADDRESS << 1, localbuffer, 1, 100) != HAL_OK)
	{
		while(1) {}
	}

	localbuffer[0] = 0x00;
	localbuffer[1] = 0x00;

	if(HAL_I2C_Master_Receive(&hi2c1, HDC2010_SLAVE_ADDRESS << 1, localbuffer, 2, 100) != HAL_OK)
	{
		while(1) {}
	}

	float tmp = HDC2010_ConcatenateTo16Bits(localbuffer[1], localbuffer[0]);

	return HDC2010_CalculateTemperature(tmp);
}

float HDC2010_GetHumidity(void)
{
	uint8_t localbuffer[2];

	localbuffer[0] = HDC2010_REG_HUMIDITY_LOW;

	if (HAL_I2C_Master_Transmit(&hi2c1, HDC2010_SLAVE_ADDRESS << 1, localbuffer, 1, 100) != HAL_OK)
	{
		while(1) {}
	}

	localbuffer[0] = 0x00;
	localbuffer[1] = 0x00;

	if(HAL_I2C_Master_Receive(&hi2c1, HDC2010_SLAVE_ADDRESS << 1, localbuffer, 2, 100) != HAL_OK)
	{
		while(1) {}
	}

	float tmp = HDC2010_ConcatenateTo16Bits(localbuffer[1], localbuffer[0]);

	return HDC2010_CalculateHumidity(tmp);
}
