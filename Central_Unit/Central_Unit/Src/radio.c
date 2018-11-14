/*
 * radio.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "radio.h"

SX1278_hw_t	SX1278_hw;
SX1278_t	SX1278;

uint8_t CurrentChannel = SX1278_LORA_BW_15_6KHZ;
uint8_t TxBuffer[5] = {RADIO_ACK_MESSAGE, RADIO_ACK_MESSAGE, RADIO_ACK_MESSAGE, RADIO_ACK_MESSAGE, RADIO_ACK_MESSAGE};

struct radiopacket_t		*pRadioRxPacket;
extern SPI_HandleTypeDef	hspi1;
extern osMessageQId 		RadioPacketQueueHandle;


void RadioInitLoraModule(void)
{
	SX1278_hw.dio0.port		= LORA_DIO0_GPIO_Port;
	SX1278_hw.dio0.pin		= LORA_DIO0_Pin;
	SX1278_hw.nss.port		= LORA_SPI_NSS_GPIO_Port;
	SX1278_hw.nss.pin		= LORA_SPI_NSS_Pin;
	SX1278_hw.reset.port	= NULL;
	SX1278_hw.reset.pin 	= 0;
	SX1278_hw.spi			= &hspi1;

	SX1278.hw				= &SX1278_hw;
}

void RadioConfigLoraModule(uint8_t channel)
{
	//SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8, SX1278_LORA_BW_15_6KHZ/*SX1278_LORA_BW_20_8KHZ*/, 10);
	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8, channel, 10);
	//SX1278_LoRaEntryRx(&SX1278, RADIO_PACKET_LENGTH, 2000);
	SX1278_LoRaEntryTx(&SX1278, RADIO_ACK_MESSAGE_LENGTH, 2000);

}

uint8_t RadioChangeChannel(uint8_t old_channel)
{
	uint8_t newchannel;

	if (old_channel == SX1278_LORA_BW_15_6KHZ)
	{
		newchannel = SX1278_LORA_BW_20_8KHZ;
	}
	else
	{
		newchannel = SX1278_LORA_BW_15_6KHZ;
	}
	return newchannel;
}

void RadioTransmitPacket(uint8_t* message_buffer, int message_length)
{
	SX1278_LoRaEntryTx(&SX1278, message_length, 2000);
	SX1278_LoRaTxPacket(&SX1278, message_buffer, message_length, 2000);
}

uint8_t RadioReceivePacket(void)
{
	uint8_t received_bytes = SX1278_LoRaRxPacket(&SX1278);

	if (received_bytes > 0)
	{
		SX1278_read(&SX1278, RadioPacket.bytes, received_bytes);
		if (xQueueSendToBack(RadioPacketQueueHandle, (void *) &pRadioRxPacket, (TickType_t)10) != pdPASS)
		{
			/* Failed to post the message, even after 10 ticks. */
			while(1);
		}
		RadioTransmitAck();
		return 0;
	}

	return 1;
}

void RadioTransmitAck(void)
{
	//SX1278_LoRaEntryTx(&SX1278, RADIO_ACK_MESSAGE_LENGTH, 2000);

	for (int i = 0; i < /*RADIO_ACK_RETRANSMISSIONS*/1; i++)
	{
		SX1278_LoRaTxPacket(&SX1278, TxBuffer, RADIO_ACK_MESSAGE_LENGTH, 2000);
		//osDelay(1);
	}

	//SX1278_LoRaEntryRx(&SX1278, RADIO_PACKET_LENGTH, 2000);
}

/* RadioTaskFunction function */
void RadioTaskFunction(void const * argument)
{
	//TxBuffer[0] = RADIO_ACK_MESSAGE;
	RadioInitLoraModule();
	RadioConfigLoraModule(CurrentChannel);
	//RadioConfigLoraModule(SX1278_LORA_BW_20_8KHZ);

	pRadioRxPacket = & RadioPacket.packet;

	for(;;)
	{
		//RadioReceivePacket();
		RadioTransmitAck();
		//CurrentChannel = RadioChangeChannel(CurrentChannel);
		//RadioConfigLoraModule(CurrentChannel);

		osDelay(10);
	}
}
