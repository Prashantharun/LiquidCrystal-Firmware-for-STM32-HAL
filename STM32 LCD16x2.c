#include "main.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void LCD_init(void);

void LCD_command(uint8_t value);

void LCD_data(uint8_t value);

void LCD_string(uint8_t *str);

uint16_t data_pins[8];


int main(void)
{

  HAL_Init();

	
  SystemClock_Config();


  MX_GPIO_Init();

// Testing all the pins
	/*
	   HAL_GPIO_WritePin(GPIOA, RS_Pin,GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOA, RW_Pin,GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOA, EN_Pin,GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOA, D0_Pin,GPIO_PIN_SET);
     HAL_GPIO_WritePin(GPIOA, D1_Pin,GPIO_PIN_SET);
     HAL_GPIO_WritePin(GPIOA, D2_Pin,GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOA, D3_Pin,GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOA, D4_Pin,GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOA, D5_Pin,GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOA, D6_Pin,GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOA, D7A12_Pin,GPIO_PIN_SET);	 	
*/
  
	
	data_pins[0] = D0_Pin;
  data_pins[1] = D1_Pin;
  data_pins[2] = D2_Pin;
  data_pins[3] = D3_Pin; 
  data_pins[4] = D4_Pin;
  data_pins[5] = D5_Pin;
  data_pins[6] = D6_Pin;
  data_pins[7] = D7A12_Pin;
	
	LCD_init();
	HAL_Delay(1000);
	//LCD_data(0X41);
//	LCD_data(0X42);
	LCD_string((uint8_t *)"SUCCESS");

  LCD_command(0x38);
	LCD_command(0XC0);                       // Moving the cursor to second line
	LCD_string((uint8_t *)"STM32 Board");

  while (1)
  {

  }

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Initialization of LCD in 8-bit mode

void LCD_init(void)
{
	 HAL_Delay(100);      // Give LCD time to intialize
	 LCD_command(0x38);  // Setting 8-bit mode
   LCD_command(0x38);
   LCD_command(0x38);


	 LCD_command(0x0C);  // Display ON and Cursor OFF


	 LCD_command(0x06);  // Auto Increment Cursor


	 LCD_command(0x01);  // Clear Display
	
	
	 LCD_command(0x80);  // Cursor at home position
	
	

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Command mode operations

void LCD_command(uint8_t value)
{
   HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(RW_GPIO_Port, RW_Pin, GPIO_PIN_RESET);
	
	// witing to data to D0, D1, D2, D3, D4, D5, D6, D7
	
	  for (int i = 0; i < 8; i++)
	{
    HAL_GPIO_WritePin(GPIOA, data_pins[i], ((value >> i) & 0x01)?GPIO_PIN_SET:GPIO_PIN_RESET);
  }

	// Pulse the Enable pin
	
	HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);    
  HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_SET);
  HAL_Delay(1);    // enable pulse must be >450ns
  HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);   // commands need > 37us to settle
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Data mode operations

void LCD_data(uint8_t value)
{
	 HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);
   HAL_GPIO_WritePin(RW_GPIO_Port, RW_Pin, GPIO_PIN_RESET);

	  for (int i = 0; i < 8; i++)
	{
    HAL_GPIO_WritePin(GPIOA, data_pins[i], ((value >> i) & 0x01)?GPIO_PIN_SET:GPIO_PIN_RESET);
  }

	// Pulse the Enable pin
	
  HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);    
  HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_SET);
  HAL_Delay(1);    // enable pulse must be >450ns
  HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);   // commands need > 37us to settle
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Printing the string

void LCD_string(uint8_t *str)
{
	int i = 0;
	
	while(str[i] != '\0')
	{
		 LCD_data(str[i]);
		 i++;
	}
   
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D7_Pin|D0_Pin|D1_Pin|D2_Pin 
                          |D3_Pin|D4_Pin|D5_Pin|D6_Pin 
                          |D7A12_Pin|RS_Pin|RW_Pin|EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : D7_Pin D0_Pin D1_Pin D2_Pin 
                           D3_Pin D4_Pin D5_Pin D6_Pin 
                           D7A12_Pin RS_Pin RW_Pin EN_Pin */
  GPIO_InitStruct.Pin = D7_Pin|D0_Pin|D1_Pin|D2_Pin 
                          |D3_Pin|D4_Pin|D5_Pin|D6_Pin 
                          |D7A12_Pin|RS_Pin|RW_Pin|EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
