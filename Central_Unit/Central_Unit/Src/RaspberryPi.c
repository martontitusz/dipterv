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


void RaspberryPiFillI2CBuffer(void)
{
	if (xQueueReceive(RadioPacketQueueHandle, tempRadioPacket.bytes, 10) != pdTRUE)
	{
		while (1);
	}
}

void RaspberryPiTaskFunction(void const * argument)
{

  for(;;)
  {
	  NumberOfAvailablePackets = uxQueueMessagesWaiting(RadioPacketQueueHandle);

	  if (isI2CBusy == false)
	  {
		  if (NumberOfAvailablePackets != 0)
		  {
			  RaspberryPiFillI2CBuffer();
			  HAL_I2C_Slave_Transmit_IT(&hi2c1, tempRadioPacket.bytes, RADIO_PACKET_LENGTH);
			  isI2CBusy = true;
			  HAL_GPIO_WritePin(RPI_GPIO_I2C_BUSY_PORT, RPI_GPIO_I2C_BUSY_PIN, SET);
		  }
	  }
	  osDelay(1);
  }
}
