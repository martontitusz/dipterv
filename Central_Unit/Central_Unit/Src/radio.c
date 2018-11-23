/*
 * radio.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "radio.h"

extern SPI_HandleTypeDef		hspi1;
extern TIM_HandleTypeDef 		htim2;
extern osMessageQId 			RadioPacketQueueHandle;

SX1278_hw_t						SX1278_hw;
SX1278_t						SX1278;

uint8_t UsedChannels[]		= {	//SX1278_LORA_BW_7_8KHZ,
								//SX1278_LORA_BW_10_4KHZ,
								SX1278_LORA_BW_15_6KHZ,
								SX1278_LORA_BW_20_8KHZ
								//SX1278_LORA_BW_31_2KHZ,
								//SX1278_LORA_BW_41_7KHZ,
								//SX1278_LORA_BW_62_5KHZ,
								//SX1278_LORA_BW_125KHZ,
								//SX1278_LORA_BW_250KHZ,
								//SX1278_LORA_BW_500KHZ
								};
uint8_t ChannelIndex		=	0;
uint8_t NrOfUsedChannels	=	sizeof(UsedChannels);
uint8_t RadioRxModeState	=	RADIO_NOT_INRXMODE;
radioState_t RadioState		=	Rx;
struct	radiopacket_t			*pRadioRxPacket;

uint32_t DeviceIdSM1[3]		=	{0x42003c,0x5246500e,0x20383352};
uint32_t DeviceIdSM2[3]		=	{0x4a002e,0x5246500e,0x20383352};

uint32_t LastPacketIdSM1	=	0;
uint32_t LastPacketIdSM2	=	0;


radioState_t RadioGetState(void)
{
	return RadioState;
}

void RadioSetState(radioState_t new_state)
{
	RadioState = new_state;
}

uint8_t RadioGetRxModeState(void)
{
	return RadioRxModeState;
}

void RadioSetRxModeState(uint8_t new)
{
	RadioRxModeState = new;
}

void RadioInitLoraModule(void)
{
	SX1278_hw.dio0.port		=	LORA_DIO0_GPIO_Port;
	SX1278_hw.dio0.pin		=	LORA_DIO0_Pin;
	SX1278_hw.nss.port		=	LORA_SPI_NSS_GPIO_Port;
	SX1278_hw.nss.pin		=	LORA_SPI_NSS_Pin;
	SX1278_hw.reset.port	=	NULL;
	SX1278_hw.reset.pin 	=	0;
	SX1278_hw.spi			=	&hspi1;

	SX1278.hw				=	&SX1278_hw;
}

void RadioStateMachineFunction(void)
{
	switch ( RadioGetState() )
	{
		case Rx:
		{
			RadioRxStateFunction();
			break;
		}
		case PacketReceived:
		{
			RadioPacketReceivedStateFunction();
			break;
		}
		case ChangeChannel:
		{
			RadioChangeChannelStateFunction();
			break;
		}
		default:
		{
			break;
		}
	}
}

void RadioRxStateFunction(void)
{
	if ( !RadioGetRxModeState() )
	{
		SX1278_receive(&SX1278, RADIO_PACKET_LENGTH, RADIO_RX_TIMEOUT);
		RadioSetRxModeState(RADIO_INRXMODE);
		HAL_TIM_Base_Start_IT(&htim2);
	}
	else {}
}

uint8_t RadioReceivePacket(void)
{
	uint8_t received_bytes = SX1278_LoRaRxPacket(&SX1278);

	if (received_bytes > 0)
	{
		SX1278_read(&SX1278, RadioPacket.bytes, received_bytes);

		if ( RadioComparePackets() )
		{
			if (xQueueSendToBack(RadioPacketQueueHandle, (void *) &pRadioRxPacket, (TickType_t)10) != pdPASS)
			{
				/* Failed to post the message, even after 10 ticks. */
				while(1);
			}
			return 0;
		}
		else
		{
			return 1;
		}
	}

	return 2;
}

uint8_t RadioComparePackets(void)
{
	if (	(DeviceIdSM1[0] == RadioPacket.packet.deviceId[0]) &&
			(DeviceIdSM1[1] == RadioPacket.packet.deviceId[1]) &&
			(DeviceIdSM1[2] == RadioPacket.packet.deviceId[2])		)
	{
		if (LastPacketIdSM1 == RadioPacket.packet.packetId)
		{
			return 0;
		}
		else
		{
			LastPacketIdSM1 = RadioPacket.packet.packetId;
			return 1;
		}
	}
	else if((DeviceIdSM2[0] == RadioPacket.packet.deviceId[0]) &&
			(DeviceIdSM2[1] == RadioPacket.packet.deviceId[1]) &&
			(DeviceIdSM2[2] == RadioPacket.packet.deviceId[2])		)
	{
		if (LastPacketIdSM2 == RadioPacket.packet.packetId)
		{
			return 0;
		}
		else
		{
			LastPacketIdSM2 = RadioPacket.packet.packetId;
			return 1;
		}
	}
	else
	{
		return 2;
	}
}

void RadioPacketReceivedStateFunction(void)
{
	RadioReceivePacket();
	RadioSetState(ChangeChannel);
}

void RadioSelectChannel(void)
{
	if (ChannelIndex == (NrOfUsedChannels - 1))
	{
		ChannelIndex = 0;
	}
	else
	{
		ChannelIndex++;
	}
	SX1278.LoRa_BW = UsedChannels[ChannelIndex];
}

void RadioChangeChannelStateFunction(void)
{
	RadioSelectChannel();
	RadioSetState(Rx);
}


/* RadioTaskFunction function */
void RadioTaskFunction(void const * argument)
{
	RadioInitLoraModule();
	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8, UsedChannels[ChannelIndex], 10);

	pRadioRxPacket = & RadioPacket.packet;

	for(;;)
	{
		RadioStateMachineFunction();
		osDelay(1);
	}
}
