/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void writeNFC(uint16_t deviceAddress, uint16_t registerAddress, uint8_t data){
	uint8_t buffer[3] = {0};

	buffer[0] = registerAddress>>8; //Register Address MSB
	buffer[1] = registerAddress; //Register Address LSB
	buffer[2] = data; //Date to Write
	HAL_I2C_Master_Transmit(&hi2c1, deviceAddress<<1, buffer, 3, 10);
}

uint8_t readNFC(uint16_t deviceAddress, uint16_t registerAddress){
	uint8_t buffer[2] = {0};

	buffer[0] = registerAddress>>8; //Register Address MSB
	buffer[1] = registerAddress; //Register Address LSB
	HAL_I2C_Master_Transmit(&hi2c1, deviceAddress<<1, buffer, 2, 10); //Choose Register

	buffer[0] = 0;
	buffer[1] = 0;
	HAL_I2C_Master_Receive(&hi2c1, deviceAddress<<1, buffer, 1, 10); //Read Byte

	return(buffer[0]);
}

void addToOpenings(){
	  //Read from I2C device information
	  HAL_GPIO_WritePin(GPIOA, VDD_EEPROM_Pin, GPIO_PIN_SET); //Give Power to ST25 EEPROM
	  HAL_Delay(10); //Wait for EEPROM to power up

	  uint16_t deviceAddress = 0b1010011;
	  //deviceAddress = 0b1010111;
	  /*
	  I2C device found at address 0x2D  !
	  I2C device found at address 0x53  !
	  I2C device found at address 0x57  !
	  */

	//Check if I2C Device is Valid
	if(HAL_I2C_IsDeviceReady(&hi2c1, deviceAddress<<1, 2, 10) == HAL_OK){

		uint8_t data = 0;
		uint16_t regAdd = 0;

		data = readNFC(deviceAddress, regAdd);
		HAL_Delay(20);
		data++;
		writeNFC(deviceAddress, regAdd, data);
	}
}

void zeroOpenings(){

	HAL_GPIO_WritePin(GPIOA, VDD_EEPROM_Pin, GPIO_PIN_SET); //Give Power to ST25 EEPROM
	HAL_Delay(10); //Wait for EEPROM to power up
	uint16_t deviceAddress = 0b1010011;


	//Check if I2C Device is Valid
	if(HAL_I2C_IsDeviceReady(&hi2c1, deviceAddress<<1, 2, 10) == HAL_OK){
		uint8_t data = 0;
		uint16_t regAdd = 0;
		writeNFC(deviceAddress, regAdd, data);
	}
}

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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t flop = 0;
  zeroOpenings();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

      if (HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin) == 0 && flop == 0)
      {
          // Turn on LED on PA0 (LED circuit is active low)
    	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    	  flop = 1;

    	  addToOpenings();
      }
      if (HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin) == 1 && flop == 1)
      {
          // Turn off LED on PA0
    	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    	  flop = 0;
      }

/*
      if (HAL_GPIO_ReadPin(GPO_GPIO_Port, GPO_Pin) == 1)
      {
          // Turn on LED on PA0 (LED circuit is active low)
    	  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    	  HAL_Delay(1000);
      }*/

      //HAL_Delay(100);

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
