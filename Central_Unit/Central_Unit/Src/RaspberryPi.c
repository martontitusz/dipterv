/*
 * RaspberryPi.c
 *
 *  Created on: 2018. okt. 7.
 *      Author: Kiss Márton Titusz
 */

#include "RaspberryPi.h"

extern osMessageQId RadioPacketQueueHandle;
extern I2C_HandleTypeDef hi2c1;
extern bool isI2CBusy;
union radiopacket_union_t tempRadioPacket;
extern uint8_t NumberOfAvailablePackets;

struct radiopacket_t *pRxPacket;


void RaspberryPiFillI2CBuffer(void)
{
	if (xQueueReceive(RadioPacketQueueHandle, &(pRxPacket), (TickType_t)10) != pdTRUE)
	{
		while (1);
	}

	tempRadioPacket.packet.deviceId[0]		=	pRxPacket->deviceId[0];
	tempRadioPacket.packet.deviceId[1]		=	pRxPacket->deviceId[1];
	tempRadioPacket.packet.deviceId[2]		=	pRxPacket->deviceId[2];
	tempRadioPacket.packet.packetId			=	pRxPacket->packetId;
	tempRadioPacket.packet.temperatuteId	=	pRxPacket->temperatuteId;
	tempRadioPacket.packet.temperature[0]	=	pRxPacket->temperature[0];
	tempRadioPacket.packet.temperature[1]	=	pRxPacket->temperature[1];
	tempRadioPacket.packet.humidityId		=	pRxPacket->humidityId;
	tempRadioPacket.packet.humidity[0]		=	pRxPacket->humidity[0];
	tempRadioPacket.packet.humidity[1]		=	pRxPacket->humidity[1];

	HAL_I2C_DisableListen_IT(&hi2c1);
	HAL_I2C_Slave_Transmit_IT(&hi2c1, (tempRadioPacket.bytes - 1), RADIO_PACKET_LENGTH + 1);
}

void RaspberryPiTaskFunction(void const * argument)
{
	HAL_I2C_EnableListen_IT(&hi2c1);

	for(;;)
	{
		NumberOfAvailablePackets = uxQueueMessagesWaiting(RadioPacketQueueHandle);

		if (isI2CBusy == false)
		{
			if (NumberOfAvailablePackets != 0)
			{
				RaspberryPiFillI2CBuffer();
				isI2CBusy = true;
			}
		}

		osDelay(10);
	}
}
