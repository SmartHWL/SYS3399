/**
  ******************************************************************************
  * @file    BSP_adc.h
  * @author  Silan - MC Team
  * @version 1.0.0
  * @date    2017/07/19
  * @brief   ADC Related Config Header File
  * @details None
	* @note    None
  ******************************************************************************
  * @attention
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
  * AS A RESULT, SILAN MICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
  * CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
  * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
	*
  ******************************************************************************
	* Change History
	* -# 2017/07/19 | v1.0.0 | Wangjianlong | Creat file.
	*
	******************************************************************************
  */

/*******************************************************************************
 * Define to prevent recursive inclusion
 ******************************************************************************/
#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

/*******************************************************************************
 * Include section
 ******************************************************************************/
#include <stdint.h>
#include "SC32F58128_adc.h"
#include "IQmathLib.h"

#include "BSP_gpio.h"

/*******************************************************************************
 * Constants and macros
 ******************************************************************************/

/* ADC channel definitions */
#define  MAX_AD_CHNL      13



#define  ADCSOCTRIG       5 
#define  ADCINDLY         0 
#define  ADCACQPS         9

#define ANALOG_MCU_IN1_PIN        GPIO_80
#define ANALOG_MCU_IN2_PIN				GPIO_81
#define ANALOG_MCU_IN3_PIN				GPIO_82
#define ANALOG_MCU_IN4_PIN				GPIO_83

//      ADCµÄ¿Ú                              ADCË³Ðò
#define ADCSOC0_CHNL  	0  /*CHARGECURR  ADC->ADCRESULT4*/
#define ADCSOC1_CHNL  	1  /*DETSTOP1    ADC->ADCRESULT5*/
#define ADCSOC2_CHNL  	2	 /*DETSTOP2    ADC->ADCRESULT6*/
#define ADCSOC3_CHNL  	3  /*Rain 			 ADC->ADCRESULT7*/

#define ADCSOC4_CHNL  	4  /*RN ADC->ADCRESULT0*/        
#define ADCSOC5_CHNL  	5	 /*RP ADC->ADCRESULT1*/											
#define ADCSOC6_CHNL  	6	 /*LN ADC->ADCRESULT2*/												
#define ADCSOC7_CHNL  	7	 /*LP ADC->ADCRESULT3*/		
												
#define ADCSOC8_CHNL  	8/*CHARGEVOL   ADC->ADCRESULT8*/
#define ADCSOC9_CHNL  	9/*BUSVOL 		 ADC->ADCRESULT9*/
#define ADCSOC10_CHNL   10/*BATTERY 	 ADC->ADCRESULT10*/


/*******************************************************************************
 * Typedefs, structs and enums
 ******************************************************************************/
/**
  *@brief  ADC sample sequency struct range from high priority to low
  */


/**
  *@brief  ADC sample sequency union range from high priority to low
  */


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes of the functions
 ******************************************************************************/
extern void BSP_AdcInit(void);

/*---------------------------------- End of the file ---------------------------------*/
#endif /* __BSP_ADC_H__ */

