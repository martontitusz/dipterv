/*
 * radio.h
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#ifndef RADIO_H_
#define RADIO_H_

#include "main.h"
#include "SX1278.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

/* Defines */
#define RADIO_PACKET_LENGTH			22	/* 22 bytes */
#define RADIO_TEMPERATUREDATA_ID	1	/* 1 byte */
#define RADIO_HUMIDITYDATA_ID		2	/* 2 byte */

/* Variables */
union {
	uint32_t	words[3];
	uint8_t		bytes[12];
} DeviceId;

union {
	uint32_t	words;
	uint8_t		bytes[4];
} PacketId;

struct radiopacket_t
{
	uint32_t deviceId[3];
	uint32_t packetId;
	uint8_t temperatuteId;
	uint8_t temperature[2];
	uint8_t humidityId;
	uint8_t humidity[2];
};

union {
	struct radiopacket_t packet;
	uint8_t bytes[22];
} RadioPacket;

/* Function prototypes */
void RadioInitLoraModule(void);
void RadioConfigLoraModule(void);
void RadioTransmitPacket(uint8_t* message_buffer, int message_length);
uint8_t RadioReceivePacket(uint8_t* message_buffer);
void RadioPacketBuilder(void);
void GetUniqueId(void);
void RadioFillDeviceId(void);
void RadioFillPacketCounter(void);
void RadioFillTemperatureId(void);
void RadioFillHumidityId(void);
void RadioFillIds(void);
void RadioFillTemperatureData(void);
void RadioFillHumidityData(void);
void RadioIncreasePacketId(void);

void RadioTaskFunction(void const * argument);

#endif /* RADIO_H_ */
