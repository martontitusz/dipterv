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
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

/* Defines */
#define RADIO_PACKET_LENGTH			22	/* 22 bytes */
#define RADIO_BUFFER_LENGTH			46
#define RADIO_TEMPERATUREDATA_ID	1
#define RADIO_HUMIDITYDATA_ID		2
#define RADIO_ACK_MESSAGE_LENGTH	5	/* 5 byte */
#define RADIO_ACK_MESSAGE			0x55
#define RADIO_MAX_RETRANSMISSONS	10
#define RADIO_ACK_RETRANSMISSIONS	10

union
{
	uint32_t	words[3];
	uint8_t		bytes[12];
} DeviceId;

union
{
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

union radiopacket_union_t
{
	struct radiopacket_t packet;
	uint8_t bytes[22];
} RadioPacket;

/* Function prototypes */
void RadioInitLoraModule(void);
void RadioConfigLoraModule(uint8_t channel);
uint8_t RadioChangeChannel(uint8_t old_channel);
void RadioTransmitPacket(uint8_t* message_buffer, int message_length);
uint8_t RadioReceivePacket(void);
void RadioTransmitAck(void);

void RadioTaskFunction(void const * argument);

#endif /* RADIO_H_ */
