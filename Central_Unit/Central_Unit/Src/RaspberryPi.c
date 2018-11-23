/*
 * RaspberryPi.c
 *
 *  Created on: 2018. okt. 7.
 *      Author: Kiss Márton Titusz
 */

#include "RaspberryPi.h"

extern	osMessageQId					RadioPacketQueueHandle;
extern	I2C_HandleTypeDef				hi2c1;

struct 	radiopacket_t					*pRxPacket;
union	radiopacket_union_t				tempRadioPacket;
uint8_t NumberOfAvailablePackets	= 	0;
uint8_t I2CState					=	I2CLAZY;


uint8_t RaspberryPiGetNumberOfAvailablePackets(void)
{
	return NumberOfAvailablePackets;
}

void RaspberryPiSetNumberOfAvailablePackets(uint8_t new)
{
	NumberOfAvailablePackets = new;
}

uint8_t RaspberryPiGetI2CState(void)
{
	return I2CState;
}

void RaspberryPiSetI2CState(uint8_t new)
{
	I2CState = new;
}

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
		RaspberryPiSetNumberOfAvailablePackets( uxQueueMessagesWaiting(RadioPacketQueueHandle) );

		if ( !RaspberryPiGetI2CState() )
		{
			if (NumberOfAvailablePackets != 0)
			{
				RaspberryPiFillI2CBuffer();
				RaspberryPiSetI2CState(I2CBUSY);
			}
		}

		osDelay(10);
	}
}
