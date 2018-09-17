/*
 * radio.h
 *
 *  Created on: 2018. szept. 17.
 *      Author: Kiss Márton Titusz
 */

#ifndef RADIO_H_
#define RADIO_H_

#include "main.h"
#include "SX1278.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

/* Function prototypes */
void RadioInitLoraModule(void);
void RadioConfigLoraModule(void);
void RadioTransmitPacket(char* message_buffer, int message_length);
uint8_t RadioReceivePacket(char* message_buffer);

void RadioTaskFunction(void const * argument);

#endif /* RADIO_H_ */
