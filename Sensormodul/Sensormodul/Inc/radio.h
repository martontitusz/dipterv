/*
 * radio.h
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss M�rton Titusz
 */

#ifndef RADIO_H_
#define RADIO_H_

#include "main.h"
#include "SX1278.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

/* Defines */
#define RADIO_PACKET_LENGTH			22		/* 22 bytes */
#define RADIO_TEMPERATUREDATA_ID	1		/* 1 byte */
#define RADIO_HUMIDITYDATA_ID		2		/* 2 byte */
#define RADIO_MAX_RETRANSMISSONS	5
#define RADIO_TX_TIMEOUT			2000	/* 2000 ms = 2 sec */

#define RADIO_BANDWIDTH				SX1278_LORA_BW_15_6KHZ	//Sensormodul1
//#define RADIO_BANDWIDTH				SX1278_LORA_BW_20_8KHZ		//Sensormodul2

/* Variables */
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
	uint32_t	deviceId[3];
	uint32_t	packetId;
	uint8_t		temperatuteId;
	uint8_t		temperature[2];
	uint8_t		humidityId;
	uint8_t		humidity[2];
};

union
{
	struct radiopacket_t packet;
	uint8_t bytes[22];
} RadioPacket;

/* Function prototypes */
radioState_t RadioGetState(void);
void RadioSetState(radioState_t new_state);
void RadioInitLoraModule(void);
void RadioPacketBuilder(void);
void GetUniqueId(void);
void RadioFillDeviceId(void);
void RadioFillTemperatureId(void);
void RadioFillHumidityId(void);
void RadioFillIds(void);
void RadioFillTemperatureData(void);
void RadioFillHumidityData(void);
void RadioIncreasePacketId(void);
void RadioStateMachineFunction(void);
void RadioPacketBuildingStateFunction(void);
void RadioTxStateFunction(void);
void RadioSleepStateFunction(void);


void RadioTaskFunction(void const * argument);

#endif /* RADIO_H_ */
