/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_host.h"
#include "usbh_def.h"
#include "ff.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern ApplicationTypeDef Appli_state;
extern USBH_HandleTypeDef hUsbHostFS;
extern char USBHPath[4];  		/* USBH logical drive path */
 
FATFS USBDISKFatFs;           /* File system object for USB disk logical drive */
FIL   MyFile;                 /* File object */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void MSC_Application(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_FATFS_Init();
  MX_UART8_Init();
  /* USER CODE BEGIN 2 */
  printf("start .... usb test \r\n");
  /* USER CODE END 2 */
 
  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 
 
  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//    switch(Appli_state)
//    {
//      case APPLICATION_READY:
//        MSC_Application();
//        Appli_state = APPLICATION_DISCONNECT;
//        break;
//      
//      case APPLICATION_DISCONNECT:
//        f_mount(NULL, "", 0);
//        break;
//      
//      default:
//        break;
//    }
    

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void MSC_Application(void)
{
  FRESULT res;                             /* FatFs function common result code */
  uint32_t byteswrite;
  uint8_t wtext[] = "hello world!";
if(FATFS_LinkDriver(&USBH_Driver, USBHPath) == 0){
  /* Register the file system object to the FatFs module */
  if(f_mount(&USBDISKFatFs, (TCHAR const*)USBHPath, 0) != FR_OK){
    /* FatFs Initialization Error */
    Error_Handler();
  }else{
    /* Create and Open a new text file object with write access */
    if(f_open(&MyFile, "hello.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK){
      /* 'STM32.TXT' file Open for write Error */
      Error_Handler();
    }else{
      res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswrite);
      if(byteswrite == 0 || (res != FR_OK)){
        Error_Handler();
      }else{
        /* Close the open text file */
        f_close(&MyFile);
        printf("write sucessful \r\n");
      }
    }
  }
}

FATFS_UnLinkDriver(USBHPath);

}


/*
---------------------------------------------------
USB Device Attached
PID: 1000h
VID: 90ch
Address (#1) assigned.
Manufacturer : General
Product : USB Flash Disk
Serial Number : 0425440000006958
Enumeration done.
This device has only 1 configuration.
Default configuration set.
Switching to Interface (#0)
Class    : 8h
SubClass : 6h
Protocol : 50h
MSC class started.
Number of supported LUN: 2
LUN #0: 
Inquiry Vendor  : General 
Inquiry Product : USB Flash Disk  
Inquiry Version : 1100
MSC Device ready
MSC Device capacity : 3758095872 Bytes
Block number : 15728639
Block Size   : 512
LUN #1: 
Inquiry Vendor  : General 
Inquiry Product : USB Flash Disk  
Inquiry Version : 1100
---------------------------------------------------
start .... usb test 
USB Device Attached
PID: 226h
VID: 1908h
Address (#1) assigned.
Manufacturer : N/A
Product : N/A
Serial Number : N/A
Enumeration done.
This device has only 1 configuration.
Default configuration set.
Switching to Interface (#0)
Class    : 8h
SubClass : 6h
Protocol : 50h
MSC class started.
Number of supported LUN: 2
LUN #0: 
Inquiry Vendor  : Generic 
Inquiry Product : Mass-Storage    
Inquiry Version : 1.11
MSC Device ready
MSC Device capacity : 501218816 Bytes
Block number : 978943
Block Size   : 512
LUN #1: 
Inquiry Vendor  : Generic 
Inquiry Product : Mass-Storage    
Inquiry Version : 1.11
MSC Device ready
MSC Device capacity : 501218816 Bytes
Block number : 978943
Block Size   : 512
---------------------------------------------------
USB Device Attached
PID: b113h
VID: 1005h
Address (#1) assigned.
Manufacturer :         
Product : USB FLASH DRIVE
Serial Number : 070224D04ED6C013
Enumeration done.
This device has only 1 configuration.
Default configuration set.
Switching to Interface (#0)
Class    : 8h
SubClass : 6h
Protocol : 50h
MSC class started.
Number of supported LUN: 2
LUN #0: 
Inquiry Vendor  :         
Inquiry Product : USB FLASH DRIVE 
Inquiry Version : PMAP
MSC Device ready
MSC Device capacity : 3128950272 Bytes
Block number : 31277055
Block Size   : 512
LUN #1: 
Inquiry Vendor  :         
Inquiry Product : USB FLASH DRIVE 
Inquiry Version : PMAP
Sense Key  : 5
Additional Sense Code : 25
Additional Sense Code Qualifier: 20
---------------------------------------------------
*/
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
