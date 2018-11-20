/*
 * radio.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "radio.h"

SX1278_hw_t	SX1278_hw;
SX1278_t	SX1278;

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

uint8_t TxBuffer[5]			= {	RADIO_ACK_MESSAGE,
								RADIO_ACK_MESSAGE,
								RADIO_ACK_MESSAGE,
								RADIO_ACK_MESSAGE,
								RADIO_ACK_MESSAGE };
uint8_t	RetransmitCounter	=	0;

bool	RadioInSleepMode	=	false;

extern bool						RadioInRxMode;
struct radiopacket_t			*pRadioRxPacket;
extern SPI_HandleTypeDef		hspi1;
extern TIM_HandleTypeDef 		htim2;
extern osMessageQId 			RadioPacketQueueHandle;
extern radioState_t 			RadioState;


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

		if (RadioPacket.packet.deviceId[0]==0x42003c)
		{
			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		}
		else if (RadioPacket.packet.deviceId[0]==0x4a002e)
		{
			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		}

		return 0;
	}

	return 1;
}

/********************************/
/* StateMachine */
void RadioStateMachineFunction(void)
{
	switch (RadioState)
	{
		case Tx:
		{
			RadioTxStateFunction();
			break;
		}
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
		case Standby:
		{
			RadioStandbyStateFunction();
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

void RadioTxStateFunction(void)
{
	if (RetransmitCounter == RADIO_ACK_MAX_RETRANSMISSIONS)
	{
		RetransmitCounter = 0;
		RadioState = ChangeChannel;
	}
	else
	{
		if (!RetransmitCounter)
		{
			SX1278_LoRaEntryTx(&SX1278, RADIO_ACK_MESSAGE_LENGTH, RADIO_TX_TIMEOUT);
		}
		TxBuffer[4] = RetransmitCounter;
		SX1278_LoRaTxPacket(&SX1278, TxBuffer, RADIO_PACKET_LENGTH, RADIO_TX_TIMEOUT);
		RetransmitCounter++;
	}
}

void RadioRxStateFunction(void)
{
	if (!RadioInRxMode)
	{
		SX1278_receive(&SX1278, RADIO_PACKET_LENGTH, RADIO_RX_TIMEOUT);
		RadioInRxMode = true;
		HAL_TIM_Base_Start_IT(&htim2);
	}
	else {}
}

void RadioPacketReceivedStateFunction(void)
{
	RadioReceivePacket();
	RadioState = ChangeChannel;
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

	RadioState = Rx;
}

void RadioStandbyStateFunction(void)
{
	SX1278_standby(&SX1278);
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

/********************************/

/* RadioTaskFunction function */
void RadioTaskFunction(void const * argument)
{
	RadioInitLoraModule();
	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8, UsedChannels[ChannelIndex], 10);

	pRadioRxPacket = & RadioPacket.packet;

	RadioState = Rx;
	SX1278_receive(&SX1278, RADIO_PACKET_LENGTH, RADIO_RX_TIMEOUT);

	for(;;)
	{
		RadioStateMachineFunction();
		osDelay(1);
	}
}
