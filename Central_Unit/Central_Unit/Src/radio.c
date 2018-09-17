/*
 * radio.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "radio.h"

SX1278_hw_t SX1278_hw;
SX1278_t SX1278;

char buffer[64];
extern uint8_t NumberOfAvailablePackets;
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
	int ret;

	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8, SX1278_LORA_BW_20_8KHZ, 10);
	ret = SX1278_LoRaEntryRx(&SX1278, 16, 2000);
}

void RadioTransmitPacket(char* message_buffer, int message_length)
{
	int ret;

	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 2000);
	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t *) message_buffer, message_length, 2000);
}

uint8_t RadioReceivePacket(char* message_buffer)
{
	uint8_t received_bytes;

	received_bytes = SX1278_LoRaRxPacket(&SX1278);
	if (received_bytes > 0)
	{
		SX1278_read(&SX1278, (uint8_t *) message_buffer, received_bytes);
	}

	return received_bytes;
}

/* RadioTaskFunction function */
void RadioTaskFunction(void const * argument)
{
	RadioInitLoraModule();
	RadioConfigLoraModule();

	for(;;)
	{
		NumberOfAvailablePackets += RadioReceivePacket(buffer);
		osDelay(10);
	}
}
