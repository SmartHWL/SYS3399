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
  * @brief   GPIO or PWM init
  * @param   ch          ch1:MCUOUT1_PIN - MCUOUT4_PIN
  *                      ch2:MCUOUT5_PIN - MCUOUT6_PIN
	*                      ch3:MCUOUT7_PIN - MCUOUT8_PIN
  * @param   MODEL       1:GPIO
	*                      2:PWM
  * @param   u32Period   PWM frequency
  * @retval 
  */
uint8_t u8GPIO_PWM_Init(uint8_t ch,uint8_t MODEL,uint32_t u32Period){
	
	u32Period=80000000/2/u32Period;
	

	if(ch==1){
		if(MODEL==1){
			GPIO_OutputEnable(MCUOUT1_PIN);
			MCUOUT1_LOW;
	    GPIO_OutputEnable(MCUOUT2_PIN);
			MCUOUT2_LOW;
	    GPIO_OutputEnable(MCUOUT3_PIN);
			MCUOUT3_LOW;
	    GPIO_OutputEnable(MCUOUT4_PIN);
			MCUOUT4_LOW;
		}
		else if(MODEL==2){
			STOP_ALL_PWM;
			PWM_PIN_SEL(0);
			PWM_PIN_SEL(2);
			PWM_CFG(0, u32Period, PREDRIVER_MODE, DEAD_TIME_CLK);
			PWM0->CMPA = 100;
			PWM0->CMPB =0;
			PWM_CFG(2, u32Period, PREDRIVER_MODE, DEAD_TIME_CLK);
			PWM2->CMPA = 100;
			PWM2->CMPB =0;
			START_ALL_PWM;
		}
		else{
			return 0;
		}
	}
	else if(ch==2){
		if(MODEL==1){
			GPIO_OutputEnable(MCUOUT5_PIN);
			MCUOUT5_LOW;
	    GPIO_OutputEnable(MCUOUT6_PIN);
			MCUOUT6_LOW;
		}
		else if(MODEL==2){
			STOP_ALL_PWM;
			PWM_PIN_SEL(1);
			PWM_CFG(1, u32Period, PREDRIVER_MODE, DEAD_TIME_CLK);
			PWM1->CMPA = 100;
			PWM1->CMPB =0;
			START_ALL_PWM;
		}
		else{
			return 0;
		}
	}
	else if(ch==3){
		if(MODEL==1){
			GPIO_OutputEnable(MCUOUT7_PIN);
			MCUOUT7_LOW;
	    GPIO_OutputEnable(MCUOUT8_PIN);
			MCUOUT8_LOW;
		}
		else if(MODEL==2){
			STOP_ALL_PWM;
			PWM_PIN_SEL(3);
			PWM_CFG(3, u32Period, PREDRIVER_MODE, DEAD_TIME_CLK);
			PWM3->CMPA = 100;
			PWM3->CMPB =0;
			START_ALL_PWM;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
	
	return 1;
}

void vGPIO_OUT_Init(void){

	for(uint8_t i=0;i<4;i++){
		
		u8GPIO_PWM_Init(i,2,20000);
	}
	GPIO_OutputEnable(MCUOUT9_PIN);
	MCUOUT9_LOW;
	GPIO_OutputEnable(MCUOUT10_PIN);
	MCUOUT10_LOW;
	GPIO_OutputEnable(MCUOUT11_PIN);
	MCUOUT11_LOW;
	GPIO_OutputEnable(MCUOUT12_PIN);
	MCUOUT12_LOW;
	GPIO_OutputEnable(MCUOUT13_PIN);
	MCUOUT13_LOW;
	GPIO_OutputEnable(MCUOUT14_PIN);
	MCUOUT14_LOW;
	GPIO_OutputEnable(MCUOUT15_PIN);
	MCUOUT15_LOW;
	GPIO_OutputEnable(MCUOUT16_PIN);
	MCUOUT16_LOW;
	
}
void vGPIO_IN_Init(void){
	
	GPIO_OutputDisable(MCUIN1_PIN);
	GPIO_OutputDisable(MCUIN2_PIN);
	GPIO_OutputDisable(MCUIN3_PIN);
	GPIO_OutputDisable(MCUIN4_PIN);
	GPIO_OutputDisable(MCUIN5_PIN);
	GPIO_OutputDisable(MCUIN6_PIN);
	GPIO_OutputDisable(MCUIN7_PIN);
	GPIO_OutputDisable(MCUIN8_PIN);
	GPIO_OutputDisable(MCUIN9_PIN);
	GPIO_OutputDisable(MCUIN10_PIN);
	GPIO_OutputDisable(MCUIN11_PIN);
	GPIO_OutputDisable(MCUIN12_PIN);
	GPIO_OutputDisable(MCUIN13_PIN);
	GPIO_OutputDisable(MCUIN14_PIN);
	GPIO_OutputDisable(MCUIN15_PIN);
}

void BSP_GpioInit(void)
{
	vGPIO_OUT_Init();
	vGPIO_IN_Init();	
	
	

}


