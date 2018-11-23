/*
 * radio.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "radio.h"

extern osMessageQId				TemperatureQueueHandle;
extern osMessageQId				HumidityQueueHandle;
extern SPI_HandleTypeDef 		hspi1;

SX1278_hw_t						SX1278_hw;
SX1278_t						SX1278;

bool RadioInSleepMode		=	false;
uint8_t							*pRxTemperature;
uint8_t							*pRxHumidity;
uint8_t RetransmitCounter	=	0;
radioState_t RadioState		=	Sleep;


radioState_t RadioGetState(void)
{
	return RadioState;
}

void RadioSetState(radioState_t new_state)
{
	RadioState = new_state;
}

void RadioInitLoraModule(void)
{
	SX1278_hw.dio0.port		= LORA_DIO0_GPIO_Port;
	SX1278_hw.dio0.pin		= LORA_DIO0_Pin;
	SX1278_hw.nss.port		= LORA_SPI_NSS_GPIO_Port;
	SX1278_hw.nss.pin		= LORA_SPI_NSS_Pin;
	SX1278_hw.reset.port	= NULL;
	SX1278_hw.reset.pin		= 0;
	SX1278_hw.spi			= &hspi1;

	SX1278.hw				= &SX1278_hw;
}

void RadioPacketBuilder(void)
{
	RadioIncreasePacketId();
	RadioFillTemperatureData();
	RadioFillHumidityData();
}

void GetUniqueId(void)
{
	DeviceId.words[0] = HAL_GetUIDw0();
	DeviceId.words[1] = HAL_GetUIDw1();
	DeviceId.words[2] = HAL_GetUIDw2();
}

void RadioFillDeviceId(void)
{
	RadioPacket.packet.deviceId[0] = DeviceId.words[0];
	RadioPacket.packet.deviceId[1] = DeviceId.words[1];
	RadioPacket.packet.deviceId[2] = DeviceId.words[2];
}

void RadioFillTemperatureId(void)
{
	RadioPacket.packet.temperatuteId = RADIO_TEMPERATUREDATA_ID;
}

void RadioFillHumidityId(void)
{
	RadioPacket.packet.humidityId = RADIO_HUMIDITYDATA_ID;
}

void RadioFillIds(void)
{
	RadioPacket.packet.packetId = 0;
	RadioFillDeviceId();
	RadioFillTemperatureId();
	RadioFillHumidityId();
}

void RadioFillTemperatureData(void)
{
	if ( uxQueueMessagesWaiting(TemperatureQueueHandle) )
	{
		if ( xQueueReceive(TemperatureQueueHandle, &(pRxTemperature), (TickType_t)10) != pdTRUE )
		{
			while (1);
		}

		RadioPacket.packet.temperature[0] = pRxTemperature[0];
		RadioPacket.packet.temperature[1] = pRxTemperature[1];
	}
}

void RadioFillHumidityData(void)
{
	if ( uxQueueMessagesWaiting(HumidityQueueHandle) )
	{
		if ( xQueueReceive(HumidityQueueHandle, &(pRxHumidity), (TickType_t)10) != pdTRUE )
		{
			while (1);
		}

		RadioPacket.packet.humidity[0] = pRxHumidity[0];
		RadioPacket.packet.humidity[1] = pRxHumidity[1];
	}
}

void RadioIncreasePacketId(void)
{
	RadioPacket.packet.packetId++;
}

void RadioStateMachineFunction(void)
{
	switch ( RadioGetState() )
	{
		case PacketBuilding:
		{
			RadioPacketBuildingStateFunction();
			break;
		}
		case Tx:
		{
			RadioTxStateFunction();
			break;
		}
		case Sleep:
		{
			RadioSleepStateFunction();
			break;
		}
		default:
		{
			RadioSleepStateFunction();
			break;
		}
	}
}

void RadioPacketBuildingStateFunction(void)
{
	RadioInSleepMode = false;
	RadioPacketBuilder();
	RadioSetState(Tx);
}

void RadioTxStateFunction(void)
{
	if (RetransmitCounter == RADIO_MAX_RETRANSMISSONS)
	{
		RetransmitCounter = 0;
		RadioSetState(Sleep);
	}
	else
	{
		if(!RetransmitCounter)
		{
			SX1278_LoRaEntryTx(&SX1278, RADIO_PACKET_LENGTH, RADIO_TX_TIMEOUT);
		}
		RetransmitCounter++;
		SX1278_LoRaTxPacket(&SX1278, RadioPacket.bytes, RADIO_PACKET_LENGTH, RADIO_TX_TIMEOUT);
	}
}

void RadioSleepStateFunction(void)
{
	if (!RadioInSleepMode)
	{
		RadioInSleepMode = true;
		SX1278_sleep(&SX1278);
	}
	else {}
}

/* RadioTaskFunction function */
void RadioTaskFunction(void const * argument)
{
	GetUniqueId();
	RadioFillIds();

	RadioInitLoraModule();
	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8, RADIO_BANDWIDTH, 10);


	for(;;)
	{
		RadioStateMachineFunction();

		osDelay(1);
	}
}
