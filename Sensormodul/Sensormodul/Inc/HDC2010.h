/*
 * HDC2010.h
 *
 *  Created on: 2018. aug. 27.
 *      Author: Kiss Márton Titusz
 */

#ifndef HDC2010_H_
#define HDC2010_H_

#include "stdint.h"
#include "i2c.h"

/* Constants */
#define HDC2010_SLAVE_ADDRESS				(uint8_t)1000000	// Resistor R6 soldered onto the PCB

/* HDC2010 Register Map */
#define HDC2010_REG_TEMPERATURE_LOW			0x00
#define HDC2010_REG_TEMPERATURE_HIGH		0x01
#define HDC2010_REG_HUMIDITY_LOW			0x02
#define HDC2010_REG_HUMIDITY_HIGH			0x03
#define HDC2010_REG_INTERRUPT_DRDY			0x04
#define HDC2010_REG_TEMPERATURE_MAX			0x05
#define HDC2010_REG_HUMIDITY_MAX			0x06
#define HDC2010_REG_INTERRUPT_ENABLE		0x07
#define HDC2010_REG_TEMP_OFFSET_ADJUST		0x08
#define HDC2010_REG_HUM_OFFSET_ADJUST		0x09
#define HDC2010_REG_TEMP_THR_L				0x0A
#define HDC2010_REG_TEMP_THR_H				0x0B
#define HDC2010_REG_RH_THR_L				0x0C
#define HDC2010_REG_RH_THR_H				0x0D
#define HDC2010_REG_RESET_DRDY_INT_CONF		0x0E
#define HDC2010_REG_MEASUREMENT_CONF		0x0F
#define HDC2010_REG_MANUFACTURER_ID_LOW		0xFC
#define HDC2010_REG_MANUFACTURER_ID_HIGH	0xFD
#define HDC2010_REG_DEVICE_ID_LOW			0xFE
#define HDC2010_REG_DEVICE_ID_HIGH			0xFF

/* Function Prototypes */
int16_t HDC2010_ConcatenateTo16Bits(int8_t highByte, int8_t lowByte);
float HDC2010_CalculateTemperature(int16_t temp16);
float HDC2010_CalculateHumidity(int16_t hum16);
int HDC2010_TriggerMeasurement(void);
int HDC2010_GetTemperature(uint8_t* temperaturebuffer);
int HDC2010_GetHumidity(uint8_t* humiditybuffer);

#endif /* HDC2010_H_ */
