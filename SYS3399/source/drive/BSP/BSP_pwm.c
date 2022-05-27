/**
  ******************************************************************************
  * @file    BSP_pwm.c
  * @author  
  * @version 1.0.0
  * @date    
  * @brief   PWM Related Config Source File
  * @details None
	* @note    None
  ******************************************************************************
  */

/*******************************************************************************
 * Include file
 ******************************************************************************/
#include <SC32F58128.h>
#include "BSP_pwm.h"
#include "BSP_gpio.h"
/*******************************************************************************
 * Global data for the project
 ******************************************************************************/


/*******************************************************************************
 * Local data for the current file
 ******************************************************************************/

/*******************************************************************************
 * Local prototypes function
 ******************************************************************************/

/*******************************************************************************
 * Local function for the current file
 ******************************************************************************/

/*******************************************************************************
 * Global function for the project
 ******************************************************************************/

#define UP_H_DOWN_H 					(1)   								
#define UP_L_DOWN_H 					(2)   								 
#define UP_L_DOWN_L 					(3)   								
#define UP_H_DOWN_L 					(4)   							
#define PREDRIVER_MODE 				(UP_H_DOWN_H) 

void PWM_PIN_SEL(uint8_t ch)
{
	switch(ch){
		case PWM_CH0:{
			GPIO_MuxSel(GPIO_0, 1);
      GPIO_MuxSel(GPIO_1, 1);
		}break;
		case PWM_CH1:{
			GPIO_MuxSel(GPIO_2, 1);
      GPIO_MuxSel(GPIO_3, 1);
		}break;
		case PWM_CH2:{
			GPIO_MuxSel(GPIO_4, 1);
      GPIO_MuxSel(GPIO_5, 1);
		}break;
		case PWM_CH3:{
			GPIO_MuxSel(GPIO_6, 1);
      GPIO_MuxSel(GPIO_7, 1);
		}break;
		case PWM_CH4:{
			GPIO_MuxSel(GPIO_8, 1);
      GPIO_MuxSel(GPIO_9, 1);
		}break;
		case PWM_CH5:{
			GPIO_MuxSel(GPIO_10, 1);
      GPIO_MuxSel(GPIO_11, 1);
		}break;
		default:
			break;
	}
//    GPIO_MuxSel(GPIO_0, 1); /* PWM0A */
//    GPIO_MuxSel(GPIO_1, 1); /* PWM0B */
//    GPIO_MuxSel(GPIO_2, 1); /* PWM1A */
//    GPIO_MuxSel(GPIO_3, 1); /* PWM1B */
//    GPIO_MuxSel(GPIO_4, 1); /* PWM2A */
//    GPIO_MuxSel(GPIO_5, 1); /* PWM2B */
    
    #ifdef GPIO_RESOLVE_INCENTIVE
    GPIO_MuxSel(GPIO_RESOLVE_INCENTIVE, 1); /* PWM3A */
    #endif
}

void PWM_CFG(uint8_t u8Ch, uint32_t u32Period, uint8_t u8Polsel, uint16_t u16DeadTime)
{
	  
    PWM[u8Ch]->TBPRD = u32Period;	                    /* 计数器周期 */
    PWM[u8Ch]->TBPHS = 0;                               /* 不装载相位值 */

    PWM[u8Ch]->TBCTL = 0;                               /* 复位寄存器 */
    PWM[u8Ch]->TBCTL_b.CTRMODE = 2;                     /* 增减中心对称模式 */
    PWM[u8Ch]->TBCTL_b.PHSEN = 0;                       /* 相位装载不使能 */
    PWM[u8Ch]->TBCTL_b.PRDLD = 0;                       /* 更新从映射寄存器取值 */
    PWM[u8Ch]->TBCTL_b.SYNCOSEL = 1;                    /* 同步模式选择：TBCTR=0 */

	PWM[u8Ch]->CMPCTL_b.SHDWAMODE = 0;	                /* CMPA影子寄存器使能 */
	PWM[u8Ch]->CMPCTL_b.SHDWBMODE = 0;	                /* CMPB影子寄存器使能 */
	PWM[u8Ch]->CMPCTL_b.LOADAMODE = 0;                  /* CMPA值在CTR=0时装载 */
	PWM[u8Ch]->CMPCTL_b.LOADBMODE = 0;                  /* CMPA值在CTR=0时装载 */
    
    /*
           /\      /\
       ---/--\----/--\-- CMPA(CMPB)    
         /|  |\  /|  |\
        / |  | \/ |  | \
        __    ____    __
          |  |    |  |
          |__|    |__|
           
    */
    PWM[u8Ch]->AQCTLA_b.CAU = 1; 	                     /* CMPA递增计数EPWMA为低 */    
	PWM[u8Ch]->AQCTLA_b.CAD = 2;	                     /* CMPA递减计数EPWMA为高 */
	PWM[u8Ch]->AQCTLB_b.CAU = 1; 	                     /* CMPB递增计数EPWMA为低 */
	PWM[u8Ch]->AQCTLB_b.CAD = 2;	                     /* CMPB递减计数EPWMA为高 */     
    PWM[u8Ch]->AQSFRC_b.RLDCSF = 3;
    
    PWM[u8Ch]->DBCTL_b.IN_MODE = 2;                      /* 10：EPWMxAIn（来自行动限定）是上升边沿延迟信号的输入源。 
                                                            EPWMxBIn（来自行动限定）是下降边沿延迟信号的输入源 */
    PWM[u8Ch]->DBCTL_b.OUT_MODE = 3;                     /* enable Dead-band module OUT_MODE */
    
    if(u8Polsel == UP_H_DOWN_H)
    {
       PWM[u8Ch]->DBCTL_b.POLSEL = 2;                         /* EPWMxA 和 EPWMxB 输出极性选择： 2：互补 A同向  B反向 */
    }
    else if(u8Polsel == UP_L_DOWN_H)
    {
        PWM[u8Ch]->DBCTL_b.POLSEL = 3;                        /* EPWMxA 和 EPWMxB 输出极性选择： 3：     A反向  B反向 */
    }
    else if(u8Polsel == UP_L_DOWN_L)
    {
        PWM[u8Ch]->DBCTL_b.POLSEL = 1;                       /* EPWMxA 和 EPWMxB 输出极性选择： 3：     A反向  B同向  */
    }
    else if(u8Polsel == UP_H_DOWN_L)
    {
        PWM[u8Ch]->DBCTL_b.POLSEL = 0;                       /* EPWMxA 和 EPWMxB 输出极性选择： 3：     A同向  B同向  */
    }

    PWM[u8Ch]->DBFED = u16DeadTime;                     /* 下降沿死区 */
    PWM[u8Ch]->DBRED = u16DeadTime;                     /* 上升沿死区 */
}

void PWM_Duty_CFG(uint16_t a,uint16_t b,uint16_t c)
{
		PWM0->CMPA = a;       
		PWM1->CMPA = b;
		PWM2->CMPA = c;
}

void PWM_INT_CFG()
{
		PWM0->ETPS_b.INTPRD = 1; //ePWM  1 个中断触发中断响应

		PWM0->ETSEL_b.INTESEL = 2;  //2：当时间基准计数器等于周期（TBCTR=TBPRD）时产生中断
		
//		PWM0->ETSEL_b.INTEN = 1;    //允许中断
	
//PWM 触发 ADC
	
/*	
SOCBSEL 定义
EPWMxSOCA 选择  决定EPWMxSOCA 脉冲何时将被生成
001：使能事件，当时间基准计数器等于0（TBCTR=0x0000）
010：使能事件，当时间基准计数器等于周期（TBCTR=TBPRD）
011：保留
100：使能事件，当定时器递增时时间基准计数器等于CMPA
101：使能事件，当定时器递减时时间基准计数器等于CMPA
110：使能事件，当定时器递增时时间基准计数器等于CMPB
111：使能事件，当定时器递减时时间基准计数器等于VMPB	
*/

		PWM0->ETPS_b.SOCAPRD = 1;    //第一个事件生成SOCA脉冲
		PWM0->ETSEL_b.SOCASEL = 2;   //101：使能事件，当定时器递减时时间基准计数器等于CMPA   生成SOCA
		PWM0->ETSEL_b.SOCAEN = 1;    //使能EPWMxSOCA
		#if 0
		PWM0->ETPS_b.SOCBPRD = 1;    //第一个事件生成SOCB脉冲
		PWM0->ETSEL_b.SOCBSEL = 1;   //001：使能事件，当时间基准计数器等于0（TBCTR=0）    生成SOCB
		PWM0->ETSEL_b.SOCBEN = 1;    //使能EPWMxSOCB
		
		PWM1->ETPS_b.SOCAPRD = 1;    //第一个事件生成SOCA脉冲
		PWM1->ETSEL_b.SOCASEL = 5;   //101：使能事件，当定时器递减时时间基准计数器等于CMPA   生成SOCA
		PWM1->ETSEL_b.SOCAEN = 1;    //使能EPWMxSOCA

		PWM2->ETPS_b.SOCAPRD = 1;    //第一个事件生成SOCA脉冲
		PWM2->ETSEL_b.SOCASEL = 5;   //101：使能事件，当定时器递减时时间基准计数器等于CMPA   生成SOCA
		PWM2->ETSEL_b.SOCAEN = 1;    //使能EPWMxSOCA
        #endif
}

void PWM_PROTECT_CFG(void)
{
//TZ0 到TZ2 来自GPIO 输入， TZ4 连接到系统时钟故障逻辑，TZ5 来自CPU 的EMUSTOP 输出信号	
	#if defined (HARDWARE_ZL) || defined (HARDWARE_80V_500A)
    GPIO_MuxSel(GPIO_12, 1);		//PA12 TZ0 引脚复用
    #endif
    
	ACCESS_ENABLE       /* 32个周期自动清零 要注意是否正确写入 */
 
    #if DEBUG_MODE
	#if defined(HARDWARE_80V_500A) || defined (HARDWARE_ZL)	
    //TZ0触发过流保护(外部引脚)
   PWM0->TZSEL_b.OSHT0 = 1;				
   PWM1->TZSEL_b.OSHT0 = 1;				
   PWM2->TZSEL_b.OSHT0 = 1;    
    #endif
    #endif
		//ACCESS_ENABLE                 //TZ3触发过流保护(内部比较器)
		//PWM0->TZSEL_b.OSHT3 = 1;				
		//ACCESS_ENABLE
		//PWM1->TZSEL_b.OSHT3 = 1;				
		//ACCESS_ENABLE
		//PWM2->TZSEL_b.OSHT3 = 1; 		
		
//		ACCESS_ENABLE               //TZ4 
//		PWM0->TZSEL_b.OSHT4 = 1;				
//		ACCESS_ENABLE
//		PWM1->TZSEL_b.OSHT4 = 1;				
//		ACCESS_ENABLE
//		PWM2->TZSEL_b.OSHT4 = 1; 	
			
//		ACCESS_ENABLE               //TZ5 
//		PWM0->TZSEL_b.OSHT5 = 1;				
//		ACCESS_ENABLE
//		PWM1->TZSEL_b.OSHT5 = 1;				
//		ACCESS_ENABLE
//		PWM2->TZSEL_b.OSHT5 = 1;           
			
		
		#if (PREDRIVER_MODE == UP_H_DOWN_H)
		//ACCESS_ENABLE
		PWM0->TZCTL_b.TZA = 2;  					//10：将EPWMxA 置低
		//ACCESS_ENABLE
		PWM0->TZCTL_b.TZB = 2;  					//10：将EPWMxB 置低
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZA = 2;  					//10：将EPWMxA 置低
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZB = 2;  					//10：将EPWMxB 置低
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZA = 2;  					//10：将EPWMxA 置低
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZB = 2;  					//10：将EPWMxB 置低
		#endif
		
		#if (PREDRIVER_MODE == UP_L_DOWN_H)
		//ACCESS_ENABLE
		PWM0->TZCTL_b.TZA = 1;  					
		//ACCESS_ENABLE
		PWM0->TZCTL_b.TZB = 2;  					
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZA = 1;  					
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZB = 2;  					
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZA = 1;  					
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZB = 2;  					
		#endif
		
		#if (PREDRIVER_MODE == UP_L_DOWN_L)
		//ACCESS_ENABLE
		PWM0->TZCTL_b.TZA = 1;  					
		//ACCESS_ENABLE
		PWM0->TZCTL_b.TZB = 1;  					
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZA = 1;  					
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZB = 1;  					
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZA = 1;  					
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZB = 1;  					
		#endif
		
		#if (PREDRIVER_MODE == UP_H_DOWN_L)
		//ACCESS_ENABLE
		PWM0->TZCTL_b.TZA = 2;  					
		//ACCESS_ENABLE
		PWM0->TZCTL_b.TZB = 1;  					
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZA = 2;  					
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZB = 1;  					
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZA = 2;  					
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZB = 1;  					
		#endif
        
        PWM0->TZEINT_b.OST = 1;

	    ACCESS_DISABLE;

}

void PWM_ResolveIncentiveInit(uint32_t u32Period)
{
    uint8_t u8Ch;
    uint8_t u8ChAB;
    #ifdef GPIO_RESOLVE_INCENTIVE
    if(GPIO_RESOLVE_INCENTIVE == GPIO_6)
    {
        u8Ch = 3;
        u8ChAB = 0;
    }
    else
    {
    }
    #endif
    PWM[u8Ch]->TBPRD = u32Period;	                    /* 计数器周期 */
    PWM[u8Ch]->TBPHS = 0;                               /* 不装载相位值 */

    PWM[u8Ch]->TBCTL = 0;                               /* 复位寄存器 */
    PWM[u8Ch]->TBCTL_b.CTRMODE = 2;                     /* 增减中心对称模式 */
    PWM[u8Ch]->TBCTL_b.PHSEN = 0;                       /* 相位装载不使能 */
    PWM[u8Ch]->TBCTL_b.PRDLD = 0;                       /* 更新从映射寄存器取值 */
    PWM[u8Ch]->TBCTL_b.SYNCOSEL = 1;                    /* 同步模式选择：TBCTR=0 */


    
	                     
    /*
           /\      /\
       ---/--\----/--\-- CMPA(CMPB)    
         /|  |\  /|  |\
        / |  | \/ |  | \
        __    ____    __
          |  |    |  |
          |__|    |__|
           
    */
    if(u8ChAB == 0)
    {
        PWM[u8Ch]->CMPCTL_b.SHDWAMODE = 0;	                /* CMPA影子寄存器使能 */
        PWM[u8Ch]->CMPCTL_b.LOADAMODE = 0;                  /* CMPA值在CTR=0时装载 */
       // PWM[u8Ch]->AQCTLA_b.CAU = 2; 	                     /* CMPA递增计数EPWMA为低 */    
       // PWM[u8Ch]->AQCTLA_b.CAD = 1;	                     /* CMPA递减计数EPWMA为高 */
       // PWM[u8Ch]->CMPA = u32Period >> 1;
        PWM[u8Ch]->AQCTLA_b.PRU = 1;
        PWM[u8Ch]->AQCTLA_b.ZRO = 2;
    }
    else
    {
        PWM[u8Ch]->CMPCTL_b.SHDWBMODE = 0;	                /* CMPB影子寄存器使能 */
        PWM[u8Ch]->CMPCTL_b.LOADBMODE = 0;                  /* CMPB值在CTR=0时装载 */
        PWM[u8Ch]->AQCTLB_b.PRU = 1;
        PWM[u8Ch]->AQCTLB_b.ZRO = 2;
    }
    
    PWM[u8Ch]->AQSFRC_b.RLDCSF = 3;
    
    PWM[u8Ch]->DBCTL_b.IN_MODE = 2;                      /* 10：EPWMxAIn（来自行动限定）是上升边沿延迟信号的输入源。 
                                                            EPWMxBIn（来自行动限定）是下降边沿延迟信号的输入源 */
    PWM[u8Ch]->DBCTL_b.OUT_MODE = 0;                     /* enable Dead-band module OUT_MODE */
    

    PWM[u8Ch]->DBCTL_b.POLSEL = 0;                       /* EPWMxA 和 EPWMxB 输出极性选择： 3：     A同向  B同向  */
 

    PWM[u8Ch]->DBFED = 0;                     /* 下降沿死区 */
    PWM[u8Ch]->DBRED = 0;                     /* 上升沿死区 */
}

void PWM_START(void)
{
    ACCESS_ENABLE
    SYSREG->PWM_CFG |= 0x000000ff;  //PWM0,1,2   0：选择pwm 软件停止和pwm tz0-4，当任何一个tz 发生时，tz5 都会产生   00b：选择cp0 输出
    ACCESS_DISABLE;
}
void motor_stop(void)
{ 
    __disable_irq();
    
    /* 强制输出后好像还经过了 寄存器 */
    #if(PREDRIVER_MODE == UP_H_DOWN_H)
    PWM0->AQCSFRC_b.CSFA = 1;//1001
    PWM0->AQCSFRC_b.CSFB = 2;
    
    PWM1->AQCSFRC_b.CSFA = 1;//0101
    PWM1->AQCSFRC_b.CSFB = 2;
    
    PWM2->AQCSFRC_b.CSFA = 1;//0101
    PWM2->AQCSFRC_b.CSFB = 2;	
    #elif (PREDRIVER_MODE == UP_H_DOWN_L)
    未测试
    PWM0->AQCSFRC = 9;
    PWM1->AQCSFRC = 9;//0101
    PWM2->AQCSFRC = 9;//0101
    #elif (PREDRIVER_MODE == UP_L_DOWN_L)
    未测试
    #elif (PREDRIVER_MODE == UP_L_DOWN_H)
    未测试
	#endif
            
    PWM0->CMPA = 0;
    PWM1->CMPA = 0;
    PWM2->CMPA = 0;
	__enable_irq();
}

void motor_start(void)
{ 
    __disable_irq();

    PWM0->AQCSFRC = 0;
    PWM1->AQCSFRC = 0;//0101
    PWM2->AQCSFRC = 0;//0101
    
	__enable_irq();
}

void TZ_clear(void)
{
	__disable_irq();
	SYSREG->ACCESS_EN = 0x05fa659a;
	SYSREG->PWM_CFG  &= ~(1<<23);	
	
	SYSREG->ACCESS_EN = 0x05fa659a;
	PWM0->TZCLR |= 0x07;
	SYSREG->ACCESS_EN = 0x05fa659a;
	PWM1->TZCLR |= 0x07;
	SYSREG->ACCESS_EN = 0x05fa659a;
	PWM2->TZCLR |= 0x07;
	
	__enable_irq();
}
/**
  * @brief  PWM Initialization.
  */
void BSP_PwmInit(void)
{
    uint8_t i;
    
    for(i = 0; i < 3; i++)
    {
			PWM_PIN_SEL(i);
       PWM_CFG(i, PWM_PERIOD, PREDRIVER_MODE, DEAD_TIME_CLK); 
    }

    PWM_Duty_CFG(100,100,100);  //Set Duty
    PWM_INT_CFG();        //PWM中断 和 SOC配置
    //PWM_PROTECT_CFG();    /* TZ保护，保护时的电平设置  需要等母线稳定在进行设置 */
    PWM_START();   
    
}

/*---------------------------------- End of the file ---------------------------------*/
