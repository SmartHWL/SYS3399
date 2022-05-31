/**
* @file          : devInit.c
* @author        : mcu Lab
* @date          : mm/dd/yyy
* @brief         : This file is for all kind of init funtion.
* @version       : Ver. 1.00

* H/W Platform       : SL_FOC FOR MOTOR CONCTROL

 *------------------------------------------------------------------------------

 * Compiler info     :keil v4

 *------------------------------------------------------------------------------

Note: In this software, the function is used in motor control.

*-------------------------------------------------------------------------------

*  History:

*              mm/dd/yyyy ver. x.y.z author

*              mm1/dd1/yyyy1 ver. x.y.z author1

*------------------------------------------------------------------------------
* @attention
 * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, SLMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.

* <h2><center>&copy; COPYRIGHT 2015 SLMicroelectronics</center></h2>
******************************************************************************
*/


/******************************************************************************
* include file
*******************************************************************************/
#include "stdint.h"
#include "system_SC32F58128.h"
#include "devInit.h"
#include "MultCtrl.h"
#include "DivCtrl.h"
#include "SC32F58128_SysClkCfg.h"
#include "SC32F58128_Timer.h"

#include "BSP_uart.h"
#include "BSP_gpio.h"
#include "BSP_dma.h"
#include "osc.h"
#include "BSP_adc.h"
#include "BSP_pwm.h"
#include "BSP_spi.h"
#include "BSP_qep.h"
/******************************************************************************
* global data for the project
*******************************************************************************/


/******************************************************************************
* loacal data for the current file
*******************************************************************************/


/******************************************************************************/
/* local prototypes function **************************************************/
/******************************************************************************/
void mg_DeviceSystemInit(void);
void mg_DevicePeripheralInit(void);
void mg_CacheInit(void);
void mg_WdtInit(void);
void mg_TimerInit(void);
void mg_EruInit(void);
void mg_SetIRQ(void);

/******************************************************************************
* loacal function for the current file
*******************************************************************************/

/**
  * @brief  Watch Dog Timer Initialization.
  */
void mg_WdtInit(void)
{
  WDT0->WDOGLOAD = SystemCoreClock / 5000; // 5kHz
  WDT0->WDOGCONTROL_b.RSTEN = 1;           // Reset Enable
  WDT0->WDOGCONTROL_b.INTEN = 1;           // Enable Interrupt and Start Counter
}


/**
  * @brief  Cache Initialization.
  */
void mg_CacheInit(void)
{
  CACHE->CACHE_CTRL_b.AUTOCLEAR = 1;                /**< Enable Cache Auto Clear Function  */
	while(CACHE->CACHE_CTRL_b.AUTOCLEAR_END == 0);
	CACHE->CACHE_CTRL_b.AUTOCLEAR_END = 1;

	CACHE->MAIN_CTRL_b.CACHE_EN = 1;                  /**< Enable Cache                      */
	CACHE->CACHE_CTRL_b.STATISTICS_EN = 1;            /**< Enable Cache Hit Count            */

  ACCESS_ENABLE;
  SYSREG->PREFETCH_CFG_b.PREFETCH_EN = 1;           /**< Prefetch Enable                   */
  SYSREG->PREFETCH_CFG_b.ONLY_CACHE = 1;            /**< Only Cache Enable                 */
  SYSREG->PREFETCH_CFG_b.PREFETCH_INFO_RST = 1;     /**< Reset Prefect Information         */
  SYSREG->PREFETCH_CFG_b.PREFETCH_INFO_RST = 0;
  ACCESS_DISABLE;
}


/**
  * @brief  Timer Initialization.
  */
void mg_TimerInit(void)
{
  TIMER_TmrConfig(0,50,1);     // 50uS
}

/**
  * @brief   irq set init.
  */
void mg_SetIRQ(void)
{
  __disable_irq();
//	NVIC_ClearPendingIRQ(SysTick_IRQn);
//  NVIC_SetPriority(SysTick_IRQn,0);
//	NVIC_EnableIRQ(SysTick_IRQn);

	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_SetPriority(UART0_IRQn,3);
	NVIC_EnableIRQ(UART0_IRQn);

	NVIC_ClearPendingIRQ(UART12_IRQn);
	NVIC_SetPriority(UART12_IRQn,3);
	NVIC_EnableIRQ(UART12_IRQn);
	
//  NVIC_ClearPendingIRQ(DMA_IRQn);
//  NVIC_SetPriority(DMA_IRQn,3);
//	NVIC_EnableIRQ(DMA_IRQn);'

	
	NVIC_ClearPendingIRQ(ADC1_IRQn);
	NVIC_EnableIRQ(ADC1_IRQn);
	NVIC_SetPriority(ADC1_IRQn,1);
		
  NVIC_ClearPendingIRQ(TIMER0_IRQn);
  NVIC_SetPriority(TIMER0_IRQn,2);
	NVIC_EnableIRQ(TIMER0_IRQn);
	
	 NVIC_ClearPendingIRQ(EQEP0_IRQn);
    NVIC_SetPriority(EQEP0_IRQn,0);
    NVIC_EnableIRQ(EQEP0_IRQn); 
	
	NVIC_ClearPendingIRQ(EQEP1_IRQn);
    NVIC_SetPriority(EQEP1_IRQn,0);
    NVIC_EnableIRQ(EQEP1_IRQn); 
  __enable_irq();
	
}


/**
  * @brief   irq set init for core1.
  */
void mg_SetIRQ_c1(void)
{
  __disable_irq();

  __enable_irq();
}
/**
  * @brief  Initialize System Clock, Systick, cache, WDT, etc.
  */
void mg_DeviceSystemInit(void)
{
	/* System Clock Init */
	SysClockConfig(SRCSEL_PLL,PLLSRC_RCH15M,CLKDIV_6,PLLMULT_32); // 15 * 32 / 6 = 80Mhz
//	SysClockConfig(SRCSEL_PLL,PLLSRC_OSC,CLKDIV_4,PLLMULT_24);	//12*24/4=72Mhz
	SysClkDivSel(CLKDIV_1,CLKDIV_1,CLKDIV_1,CLKDIV_1);

  /* System Core Clock Update */
	SystemCoreClockUpdate();

	/* SysTick Init */
//	SysTick_Config(SystemAhbClock/1000);

	/* Cache Init */
	mg_CacheInit();

	/* Coproc Init */
  MULT_INIT_MACRO()
  DIV_INIT_MACRO()

}

/**
  * @brief  Initialize Peripherals Related with Application.
  */
void mg_DevicePeripheralInit(void){
	
	/* GPIO Init */
	BSP_GpioInit();

	/* Timer Init */
	mg_TimerInit();
	
	/* SPI Init */
  BSP_SpiInit();
	
	/* DMA Init */
//	DMA_Init();

	/* UART Init */
	//BSP_UartInit();
	
	//BSP_PwmInit();
	
	//BSP_AdcInit();
	
	BSP_QepInit();
	
}


/******************************************************************************
* global function for the current project
*******************************************************************************/

/**
  * @brief     init all data for the function.
  * @param[in]  void : None.
  * @retval     void : None
  * @par        None
  * @par        Example:
  * This example shows how to call the function:
  * @code
  * INIT_DeviceInit();
  * @endcode
  * @Note:
*/
void INIT_DeviceInit(void)
{
  /* System Clock, Flash&SRAM, Mult&Div Init */
	mg_DeviceSystemInit();
	mg_DevicePeripheralInit();

//  /* Debug Tool Init */
//  WAVE_Init();
//  DBG_ParamInit();

  /* Watch Dog Timer Init */
//	mg_WdtInit();

  /* NVIC Init */
	mg_SetIRQ();
}


void INIT_DeviceInit_c1(void)
{
  /* MULT&DIV Init */
  MULT_INIT_MACRO()
  DIV_INIT_MACRO()

  /* NVIC Init */
  mg_SetIRQ_c1();
}
/*---------------------------------- End of the file ---------------------------------*/
