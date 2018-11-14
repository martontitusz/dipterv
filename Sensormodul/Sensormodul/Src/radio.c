/*
 * radio.c
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#include "radio.h"

SX1278_hw_t SX1278_hw;
SX1278_t 	SX1278;

uint8_t		*pRxTemperature;
uint8_t		*pRxHumidity;

uint8_t		RxBuffer[5];
uint8_t		RetransmitCounter	= 0;
bool		RadioInRxMode		= false;
bool		RadioInSleepMode	= false;

extern osMessageQId			TemperatureQueueHandle;
extern osMessageQId			HumidityQueueHandle;
extern SPI_HandleTypeDef 	hspi1;
extern radioState_t			RadioState;

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

void RadioConfigLoraModule(void)
{
	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8, RADIO_BANDWIDTH, 10);
	SX1278_LoRaEntryTx(&SX1278, 16, RADIO_TX_TIMEOUT);
}

void RadioTransmitSinglePacket(uint8_t* message_buffer, int message_length)
{
	SX1278_LoRaEntryTx(&SX1278, message_length, RADIO_TX_TIMEOUT);
	SX1278_LoRaTxPacket(&SX1278, message_buffer, message_length, RADIO_TX_TIMEOUT);
}

uint8_t RadioReceivePacket(void)
{
	uint8_t received_bytes = SX1278_LoRaRxPacket(&SX1278);

	if (received_bytes > 0)
	{
		SX1278_read(&SX1278, RxBuffer, received_bytes);
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

/********************************/
/* StateMachine */
void RadioStateMachineFunction(void)
{
	switch (RadioState)
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

void RadioPacketBuildingStateFunction(void)
{
	RadioInSleepMode = false;
	RadioPacketBuilder();
	RadioState = Tx;
}

void RadioTxStateFunction(void)
{
	if (RetransmitCounter == RADIO_MAX_RETRANSMISSONS)
	{
		RetransmitCounter = 0;
		RadioState = Sleep;
	}
	else
	{
		RetransmitCounter++;
		SX1278_transmit(&SX1278, RadioPacket.bytes, RADIO_PACKET_LENGTH, RADIO_TX_TIMEOUT);
	}
}

void RadioRxStateFunction(void)
{
	if (!RadioInRxMode)
	{
		SX1278_receive(&SX1278, RADIO_ACK_MESSAGE_LENGTH, RADIO_RX_TIMEOUT);
		RadioInRxMode = true;
	}
	else {}
}

void RadioPacketReceivedStateFunction(void)
{
	RadioReceivePacket();

	if (RxBuffer[0] == RADIO_ACK_MESSAGE)
	{
		RadioState = Sleep;
	}
	else
	{
		RadioState = Tx;
	}
	RadioInRxMode = false;
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
	GetUniqueId();
	RadioFillIds();

	RadioInitLoraModule();
	SX1278_begin(&SX1278, SX1278_433MHZ, SX1278_POWER_17DBM, SX1278_LORA_SF_8, RADIO_BANDWIDTH, 10);

	RadioState = Sleep;

	for(;;)
	{
//	FOR TEST AND DEBUG:
//		switch (RadioState)
//		{
//			case Tx:
//			{
//				HAL_GPIO_WritePin(USER_GPIO0_GPIO_Port, USER_GPIO0_Pin, GPIO_PIN_SET);
//				HAL_GPIO_WritePin(USER_GPIO1_GPIO_Port, USER_GPIO1_Pin, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(USER_GPIO2_GPIO_Port, USER_GPIO2_Pin, GPIO_PIN_RESET);
//				break;
//			}
//			case Rx:
//			{
//				HAL_GPIO_WritePin(USER_GPIO0_GPIO_Port, USER_GPIO0_Pin, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(USER_GPIO1_GPIO_Port, USER_GPIO1_Pin, GPIO_PIN_SET);
//				HAL_GPIO_WritePin(USER_GPIO2_GPIO_Port, USER_GPIO2_Pin, GPIO_PIN_RESET);
//				break;
//			}
//			case Sleep:
//			{
//				HAL_GPIO_WritePin(USER_GPIO0_GPIO_Port, USER_GPIO0_Pin, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(USER_GPIO1_GPIO_Port, USER_GPIO1_Pin, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(USER_GPIO2_GPIO_Port, USER_GPIO2_Pin, GPIO_PIN_SET);
//				break;
//			}
//			default:
//			{
//				HAL_GPIO_WritePin(USER_GPIO0_GPIO_Port, USER_GPIO0_Pin, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(USER_GPIO1_GPIO_Port, USER_GPIO1_Pin, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(USER_GPIO2_GPIO_Port, USER_GPIO2_Pin, GPIO_PIN_RESET);
//				break;
//			}
//		}
		RadioStateMachineFunction();

		osDelay(10);
	}
}
