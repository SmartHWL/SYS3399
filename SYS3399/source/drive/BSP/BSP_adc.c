/**
  ******************************************************************************
  * @file    BSP_adc.c
  * @author  Silan - MC Team
  * @version 1.0.0
  * @date    2017/07/19
  * @brief   ADC related Configuration Source File
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
 * Include file
 ******************************************************************************/
#include <SC32F58128.h>
#include "BSP_adc.h"
#include "BSP_gpio.h"

#include "SC32F58128_adc.h"

/*******************************************************************************
 * Global data for the project
 ******************************************************************************/


/*******************************************************************************
 * Loacal data for the current file
 ******************************************************************************/

/*******************************************************************************
 * Local prototypes function
 ******************************************************************************/

/*******************************************************************************
 * Loacal function for the current file
 ******************************************************************************/

/*******************************************************************************
 * Global function for the current project
 ******************************************************************************/

/**
  * @brief  get sampled data.
  */

//#define ADC_LP			ADC->ADCRESULT3     
//#define ADC_LN			ADC->ADCRESULT2
//#define ADC_RP      ADC->ADCRESULT1
//#define ADC_RN      ADC->ADCRESULT0

void AD_PIN_SEL(){
	
  GPIO_ModeSel(ANALOG_MCU_IN1_PIN, GPIO_MODE_ANALOG); 
  GPIO_ModeSel(ANALOG_MCU_IN2_PIN, GPIO_MODE_ANALOG);
  GPIO_ModeSel(ANALOG_MCU_IN3_PIN, GPIO_MODE_ANALOG);
  GPIO_ModeSel(ANALOG_MCU_IN4_PIN, GPIO_MODE_ANALOG);
}

/**
  * @brief  ADC Initialization.
  */
void BSP_AdcInit(void)
{
  AD_PIN_SEL();
  AdcInit(ADC_33V,ADC_CLKDIV_3);    // ADC Voltage = 3.3V, Clock Division = 3
  ACCESS_ENABLE;
  ADC->ADCCTL2_b.dlyprediv = 1;     // Trigger delay clock prescale: SystemMtClock/2
//  ADC->INTSEL1N2 = 0x0025;          // 通道12转换完成产生中断ADCINT1
  ADC->INTSEL1N2_b.INT1E = 1;     /*Enable ADCINT0*/	
  ADC->INTSEL1N2_b.INT1CONT = 1;  /*Cont*/	
  ADC->INTSEL1N2_b.INT1SEL = 3; /* EOC0 is selected as trigger source*/

  ADC->ADCSAMPLEMODE = 0;           // 采样模式为0: 顺序采样(非同时采样)
  ADC->ADCCTL3_b.shift = 0;
  //********************************
  ADC->ADCSOC0CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC4_CHNL << 6 | ADCACQPS;//RN
  ADC->ADCSOC1CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC5_CHNL << 6 | ADCACQPS;//RP
  ADC->ADCSOC2CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC6_CHNL << 6 | ADCACQPS;//LN
  ADC->ADCSOC3CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC7_CHNL << 6 | ADCACQPS;//LP
	
  ADC->ADCSOC4CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC0_CHNL << 6 | ADCACQPS;
  ADC->ADCSOC5CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC1_CHNL << 6 | ADCACQPS; 
  ADC->ADCSOC6CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC2_CHNL << 6 | ADCACQPS;	
  ADC->ADCSOC7CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC3_CHNL << 6 | ADCACQPS;	
  ADC->ADCSOC8CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC8_CHNL << 6 | ADCACQPS; 
  ADC->ADCSOC9CTL  = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC9_CHNL << 6 | ADCACQPS;	
  ADC->ADCSOC10CTL = ADCINDLY << 16 | ADCSOCTRIG << 11 | ADCSOC10_CHNL << 6 | ADCACQPS;			
	
	
  ADC->ADCINTFLGCLR_b.ADCINT1 = 1;  // 清除ADC模块的中断标志.
  ACCESS_DISABLE;
	
	// 软件复位触发规避ADC BUG
  ACCESS_ENABLE;
//  ADC->ADCSOC15CTL = ADCINDLY << 16 | 0 << 11 | ADCSOC2_CHNL << 6 | ADCACQPS;
//  ADC->ADCSOCFRC1_b.SOC15 = 1;
  ACCESS_DISABLE;
}

/*---------------------------------- End of the file ---------------------------------*/
