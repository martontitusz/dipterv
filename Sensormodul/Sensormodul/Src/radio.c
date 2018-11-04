/*
 * radio.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "radio.h"

SX1278_hw_t SX1278_hw;
SX1278_t 	SX1278;

uint8_t *pRxTemperature;
uint8_t *pRxHumidity;

extern osMessageQId TemperatureQueueHandle;
extern osMessageQId HumidityQueueHandle;
extern SPI_HandleTypeDef hspi1;

void RadioInitLoraModule(void)
{
	SX1278_hw.dio0.port = LORA_DIO0_GPIO_Port;
	SX1278_hw.dio0.pin = LORA_DIO0_Pin;
	SX1278_hw.nss.port = LORA_SPI_NSS_GPIO_Port;
	SX1278_hw.nss.pin = LORA_SPI_NSS_Pin;
	SX1278_hw.reset.port = NULL;
	SX1278_hw.reset.pin = 0;
	SX1278_hw.spi = &hspi1;

	SX1278.hw = &SX1278_hw;
}

void RadioConfigLoraModule(void)
{
	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8, SX1278_LORA_BW_20_8KHZ, 10);
	SX1278_LoRaEntryTx(&SX1278, 16, 2000);
}

void RadioTransmitPacket(uint8_t* message_buffer, int message_length)
{
	SX1278_LoRaEntryTx(&SX1278, message_length, 2000);
	SX1278_LoRaTxPacket(&SX1278, message_buffer, message_length, 2000);
}

uint8_t RadioReceivePacket(uint8_t* message_buffer)
{
	uint8_t received_bytes = SX1278_LoRaRxPacket(&SX1278);

	if (received_bytes > 0)
	{
		SX1278_read(&SX1278, message_buffer, received_bytes);
	}

	return received_bytes;
}

void RadioPacketBuilder(void)
{
	RadioIncreasePacketId();
	RadioFillTemperatureData();
	RadioFillHumidityData();
}

void GetUniqueId(void)
{
	DeviceId.words[0]	= HAL_GetUIDw0();
	DeviceId.words[1]	= HAL_GetUIDw1();
	DeviceId.words[2]	= HAL_GetUIDw2();
}

void RadioFillDeviceId(void)
{
	RadioPacket.packet.deviceId[0]	= DeviceId.words[0];
	RadioPacket.packet.deviceId[1]	= DeviceId.words[1];
	RadioPacket.packet.deviceId[2]	= DeviceId.words[2];
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
	RadioFillDeviceId();
	RadioFillTemperatureId();
	RadioFillHumidityId();
}

void RadioFillTemperatureData(void)
{
	if (uxQueueMessagesWaiting(TemperatureQueueHandle))
	{
		if (xQueueReceive(TemperatureQueueHandle, &(pRxTemperature), (TickType_t)10) != pdTRUE)
		{
			while (1);
		}

		RadioPacket.packet.temperature[0] = pRxTemperature[0];
		RadioPacket.packet.temperature[1] = pRxTemperature[1];
	}
}

void RadioFillHumidityData(void)
{
	if (uxQueueMessagesWaiting(HumidityQueueHandle))
	{
		if (xQueueReceive(HumidityQueueHandle, &(pRxHumidity), (TickType_t)10) != pdTRUE)
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

/* RadioTaskFunction function */
void RadioTaskFunction(void const * argument)
{
	GetUniqueId();
	RadioFillIds();

	RadioInitLoraModule();
	RadioConfigLoraModule();

	for(;;)
	{
		RadioPacketBuilder();
		RadioTransmitPacket(RadioPacket.bytes, RADIO_PACKET_LENGTH);

		osDelay(1000);
	}
}
