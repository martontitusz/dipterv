/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LORA_DIO0_Pin GPIO_PIN_3
#define LORA_DIO0_GPIO_Port GPIOA
#define LORA_DIO0_EXTI_IRQn EXTI3_IRQn
#define LORA_SPI_NSS_Pin GPIO_PIN_4
#define LORA_SPI_NSS_GPIO_Port GPIOA
#define LORA_SPI_SCK_Pin GPIO_PIN_5
#define LORA_SPI_SCK_GPIO_Port GPIOA
#define LORA_SPI_MISO_Pin GPIO_PIN_6
#define LORA_SPI_MISO_GPIO_Port GPIOA
#define LORA_SPI_MOSI_Pin GPIO_PIN_7
#define LORA_SPI_MOSI_GPIO_Port GPIOA
#define LORA_DIO1_Pin GPIO_PIN_4
#define LORA_DIO1_GPIO_Port GPIOC
#define LORA_DIO1_EXTI_IRQn EXTI4_IRQn
#define LORA_DIO2_Pin GPIO_PIN_5
#define LORA_DIO2_GPIO_Port GPIOC
#define LORA_DIO3_Pin GPIO_PIN_0
#define LORA_DIO3_GPIO_Port GPIOB
#define LORA_DIO4_Pin GPIO_PIN_1
#define LORA_DIO4_GPIO_Port GPIOB
#define LORA_DIO5_Pin GPIO_PIN_2
#define LORA_DIO5_GPIO_Port GPIOB
#define SPI2_NSS2_Pin GPIO_PIN_14
#define SPI2_NSS2_GPIO_Port GPIOB
#define SPI2_NSS1_Pin GPIO_PIN_15
#define SPI2_NSS1_GPIO_Port GPIOB
#define SPI2_NSS0_Pin GPIO_PIN_6
#define SPI2_NSS0_GPIO_Port GPIOC
#define USER_GPIO2_Pin GPIO_PIN_9
#define USER_GPIO2_GPIO_Port GPIOA
#define USER_GPIO1_Pin GPIO_PIN_10
#define USER_GPIO1_GPIO_Port GPIOA
#define USER_GPIO0_Pin GPIO_PIN_11
#define USER_GPIO0_GPIO_Port GPIOA
#define HDC2010_INT_Pin GPIO_PIN_5
#define HDC2010_INT_GPIO_Port GPIOB
#define HDC2010_INT_EXTI_IRQn EXTI9_5_IRQn
#define HDC2010_I2C_SCL_Pin GPIO_PIN_6
#define HDC2010_I2C_SCL_GPIO_Port GPIOB
#define HDC2010_I2C_SDA_Pin GPIO_PIN_7
#define HDC2010_I2C_SDA_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
typedef enum StateEnum
{
	PacketBuilding,
	Tx,
	Rx,
	PacketReceived,
	Standby,
	Sleep
} radioState_t;

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
