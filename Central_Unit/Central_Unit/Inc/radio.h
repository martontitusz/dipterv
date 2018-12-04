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
#define RADIO_PACKET_LENGTH				22		/* 22 bytes */
#define RADIO_TEMPERATUREDATA_ID		1
#define RADIO_HUMIDITYDATA_ID			2
#define RADIO_RX_TIMEOUT				2000	/* 2000 ms = 2 sec */

#define RADIO_NOT_INRXMODE				0
#define RADIO_INRXMODE					1

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

typedef enum StateEnum
{
	Rx,
	PacketReceived,
	ChangeChannel
} radioState_t;

/* Function prototypes */
radioState_t RadioGetState(void);
void RadioSetState(radioState_t new_state);
uint8_t RadioGetRxModeState(void);
void RadioSetRxModeState(uint8_t new);
void RadioInitLoraModule(void);
void RadioStateMachineFunction(void);
void RadioRxStateFunction(void);
uint8_t RadioReceivePacket(void);
uint8_t RadioComparePackets(void);
void RadioPacketReceivedStateFunction(void);
void RadioSelectChannel(void);
void RadioChangeChannelStateFunction(void);


void RadioTaskFunction(void const * argument);

#endif /* RADIO_H_ */
