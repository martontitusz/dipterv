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
#define RADIO_PACKET_LENGTH				22	/* 22 bytes */
#define RADIO_BUFFER_LENGTH				46
#define RADIO_TEMPERATUREDATA_ID		1
#define RADIO_HUMIDITYDATA_ID			2
#define RADIO_ACK_MESSAGE_LENGTH		5	/* 5 byte */
#define RADIO_ACK_MESSAGE				0x55
#define RADIO_ACK_MAX_RETRANSMISSIONS	5
#define RADIO_RX_TIMEOUT				2000
#define RADIO_TX_TIMEOUT				2000

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
uint8_t RadioReceivePacket(void);

/* StateMachine Functions: */
void RadioStateMachineFunction(void);
void RadioTxStateFunction(void);
void RadioRxStateFunction(void);
void RadioPacketReceivedStateFunction(void);
void RadioSelectChannel(void);
void RadioChangeChannelStateFunction(void);
void RadioStandbyStateFunction(void);
void RadioSleepStateFunction(void);

/***************************/

void RadioTaskFunction(void const * argument);

#endif /* RADIO_H_ */
