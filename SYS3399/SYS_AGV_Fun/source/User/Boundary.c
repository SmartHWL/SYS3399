/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName:   Boundary.c
 Author:     Version :   Date: 
 xyb         1.0         2022��1��11��
 Description: ������Ϣ��ȡ
 Version: �汾��Ϣ
 History: 
 <author>    <time>          <version >   <desc> 
 xyb        2022��1��11��       1.0        ����
***********************************************************/
#include "SC32F58128.h"
#include "sl_niv.h"
#include "SC32F58128_timer.h"
#include "BSP_gpio.h"
#include "BSP_uart.h"
#include "UartToMotor.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Boundary.h"
#include "Control&Filter.h"
#include "Control&Filter.h"
#include "UserConfig.h"



 
 
#define  COUNT5MS 			 (5000/50)
#define  COUNT50MS       (50000/50)
#define  ADCDIFFERENCE   (1000)


/* �ṹ��ָ������ */
BOUNDARY_STR  Boundary;  
/* �������� */
void Boundary_Init(BOUNDARY_STR *pstr );
void Boundary_Deal(BOUNDARY_SUB_STR *pstr);
void Boundary_ADCIsr(BOUNDARY_STR *pstr );	
/* ����ָ������ */
BOUNDARY_FUN_STR strBoundaryFun = 
{
	&Boundary_Init,
	&Boundary_Deal,	
	&Boundary_ADCIsr,

};


void Boundary_Init(BOUNDARY_STR *pstr )
{
	

}
/**
* @brief  None
* @param  None
* @retval None

14ms���������壬����150us��ADC 50us����һ��

					________
				 |				|
P				 |				|
_________|        |					 _________ 
									|					|
									|					|
									|_________| 
									 _________ 
									|					|
N									|					|
________				  |					|________
				|				  |
				|				  |					
				|_________| 

�ڱ߽���Ȧ�ڵ�ͼʾ

*/
void Boundary_Deal(BOUNDARY_SUB_STR *pstr )
{
     uint8_t  lu8FilterCount;
	
	  if(abs((int)(pstr->In.Postive-pstr->In.Negative))>1200)/*ADC��ֵ����һ��ֵ�ж�Ϊ�б߽��ź�*/
	  { 							   							
			if(pstr->Para.u8FirstFlag==0)
			{
					
				pstr->Para.u8FirstFlag=1;/*��һ�ν��뵽��ֵ����,���Ҽ�¼��־λ*/
			  pstr->Para.u32Count5ms=0; 		/*5ms���н��յ�*/			
				pstr->Para.u32Count50ms=0;    /*50ms���н��յ�*/
				
				if(pstr->In.Postive>pstr->In.Negative)
				{
					pstr->Out.u8statuesCurrent=DETECTORIN;/*�ȸߺ��*/
				}					
				else 									
				{
					pstr->Out.u8statuesCurrent=DETECTOROUT;/*�ȵͺ��*/
				}		
			 }	
			
			 
		 
			 /*�������˲�*/		
			 /*14msʱ�������㣬�����Ǹ���*/		
			 /*��һ��֮��Ҫȷ���ڶ��κܿ����������ʱ�������Ǳ�ĸ���	 			 
			 */					 			 			 			 
			 		 
			 
			 /*���źŽ����˲�*/		
			 if(pstr->Out.u8statues!=pstr->Out.u8statuesCurrent)
			 {				 				 
				 lu8FilterCount++;
					if(lu8FilterCount>20)
					{
						lu8FilterCount=0;
						pstr->Out.u8statues=pstr->Out.u8statuesCurrent;								
					}	
			 }
			 else
			 {
						if(lu8FilterCount>3) lu8FilterCount=lu8FilterCount-3;
						else lu8FilterCount=0;			
			 }
			 	 
	   }
		 else
	   {
			 
			 pstr->Para.u32Count5ms++;
			 if(pstr->Para.u32Count5ms>=COUNT5MS)			 /*5ms�����͸�λ*/
			 {			 
				 pstr->Para.u32Count5ms=0;
				 pstr->Para.u8FirstFlag=0;
				 				 
			 }
			 
			 pstr->Para.u32Count50ms++;
			 if(pstr->Para.u32Count50ms>=COUNT50MS)		/*50ms�����͸�λ������*/
			 {			 
				 pstr->Para.u32Count50ms=0;
					pstr->Out.u8statues=DETECTORNONE;/*�ȵͺ��*/
			 }			 			 			 
		 }	
	 
}




void Boundary_ADCIsr(BOUNDARY_STR *pstr )
{
	
	pstr->L.In.Postive =ADC_LP;
	pstr->L.In.Negative=ADC_LN;	
	pstr->R.In.Postive =ADC_RP;
	pstr->R.In.Negative=ADC_RN;

	Boundary_Deal(&pstr->L);
	Boundary_Deal(&pstr->R);


	
	
#if  DECTOR==DOWM /*20K HZ*/
	if((pstr->L.Out.u8statues==DETECTORIN)&&(pstr->R.Out.u8statues==DETECTORIN))/*����Ȧ��*/
	{
		 pstr->Out.eMowerPosition=MOWER_LIN_RIN;
	}
	else if((pstr->L.Out.u8statues==DETECTORIN)&&(pstr->R.Out.u8statues==DETECTOROUT))/*�����Ȧ�ڣ��ұ���Ȧ��*/
	{
			pstr->Out.eMowerPosition=MOWER_LIN_ROUT;
	}
	else if((pstr->L.Out.u8statues==DETECTOROUT)&&(pstr->R.Out.u8statues==DETECTORIN))/*�����Ȧ�⣬�ұ���Ȧ��*/
	{
			pstr->Out.eMowerPosition=MOWER_LOUT_RIN;
	}
	else if((pstr->L.Out.u8statues==DETECTOROUT)&&(pstr->R.Out.u8statues==DETECTOROUT))/*�����Ȧ�⣬�ұ���Ȧ��*/
	{
			pstr->Out.eMowerPosition=MOWER_LOUT_ROUT;
	}
	else/*����*/
	{ 
			pstr->Out.eMowerPosition=MOWER_NONE;
	}
	
#else 
	
	if((pstr->L.Out.u8statues==DETECTORIN)&&(pstr->R.Out.u8statues==DETECTORIN))/*����Ȧ��*/
	{
			pstr->Out.eMowerPosition=MOWER_LOUT_ROUT;		 
	}
	else if((pstr->L.Out.u8statues==DETECTORIN)&&(pstr->R.Out.u8statues==DETECTOROUT))/*�����Ȧ�ڣ��ұ���Ȧ��*/
	{
			pstr->Out.eMowerPosition=MOWER_LOUT_RIN;
	}
	else if((pstr->L.Out.u8statues==DETECTOROUT)&&(pstr->R.Out.u8statues==DETECTORIN))/*�����Ȧ�⣬�ұ���Ȧ��*/
	{			
		pstr->Out.eMowerPosition=MOWER_LIN_ROUT;
	}
	else if((pstr->L.Out.u8statues==DETECTOROUT)&&(pstr->R.Out.u8statues==DETECTOROUT))/*�����Ȧ�⣬�ұ���Ȧ��*/
	{
		pstr->Out.eMowerPosition=MOWER_LIN_RIN;
	
	}
	else/*����*/
	{ 
			pstr->Out.eMowerPosition=MOWER_NONE;
	}
		
#endif	
	

	
}
