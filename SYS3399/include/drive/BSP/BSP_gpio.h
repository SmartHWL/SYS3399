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
#include "BSP_pwm.h"
/********************************************************************************
 * Constants and macros
 *******************************************************************************/
#define UP_H_DOWN_H 					(1)   								
#define UP_L_DOWN_H 					(2)   								 
#define UP_L_DOWN_L 					(3)   								
#define UP_H_DOWN_L 					(4)   							
#define PREDRIVER_MODE 				(UP_H_DOWN_H) 


#define MCUOUT1_PIN  GPIO_0
#define MCUOUT2_PIN  GPIO_4
#define MCUOUT3_PIN  GPIO_5
#define MCUOUT4_PIN  GPIO_1
#define MCUOUT5_PIN  GPIO_2
#define MCUOUT6_PIN  GPIO_3
#define MCUOUT7_PIN  GPIO_6
#define MCUOUT8_PIN  GPIO_7
#define MCUOUT9_PIN  GPIO_25
#define MCUOUT10_PIN GPIO_44
#define MCUOUT11_PIN GPIO_41
#define MCUOUT12_PIN GPIO_8
#define MCUOUT13_PIN GPIO_9
#define MCUOUT14_PIN GPIO_10
#define MCUOUT15_PIN GPIO_11
#define MCUOUT16_PIN GPIO_12

#define MCUIN1_PIN  GPIO_26
#define MCUIN2_PIN  GPIO_60
#define MCUIN3_PIN  GPIO_42
#define MCUIN4_PIN  GPIO_43
#define MCUIN5_PIN  GPIO_36
#define MCUIN6_PIN  GPIO_35
#define MCUIN7_PIN  GPIO_84
#define MCUIN8_PIN  GPIO_86
#define MCUIN9_PIN  GPIO_90
#define MCUIN10_PIN  GPIO_92
#define MCUIN11_PIN  GPIO_94
#define MCUIN12_PIN  GPIO_27
#define MCUIN13_PIN  GPIO_40
#define MCUIN14_PIN  GPIO_39
#define MCUIN15_PIN  GPIO_14
#define MCUIN16_PIN  GPIO_13


#define MCUOUT1_HIGH     GPIO_SetHigh(MCUOUT1_PIN)
#define MCUOUT1_LOW      GPIO_SetLow(MCUOUT1_PIN)

#define MCUOUT2_HIGH     GPIO_SetHigh(MCUOUT2_PIN)
#define MCUOUT2_LOW      GPIO_SetLow(MCUOUT2_PIN) 

#define MCUOUT3_HIGH     GPIO_SetHigh(MCUOUT3_PIN)
#define MCUOUT3_LOW      GPIO_SetLow(MCUOUT3_PIN)

#define MCUOUT4_HIGH     GPIO_SetHigh(MCUOUT4_PIN)
#define MCUOUT4_LOW      GPIO_SetLow(MCUOUT4_PIN)

#define MCUOUT5_HIGH     GPIO_SetHigh(MCUOUT5_PIN)
#define MCUOUT5_LOW      GPIO_SetLow(MCUOUT5_PIN)

#define MCUOUT6_HIGH     GPIO_SetHigh(MCUOUT6_PIN)
#define MCUOUT6_LOW      GPIO_SetLow(MCUOUT6_PIN)

#define MCUOUT7_HIGH     GPIO_SetHigh(MCUOUT7_PIN)
#define MCUOUT7_LOW      GPIO_SetLow(MCUOUT7_PIN)

#define MCUOUT8_HIGH     GPIO_SetHigh(MCUOUT8_PIN)
#define MCUOUT8_LOW      GPIO_SetLow(MCUOUT8_PIN)

#define MCUOUT9_HIGH     GPIO_SetHigh(MCUOUT9_PIN)
#define MCUOUT9_LOW      GPIO_SetLow(MCUOUT9_PIN)

#define MCUOUT10_HIGH     GPIO_SetHigh(MCUOUT10_PIN)
#define MCUOUT10_LOW      GPIO_SetLow(MCUOUT10_PIN)

#define MCUOUT11_HIGH     GPIO_SetHigh(MCUOUT11_PIN)
#define MCUOUT11_LOW      GPIO_SetLow(MCUOUT11_PIN)

#define MCUOUT12_HIGH     GPIO_SetHigh(MCUOUT12_PIN)
#define MCUOUT12_LOW      GPIO_SetLow(MCUOUT12_PIN)

#define MCUOUT13_HIGH     GPIO_SetHigh(MCUOUT13_PIN)
#define MCUOUT13_LOW      GPIO_SetLow(MCUOUT13_PIN)

#define MCUOUT14_HIGH     GPIO_SetHigh(MCUOUT14_PIN)
#define MCUOUT14_LOW      GPIO_SetLow(MCUOUT14_PIN)

#define MCUOUT15_HIGH     GPIO_SetHigh(MCUOUT15_PIN)
#define MCUOUT15_LOW      GPIO_SetLow(MCUOUT15_PIN)

#define MCUOUT16_HIGH     GPIO_SetHigh(MCUOUT16_PIN)
#define MCUOUT16_LOW      GPIO_SetLow(MCUOUT16_PIN)

#define MCUIN1            GPIO_GetGpioStatus(MCUIN1_PIN)
#define MCUIN2            GPIO_GetGpioStatus(MCUIN2_PIN)
#define MCUIN3            GPIO_GetGpioStatus(MCUIN3_PIN)
#define MCUIN4            GPIO_GetGpioStatus(MCUIN4_PIN)
#define MCUIN5            GPIO_GetGpioStatus(MCUIN5_PIN)
#define MCUIN6            GPIO_GetGpioStatus(MCUIN6_PIN)
#define MCUIN7            GPIO_GetGpioStatus(MCUIN7_PIN)
#define MCUIN8            GPIO_GetGpioStatus(MCUIN8_PIN)
#define MCUIN9            GPIO_GetGpioStatus(MCUIN9_PIN)
#define MCUIN10           GPIO_GetGpioStatus(MCUIN10_PIN)
#define MCUIN11           GPIO_GetGpioStatus(MCUIN11_PIN)
#define MCUIN12           GPIO_GetGpioStatus(MCUIN12_PIN)
#define MCUIN13           GPIO_GetGpioStatus(MCUIN13_PIN)
#define MCUIN14           GPIO_GetGpioStatus(MCUIN14_PIN)
#define MCUIN15           GPIO_GetGpioStatus(MCUIN15_PIN)
#define MCUIN16           GPIO_GetGpioStatus(MCUIN16_PIN)




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
void vGPIO_OUT_Init(void);
void vGPIO_IN_Init(void);
/*---------------------------------- End of the file ---------------------------------*/
#endif /* __BSP_GPIO_H__ */

