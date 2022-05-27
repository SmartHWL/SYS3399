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
	  
    PWM[u8Ch]->TBPRD = u32Period;	                    /* ���������� */
    PWM[u8Ch]->TBPHS = 0;                               /* ��װ����λֵ */

    PWM[u8Ch]->TBCTL = 0;                               /* ��λ�Ĵ��� */
    PWM[u8Ch]->TBCTL_b.CTRMODE = 2;                     /* �������ĶԳ�ģʽ */
    PWM[u8Ch]->TBCTL_b.PHSEN = 0;                       /* ��λװ�ز�ʹ�� */
    PWM[u8Ch]->TBCTL_b.PRDLD = 0;                       /* ���´�ӳ��Ĵ���ȡֵ */
    PWM[u8Ch]->TBCTL_b.SYNCOSEL = 1;                    /* ͬ��ģʽѡ��TBCTR=0 */

	PWM[u8Ch]->CMPCTL_b.SHDWAMODE = 0;	                /* CMPAӰ�ӼĴ���ʹ�� */
	PWM[u8Ch]->CMPCTL_b.SHDWBMODE = 0;	                /* CMPBӰ�ӼĴ���ʹ�� */
	PWM[u8Ch]->CMPCTL_b.LOADAMODE = 0;                  /* CMPAֵ��CTR=0ʱװ�� */
	PWM[u8Ch]->CMPCTL_b.LOADBMODE = 0;                  /* CMPAֵ��CTR=0ʱװ�� */
    
    /*
           /\      /\
       ---/--\----/--\-- CMPA(CMPB)    
         /|  |\  /|  |\
        / |  | \/ |  | \
        __    ____    __
          |  |    |  |
          |__|    |__|
           
    */
    PWM[u8Ch]->AQCTLA_b.CAU = 1; 	                     /* CMPA��������EPWMAΪ�� */    
	PWM[u8Ch]->AQCTLA_b.CAD = 2;	                     /* CMPA�ݼ�����EPWMAΪ�� */
	PWM[u8Ch]->AQCTLB_b.CAU = 1; 	                     /* CMPB��������EPWMAΪ�� */
	PWM[u8Ch]->AQCTLB_b.CAD = 2;	                     /* CMPB�ݼ�����EPWMAΪ�� */     
    PWM[u8Ch]->AQSFRC_b.RLDCSF = 3;
    
    PWM[u8Ch]->DBCTL_b.IN_MODE = 2;                      /* 10��EPWMxAIn�������ж��޶��������������ӳ��źŵ�����Դ�� 
                                                            EPWMxBIn�������ж��޶������½������ӳ��źŵ�����Դ */
    PWM[u8Ch]->DBCTL_b.OUT_MODE = 3;                     /* enable Dead-band module OUT_MODE */
    
    if(u8Polsel == UP_H_DOWN_H)
    {
       PWM[u8Ch]->DBCTL_b.POLSEL = 2;                         /* EPWMxA �� EPWMxB �������ѡ�� 2������ Aͬ��  B���� */
    }
    else if(u8Polsel == UP_L_DOWN_H)
    {
        PWM[u8Ch]->DBCTL_b.POLSEL = 3;                        /* EPWMxA �� EPWMxB �������ѡ�� 3��     A����  B���� */
    }
    else if(u8Polsel == UP_L_DOWN_L)
    {
        PWM[u8Ch]->DBCTL_b.POLSEL = 1;                       /* EPWMxA �� EPWMxB �������ѡ�� 3��     A����  Bͬ��  */
    }
    else if(u8Polsel == UP_H_DOWN_L)
    {
        PWM[u8Ch]->DBCTL_b.POLSEL = 0;                       /* EPWMxA �� EPWMxB �������ѡ�� 3��     Aͬ��  Bͬ��  */
    }

    PWM[u8Ch]->DBFED = u16DeadTime;                     /* �½������� */
    PWM[u8Ch]->DBRED = u16DeadTime;                     /* ���������� */
}

void PWM_Duty_CFG(uint16_t a,uint16_t b,uint16_t c)
{
		PWM0->CMPA = a;       
		PWM1->CMPA = b;
		PWM2->CMPA = c;
}

void PWM_INT_CFG()
{
		PWM0->ETPS_b.INTPRD = 1; //ePWM  1 ���жϴ����ж���Ӧ

		PWM0->ETSEL_b.INTESEL = 2;  //2����ʱ���׼�������������ڣ�TBCTR=TBPRD��ʱ�����ж�
		
//		PWM0->ETSEL_b.INTEN = 1;    //�����ж�
	
//PWM ���� ADC
	
/*	
SOCBSEL ����
EPWMxSOCA ѡ��  ����EPWMxSOCA �����ʱ��������
001��ʹ���¼�����ʱ���׼����������0��TBCTR=0x0000��
010��ʹ���¼�����ʱ���׼�������������ڣ�TBCTR=TBPRD��
011������
100��ʹ���¼�������ʱ������ʱʱ���׼����������CMPA
101��ʹ���¼�������ʱ���ݼ�ʱʱ���׼����������CMPA
110��ʹ���¼�������ʱ������ʱʱ���׼����������CMPB
111��ʹ���¼�������ʱ���ݼ�ʱʱ���׼����������VMPB	
*/

		PWM0->ETPS_b.SOCAPRD = 1;    //��һ���¼�����SOCA����
		PWM0->ETSEL_b.SOCASEL = 2;   //101��ʹ���¼�������ʱ���ݼ�ʱʱ���׼����������CMPA   ����SOCA
		PWM0->ETSEL_b.SOCAEN = 1;    //ʹ��EPWMxSOCA
		#if 0
		PWM0->ETPS_b.SOCBPRD = 1;    //��һ���¼�����SOCB����
		PWM0->ETSEL_b.SOCBSEL = 1;   //001��ʹ���¼�����ʱ���׼����������0��TBCTR=0��    ����SOCB
		PWM0->ETSEL_b.SOCBEN = 1;    //ʹ��EPWMxSOCB
		
		PWM1->ETPS_b.SOCAPRD = 1;    //��һ���¼�����SOCA����
		PWM1->ETSEL_b.SOCASEL = 5;   //101��ʹ���¼�������ʱ���ݼ�ʱʱ���׼����������CMPA   ����SOCA
		PWM1->ETSEL_b.SOCAEN = 1;    //ʹ��EPWMxSOCA

		PWM2->ETPS_b.SOCAPRD = 1;    //��һ���¼�����SOCA����
		PWM2->ETSEL_b.SOCASEL = 5;   //101��ʹ���¼�������ʱ���ݼ�ʱʱ���׼����������CMPA   ����SOCA
		PWM2->ETSEL_b.SOCAEN = 1;    //ʹ��EPWMxSOCA
        #endif
}

void PWM_PROTECT_CFG(void)
{
//TZ0 ��TZ2 ����GPIO ���룬 TZ4 ���ӵ�ϵͳʱ�ӹ����߼���TZ5 ����CPU ��EMUSTOP ����ź�	
	#if defined (HARDWARE_ZL) || defined (HARDWARE_80V_500A)
    GPIO_MuxSel(GPIO_12, 1);		//PA12 TZ0 ���Ÿ���
    #endif
    
	ACCESS_ENABLE       /* 32�������Զ����� Ҫע���Ƿ���ȷд�� */
 
    #if DEBUG_MODE
	#if defined(HARDWARE_80V_500A) || defined (HARDWARE_ZL)	
    //TZ0������������(�ⲿ����)
   PWM0->TZSEL_b.OSHT0 = 1;				
   PWM1->TZSEL_b.OSHT0 = 1;				
   PWM2->TZSEL_b.OSHT0 = 1;    
    #endif
    #endif
		//ACCESS_ENABLE                 //TZ3������������(�ڲ��Ƚ���)
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
		PWM0->TZCTL_b.TZA = 2;  					//10����EPWMxA �õ�
		//ACCESS_ENABLE
		PWM0->TZCTL_b.TZB = 2;  					//10����EPWMxB �õ�
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZA = 2;  					//10����EPWMxA �õ�
		//ACCESS_ENABLE
		PWM1->TZCTL_b.TZB = 2;  					//10����EPWMxB �õ�
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZA = 2;  					//10����EPWMxA �õ�
		//ACCESS_ENABLE
		PWM2->TZCTL_b.TZB = 2;  					//10����EPWMxB �õ�
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
    PWM[u8Ch]->TBPRD = u32Period;	                    /* ���������� */
    PWM[u8Ch]->TBPHS = 0;                               /* ��װ����λֵ */

    PWM[u8Ch]->TBCTL = 0;                               /* ��λ�Ĵ��� */
    PWM[u8Ch]->TBCTL_b.CTRMODE = 2;                     /* �������ĶԳ�ģʽ */
    PWM[u8Ch]->TBCTL_b.PHSEN = 0;                       /* ��λװ�ز�ʹ�� */
    PWM[u8Ch]->TBCTL_b.PRDLD = 0;                       /* ���´�ӳ��Ĵ���ȡֵ */
    PWM[u8Ch]->TBCTL_b.SYNCOSEL = 1;                    /* ͬ��ģʽѡ��TBCTR=0 */


    
	                     
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
        PWM[u8Ch]->CMPCTL_b.SHDWAMODE = 0;	                /* CMPAӰ�ӼĴ���ʹ�� */
        PWM[u8Ch]->CMPCTL_b.LOADAMODE = 0;                  /* CMPAֵ��CTR=0ʱװ�� */
       // PWM[u8Ch]->AQCTLA_b.CAU = 2; 	                     /* CMPA��������EPWMAΪ�� */    
       // PWM[u8Ch]->AQCTLA_b.CAD = 1;	                     /* CMPA�ݼ�����EPWMAΪ�� */
       // PWM[u8Ch]->CMPA = u32Period >> 1;
        PWM[u8Ch]->AQCTLA_b.PRU = 1;
        PWM[u8Ch]->AQCTLA_b.ZRO = 2;
    }
    else
    {
        PWM[u8Ch]->CMPCTL_b.SHDWBMODE = 0;	                /* CMPBӰ�ӼĴ���ʹ�� */
        PWM[u8Ch]->CMPCTL_b.LOADBMODE = 0;                  /* CMPBֵ��CTR=0ʱװ�� */
        PWM[u8Ch]->AQCTLB_b.PRU = 1;
        PWM[u8Ch]->AQCTLB_b.ZRO = 2;
    }
    
    PWM[u8Ch]->AQSFRC_b.RLDCSF = 3;
    
    PWM[u8Ch]->DBCTL_b.IN_MODE = 2;                      /* 10��EPWMxAIn�������ж��޶��������������ӳ��źŵ�����Դ�� 
                                                            EPWMxBIn�������ж��޶������½������ӳ��źŵ�����Դ */
    PWM[u8Ch]->DBCTL_b.OUT_MODE = 0;                     /* enable Dead-band module OUT_MODE */
    

    PWM[u8Ch]->DBCTL_b.POLSEL = 0;                       /* EPWMxA �� EPWMxB �������ѡ�� 3��     Aͬ��  Bͬ��  */
 

    PWM[u8Ch]->DBFED = 0;                     /* �½������� */
    PWM[u8Ch]->DBRED = 0;                     /* ���������� */
}

void PWM_START(void)
{
    ACCESS_ENABLE
    SYSREG->PWM_CFG |= 0x000000ff;  //PWM0,1,2   0��ѡ��pwm ���ֹͣ��pwm tz0-4�����κ�һ��tz ����ʱ��tz5 �������   00b��ѡ��cp0 ���
    ACCESS_DISABLE;
}
void motor_stop(void)
{ 
    __disable_irq();
    
    /* ǿ���������񻹾����� �Ĵ��� */
    #if(PREDRIVER_MODE == UP_H_DOWN_H)
    PWM0->AQCSFRC_b.CSFA = 1;//1001
    PWM0->AQCSFRC_b.CSFB = 2;
    
    PWM1->AQCSFRC_b.CSFA = 1;//0101
    PWM1->AQCSFRC_b.CSFB = 2;
    
    PWM2->AQCSFRC_b.CSFA = 1;//0101
    PWM2->AQCSFRC_b.CSFB = 2;	
    #elif (PREDRIVER_MODE == UP_H_DOWN_L)
    δ����
    PWM0->AQCSFRC = 9;
    PWM1->AQCSFRC = 9;//0101
    PWM2->AQCSFRC = 9;//0101
    #elif (PREDRIVER_MODE == UP_L_DOWN_L)
    δ����
    #elif (PREDRIVER_MODE == UP_L_DOWN_H)
    δ����
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
    PWM_INT_CFG();        //PWM�ж� �� SOC����
    //PWM_PROTECT_CFG();    /* TZ����������ʱ�ĵ�ƽ����  ��Ҫ��ĸ���ȶ��ڽ������� */
    PWM_START();   
    
}

/*---------------------------------- End of the file ---------------------------------*/
