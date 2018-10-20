/*
 * radio.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "radio.h"

SX1278_hw_t SX1278_hw;
SX1278_t 	SX1278;

extern uint8_t TemperatureBuffer[2];
extern uint8_t HumidityBuffer[2];
extern bool dataAvailable;

extern SPI_HandleTypeDef hspi1;

extern osSemaphoreId TemperatureSemaphoreHandle;
extern osSemaphoreId HumiditySemaphoreHandle;

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
	RadioFillPacketCounter();
	RadioFillTemperatureData();
	RadioFillHumidityData();
}

void GetUniqueId(void)
{
	DeviceId.bytes[0]	= HAL_GetUIDw0();
	DeviceId.bytes[1]	= HAL_GetUIDw1();
	DeviceId.bytes[2]	= HAL_GetUIDw2();
}

void RadioFillDeviceId(void)
{
	RadioPacket.packet.deviceId[0]	= DeviceId.bytes[0];
	RadioPacket.packet.deviceId[1]	= DeviceId.bytes[1];
	RadioPacket.packet.deviceId[2]	= DeviceId.bytes[2];
}

void RadioFillPacketCounter(void)
{
	RadioPacket.packet.packetId++;
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
	RadioFillTemperatureData();
	RadioFillHumidityId();
	RadioFillHumidityData();
}

void RadioFillTemperatureData(void)
{
	if (xSemaphoreTake(TemperatureSemaphoreHandle, 100))
	{
		RadioPacket.packet.temperature[0] = TemperatureBuffer[0];
		RadioPacket.packet.temperature[1] = TemperatureBuffer[1];

		xSemaphoreGive(TemperatureSemaphoreHandle);
	}
}

void RadioFillHumidityData(void)
{
	if (xSemaphoreTake(HumiditySemaphoreHandle, 100))
	{
		//RadioPacket.packet.humidity[0] = HumidityBuffer[0];
		//RadioPacket.packet.humidity[1] = HumidityBuffer[1];

		RadioPacket.packet.humidity[0] = 120;
		RadioPacket.packet.humidity[1] = 210;

		xSemaphoreGive(HumiditySemaphoreHandle);
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
		if (dataAvailable == true)
		{
			RadioTransmitPacket(RadioPacket.bytes, RADIO_PACKET_LENGTH);
			RadioIncreasePacketId();
		}

		osDelay(1000);
	}
}
