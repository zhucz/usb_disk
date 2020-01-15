/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
/* USER CODE END 0 */

UART_HandleTypeDef huart8;

/* UART8 init function */
void MX_UART8_Init(void)
{

  huart8.Instance = UART8;
  huart8.Init.BaudRate = 115200;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART8)
  {
  /* USER CODE BEGIN UART8_MspInit 0 */

  /* USER CODE END UART8_MspInit 0 */
    /* UART8 clock enable */
    __HAL_RCC_UART8_CLK_ENABLE();
  
    __HAL_RCC_GPIOE_CLK_ENABLE();
    /**UART8 GPIO Configuration    
    PE0     ------> UART8_RX
    PE1     ------> UART8_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART8;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* USER CODE BEGIN UART8_MspInit 1 */

  /* USER CODE END UART8_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART8)
  {
  /* USER CODE BEGIN UART8_MspDeInit 0 */

  /* USER CODE END UART8_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART8_CLK_DISABLE();
  
    /**UART8 GPIO Configuration    
    PE0     ------> UART8_RX
    PE1     ------> UART8_TX 
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN UART8_MspDeInit 1 */

  /* USER CODE END UART8_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//__use_no_semihosting was requested, but _ttywrch was 
void _ttywrch(int ch)
{
	ch = ch;
}

//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((UART8->SR&0X40)==0);//循环发送,直到发送完毕   
	UART8->DR = (uint8_t) ch;      
	return ch;
}
#endif
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
