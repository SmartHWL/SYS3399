/**************************************************************************//**
 * @file     BSP_gpio.c
 * @brief    GPIO related Configuration Source File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/

/******************************************************************************
* include file
*******************************************************************************/
#include "SC32F58128.h"
#include "BSP_gpio.h"

/******************************************************************************
* global data for the project
*******************************************************************************/

/******************************************************************************
* loacal data for the current file
*******************************************************************************/

/******************************************************************************/
/* local prototypes function **************************************************/
/******************************************************************************/

/******************************************************************************
* loacal function for the current file
*******************************************************************************/

/******************************************************************************
* global function for the current project
*******************************************************************************/

/**
  * @brief  Gpio Initialization.
*/
void BSP_GpioInit(void)
{
	
  /* LED Gpio Config */
  GPIO_OutputEnable(GPIO_LED1);
  GPIO_OutputEnable(GPIO_LED2);
  GPIO_OutputEnable(GPIO_LED3);
  GPIO_OutputEnable(GPIO_LED3);	
	
	LED1_OFF;
	LED2_ON;
	LED3_OFF;
	
	GPIO_OutputEnable(GPIO_BUZZER_PIN);
    BUZZER_LOW;
	

}

