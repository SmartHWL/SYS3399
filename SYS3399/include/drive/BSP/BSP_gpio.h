/**************************************************************************//**
 * @file     BSP_gpio.h
 * @brief    GPIO Related Config Header File
 * @version  V1.00
 * @date     25. March 2017
 *
 * @note
 *
 ******************************************************************************/
#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

/********************************************************************************
 * Include section
 *******************************************************************************/
#include "SC32F58128_Gpio.h"

/********************************************************************************
 * Constants and macros
 *******************************************************************************/
/*上电IO*/
#define GPIO_POWER      	 GPIO_26
#define GPIO_POWERGET      GPIO_9

#define SET_CTRL_POWER_RLY_HIGH()              GPIO_SetHigh(GPIO_POWER)
#define SET_CTRL_POWER_RLY_LOW()               GPIO_SetLow(GPIO_POWER)
#define START_HANDLER													 GPIO_GetGpioStatus(GPIO_POWERGET)

/*充电功能IO*/
#define GPIO_CHARGE        GPIO_35
#define GPIO_CHARGEGET     GPIO_92
#define GPIO_CHARGEOK      GPIO_94

#define SET_CTRL_CHARGE_RLY_ENABLE()           GPIO_SetLow(GPIO_CHARGE)   
#define SET_CTRL_CHARGE_RLY_DISABLE()          GPIO_SetHigh(GPIO_CHARGE)   

/*防反接*/
#define GPIO_ANTIREVERSE        GPIO_8

#define SET_CTRL_ANTI_RLY_ENABLE()        GPIO_SetHigh(GPIO_ANTIREVERSE)     
#define SET_CTRL_ANTI_RLY_DISABLE()       GPIO_SetLow(GPIO_ANTIREVERSE)     


/*左倾倒开关*/
#define GPIO_LEFTLIFT        GPIO_15
#define LEFTLIFT_HANDLER									GPIO_GetGpioStatus(GPIO_LEFTLIFT)

/*右倾倒开关*/
#define GPIO_RIGHTLIFT       GPIO_34
#define RIGHTLIFT_HANDLER									GPIO_GetGpioStatus(GPIO_RIGHTLIFT)

/*蜂鸣器*/
#define GPIO_BUZZER       GPIO_27
#define SET_CTRL_BUZZER_ENABLE()         GPIO_SetHigh(GPIO_BUZZER)   
#define SET_CTRL_BUZZER_DISABLE()        GPIO_SetLow(GPIO_BUZZER)     
#define SET_CTRL_BUZZER_TOGGLE()         GPIO_Toggle(GPIO_BUZZER)



#define GPIO_WATCHER_TXD   GPIO_29
#define GPIO_WATCHER_RXD   GPIO_28

#define GPIO_CLI_TXD   GPIO_22
#define GPIO_CLI_RXD   GPIO_23

#define GPIO_LED1          GPIO_44
#define GPIO_LED2          GPIO_1
#define GPIO_LED3          GPIO_38
#define GPIO_TESTPIN       GPIO_21
#define GPIO_BUZZER_PIN                         GPIO_0

#define LED1_ON            GPIO_SetLow  (GPIO_LED1)
#define LED1_OFF           GPIO_SetHigh (GPIO_LED1)	
#define LED1_FLASH         GPIO_Toggle	(GPIO_LED1)

#define LED2_ON            GPIO_SetLow  (GPIO_LED2)
#define LED2_OFF           GPIO_SetHigh (GPIO_LED2)	
#define LED2_FLASH         GPIO_Toggle	(GPIO_LED2)
#define LED3_ON            GPIO_SetLow  (GPIO_LED3)
#define LED3_OFF           GPIO_SetHigh (GPIO_LED3)	
#define LED3_FLASH         GPIO_Toggle	(GPIO_LED3)

#define TESTPIN_HIGH       GPIO_SetHigh (GPIO_TESTPIN)	
#define TESTPIN_LOW        GPIO_SetLow  (GPIO_TESTPIN)	
#define TESTPIN_TOGGLE     GPIO_Toggle	(GPIO_TESTPIN)

#define BUZZER_HIGH       GPIO_SetHigh (GPIO_BUZZER_PIN)	
#define BUZZER_LOW        GPIO_SetLow  (GPIO_BUZZER_PIN)	
#define BUZZER_TOGGLE     GPIO_Toggle	(GPIO_BUZZER_PIN)


/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/


/********************************************************************************
* Variables
*******************************************************************************/


/********************************************************************************
* prototypes of the functions
*******************************************************************************/
extern void BSP_GpioInit(void);


/*---------------------------------- End of the file ---------------------------------*/
#endif /* __BSP_GPIO_H__ */

