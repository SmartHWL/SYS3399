/************************************************************ 
 Copyright (C), 2021, ShiTeng Tech. Co., Ltd. 
 FileName:   Boundary.c
 Author:     Version :   Date: 
 xyb         1.0         2022年1月11日
 Description: 采样信息获取
 Version: 版本信息
 History: 
 <author>    <time>          <version >   <desc> 
 xyb        2022年1月11日       1.0        初版
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


/* 结构体指针声明 */
BOUNDARY_STR  Boundary;  
/* 函数声明 */
void Boundary_Init(BOUNDARY_STR *pstr );
void Boundary_Deal(BOUNDARY_SUB_STR *pstr);
void Boundary_ADCIsr(BOUNDARY_STR *pstr );	
/* 函数指针声明 */
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

14ms的周期脉冲，脉宽150us，ADC 50us采样一次

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

在边界线圈内的图示

*/
void Boundary_Deal(BOUNDARY_SUB_STR *pstr )
{
     uint8_t  lu8FilterCount;
	
	  if(abs((int)(pstr->In.Postive-pstr->In.Negative))>1200)/*ADC差值大于一定值判定为有边界信号*/
	  { 							   							
			if(pstr->Para.u8FirstFlag==0)
			{
					
				pstr->Para.u8FirstFlag=1;/*第一次进入到差值里面,并且记录标志位*/
			  pstr->Para.u32Count5ms=0; 		/*5ms内有接收到*/			
				pstr->Para.u32Count50ms=0;    /*50ms内有接收到*/
				
				if(pstr->In.Postive>pstr->In.Negative)
				{
					pstr->Out.u8statuesCurrent=DETECTORIN;/*先高后低*/
				}					
				else 									
				{
					pstr->Out.u8statuesCurrent=DETECTOROUT;/*先低后高*/
				}		
			 }	
			
			 
		 
			 /*对类型滤波*/		
			 /*14ms时间间隔计算，以免是干扰*/		
			 /*第一段之后，要确保第二段很快就下来，计时，以免是别的干扰	 			 
			 */					 			 			 			 
			 		 
			 
			 /*对信号进行滤波*/		
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
			 if(pstr->Para.u32Count5ms>=COUNT5MS)			 /*5ms后类型复位*/
			 {			 
				 pstr->Para.u32Count5ms=0;
				 pstr->Para.u8FirstFlag=0;
				 				 
			 }
			 
			 pstr->Para.u32Count50ms++;
			 if(pstr->Para.u32Count50ms>=COUNT50MS)		/*50ms后类型复位，故障*/
			 {			 
				 pstr->Para.u32Count50ms=0;
					pstr->Out.u8statues=DETECTORNONE;/*先低后高*/
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
	if((pstr->L.Out.u8statues==DETECTORIN)&&(pstr->R.Out.u8statues==DETECTORIN))/*都在圈内*/
	{
		 pstr->Out.eMowerPosition=MOWER_LIN_RIN;
	}
	else if((pstr->L.Out.u8statues==DETECTORIN)&&(pstr->R.Out.u8statues==DETECTOROUT))/*左边在圈内，右边在圈外*/
	{
			pstr->Out.eMowerPosition=MOWER_LIN_ROUT;
	}
	else if((pstr->L.Out.u8statues==DETECTOROUT)&&(pstr->R.Out.u8statues==DETECTORIN))/*左边在圈外，右边在圈内*/
	{
			pstr->Out.eMowerPosition=MOWER_LOUT_RIN;
	}
	else if((pstr->L.Out.u8statues==DETECTOROUT)&&(pstr->R.Out.u8statues==DETECTOROUT))/*左边在圈外，右边在圈外*/
	{
			pstr->Out.eMowerPosition=MOWER_LOUT_ROUT;
	}
	else/*故障*/
	{ 
			pstr->Out.eMowerPosition=MOWER_NONE;
	}
	
#else 
	
	if((pstr->L.Out.u8statues==DETECTORIN)&&(pstr->R.Out.u8statues==DETECTORIN))/*都在圈内*/
	{
			pstr->Out.eMowerPosition=MOWER_LOUT_ROUT;		 
	}
	else if((pstr->L.Out.u8statues==DETECTORIN)&&(pstr->R.Out.u8statues==DETECTOROUT))/*左边在圈内，右边在圈外*/
	{
			pstr->Out.eMowerPosition=MOWER_LOUT_RIN;
	}
	else if((pstr->L.Out.u8statues==DETECTOROUT)&&(pstr->R.Out.u8statues==DETECTORIN))/*左边在圈外，右边在圈内*/
	{			
		pstr->Out.eMowerPosition=MOWER_LIN_ROUT;
	}
	else if((pstr->L.Out.u8statues==DETECTOROUT)&&(pstr->R.Out.u8statues==DETECTOROUT))/*左边在圈外，右边在圈外*/
	{
		pstr->Out.eMowerPosition=MOWER_LIN_RIN;
	
	}
	else/*故障*/
	{ 
			pstr->Out.eMowerPosition=MOWER_NONE;
	}
		
#endif	
	

	
}
