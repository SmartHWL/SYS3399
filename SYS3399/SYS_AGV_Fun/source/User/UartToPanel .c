#include "SC32F58128.h"
#include "sl_niv.h"
#include "SC32F58128_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_gpio.h"

#include "BSP_uart.h"
#include "UartToPanel.h"
#include "Control&Filter.h"
#include "SampleInfo.h"
#include "Fault.h"
#include "UserConfig.h"

/* �ṹ��ָ������ */
UARTPANEL_STR  UartPanel;

/* �������� */
void vUartPanelInit(UARTPANEL_STR *pstr);
void vUartPanelLoad(void);	
void vUartPanelDeal(UART_BASIC_STR  *pstr);

/* ����ָ������ */
UARTPANEL_FUN_STR  strUartPanelFun=
{
	&vUartPanelInit,
	&vUartPanelLoad,
	&vUartPanelDeal
};

void vUartPanelInit(UARTPANEL_STR *pstr)
{  
	memset(&UartPanel,0,sizeof(UartPanel));   /*��ʼ��*/	
}




/************************************************* 
 Function:    uart0_load
 Description: ��������
			 0xFA   0x14     
       ֡ͷ   ֡��       

 Input:      
 Output:      
 Return: 
 Others: 
*************************************************/

void vUartPanelLoad(void)
{
	uint8_t i=0,check=0;
	
	UartPanel.Basic.tx_buff[UL_START]=0XFA; /* ֡ͷ */
	UartPanel.Basic.tx_buff[UL_LENG]=BUFFER_LENGTH; /* ֡�� */
	UartPanel.Basic.tx_buff[UL_CMD]  		  =0x0F0; 	 /* ���� */	
	UartPanel.Basic.tx_buff[UL_YEAR]  		=0x00; 	   /* �� */
	UartPanel.Basic.tx_buff[UL_MONTH] 		=0x01;     /* �� */		
	UartPanel.Basic.tx_buff[UL_DAY]   		=0x01;  	 /* ��*/		
	UartPanel.Basic.tx_buff[UL_HOUR]  		=0x01; 	   /* ʱ*/	
	UartPanel.Basic.tx_buff[UL_MINUTE] 		=0x01;     /* ��*/	
	UartPanel.Basic.tx_buff[UL_ERRORCODE] =strFault[0].Out.CurrentFault.u16ErrCode;  	 /* ������*/			
	UartPanel.Basic.tx_buff[UL_LCDONOFF]  =MowerFSM.u8onoff;  	/* ��Ļ����ָʾ*/			
	UartPanel.Basic.tx_buff[UL_PINCODE]   =0x00;   				 /* PIN��*/	
	UartPanel.Basic.tx_buff[UL_LANGUAGESELECT]   =0x00;   /* ���Ը�ʽ*/			
	UartPanel.Basic.tx_buff[UL_TIMEFORMATSELECT] =0x00;   /* ʱ���ʽ*/			
	UartPanel.Basic.tx_buff[UL_DAYFORMATSELECT]  =0x00;   /* ���ڸ�ʽ*/			
	UartPanel.Basic.tx_buff[UL_VERSION_HI]  =CORE_SOFTWARE_MAIN_VERSION;    /* �汾�Ÿ�λ*/	
	UartPanel.Basic.tx_buff[UL_VERSION_LOW] =CORE_SOFTWARE_SUB_VERSION;    /* �汾�ŵ�λ*/			
	UartPanel.Basic.tx_buff[UL_CHARGEPERCENTAGE] =strSampleInfo.BatteryVol.Out.u16UdcPercentage;   /* �����ٷֱ�*/			
	UartPanel.Basic.tx_buff[UL_STOPBIT] = MowerFSM.CtrlLogic.State.eMowerDisplayStatus;   /* ֹͣλ*/		
	
	UartPanel.Basic.tx_buff[18]=0X1;  /* checksum */
	UartPanel.Basic.tx_buff[19]=PKG_END; /* ֡β */

	check=Modbus_CRC16(UartPanel.Basic.tx_buff,(UartPanel.Basic.tx_buff[UL_LENG]-2));	/*У��*/
	UartPanel.Basic.tx_buff[UartPanel.Basic.tx_buff[UL_LENG]-2]=check;
	UartPanel.Basic.tx_buff[UartPanel.Basic.tx_buff[UL_LENG]-1]=PKG_END;  /* ֡β */			

	UartPanel.Basic.tx_size=UartPanel.Basic.tx_buff[UL_LENG];	 
	UartPanel.Basic.tx_idx=0;	
	
}



void vUartPanelDeal(UART_BASIC_STR  *pstr)
{
	if(pstr->rx_buff[UL_CMD]==CMD_START)   /* ����ָ�� */
	{
		
	  	UartPanel.Out.u8Command=CMD_START;
		  UartPanel.Out.u32WorkTimeOfOneShotTime=pstr->rx_buff[UL_WORKTIME]*TIMEMIN(5);
		  UartPanel.Out.bReturnofOneShotTime=pstr->rx_buff[UL_RETURNENABLE];	
		
			strUartMotorFun.pvACKLoad(pstr);/* ����ACKָ�� */	
		
	}
	else if(pstr->rx_buff[UL_CMD]==CMD_HOME)/* Homeָ�� */
	{							
		  UartPanel.Out.u8Command=CMD_HOME;
			strUartMotorFun.pvACKLoad(pstr);		
	}
	else if(pstr->rx_buff[UL_CMD]==CMD_STOP)/* ָֹͣ�� */
	{	
		
			UartPanel.Out.u8Command=CMD_STOP;		
			strUartMotorFun.pvACKLoad(pstr);		
		
	}
	else if(pstr->rx_buff[UL_CMD]==CMD_FAULTCLEAR)/* �������ָ�� */
	{		
			UartPanel.Out.bFaultReset=TRUE;
			strUartMotorFun.pvACKLoad(pstr);				
	}
	else
	{
		
	}

}