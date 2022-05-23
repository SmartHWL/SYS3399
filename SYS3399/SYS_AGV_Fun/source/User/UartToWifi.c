
#include "SC32F58128.h"
#include "sl_niv.h"
#include "SC32F58128_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_gpio.h"

#include "BSP_uart.h"
#include "UartToWifi.h"
#include "Control&Filter.h"
#include "SampleInfo.h"
#include "UartToPanel.h"
#include "UserConfig.h"
#include "Fault.h"

/* �ṹ��ָ������ */
UARTWIFI_STR  UartWifi;

/* �������� */
void vUartWifiInit(UARTWIFI_STR *pstr);
void vUartWifiLoad(void);	
void vUartWifiDeal(UART_BASIC_STR  *pstr);

/* ����ָ������ */
UARTWIFI_FUN_STR  strUartWifiFun=
{
	&vUartWifiInit,
	&vUartWifiLoad,
	&vUartWifiDeal
};



void vUartWifiInit(UARTWIFI_STR *pstr)
{
	memset(&UartWifi,0,sizeof(UartWifi));   /*��ʼ��*/
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


void vUartWifiLoad(void)
{
	uint8_t i=0,check=0;
	
	UartWifi.Basic.tx_buff[0]=0XFA; /* ֡ͷ */
	UartWifi.Basic.tx_buff[1]=BUFFER_TX_LENGTH; /* ֡�� */
	UartWifi.Basic.tx_buff[2] =0xF0; 	/* ����֡ */
	
	UartWifi.Basic.tx_buff[3] =strSampleInfo.BatteryVol.Out.u16UdcX10; /* ��ص�ѹ */  
	UartWifi.Basic.tx_buff[4] =strSampleInfo.ChargeVol.Out.u16UChargeX10;  /* ����ѹ */  
	UartWifi.Basic.tx_buff[5] =strSampleInfo.ChargeCurr.Out.s16IChargeX10; /* ������ */  	   
	UartWifi.Basic.tx_buff[6] =MowerFSM.CtrlLogic.State.eMowerDisplayStatus;/* ��ʾ���� */  
	
	UartWifi.Basic.tx_buff[7] =(uint8_t)(UartMotor.Out.u16LeftMechSpeed>>8);/* ����ת�� */    	  	
	UartWifi.Basic.tx_buff[8] =(uint8_t)(UartMotor.Out.u16LeftMechSpeed);   /* ����ת�� */ 	
	UartWifi.Basic.tx_buff[9] = UartMotor.Out.u8LeftRealCurrentX10;/* ����ʵ�ʵ��� */   	
	UartWifi.Basic.tx_buff[10] =UartMotor.Out.u8LeftRealPowerX10; /* ����ʵ�ʹ��� */ 
	
	UartWifi.Basic.tx_buff[11] =(uint8_t)(UartMotor.Out.u16RightMechSpeed>>8);/* �ҵ��ת�� */    	  	
	UartWifi.Basic.tx_buff[12] =(uint8_t)(UartMotor.Out.u16RightMechSpeed);   /* �ҵ��ת�� */ 	
	UartWifi.Basic.tx_buff[13] =UartMotor.Out.u8RightRealCurrentX10;/* �ҵ��ʵ�ʵ��� */   	
	UartWifi.Basic.tx_buff[14] =UartMotor.Out.u8RightRealPowerX10;  /* �ҵ��ʵ�ʹ��� */ 	
	
	
	UartWifi.Basic.tx_buff[15] =(uint8_t)(UartMotor.Out.u16CutMechSpeed>>8);/* ���̵��ת�� */    	  	
	UartWifi.Basic.tx_buff[16] =(uint8_t)(UartMotor.Out.u16CutMechSpeed);   /* ���̵��ת�� */ 	
	UartWifi.Basic.tx_buff[17] =UartMotor.Out.u8CutRealCurrentX10;/* ���̵��ʵ�ʵ��� */   	
	UartWifi.Basic.tx_buff[18] =UartMotor.Out.u8CutRealPowerX10; /* ���̵��ʵ�ʹ��� */ 
	
	
	UartWifi.Basic.tx_buff[7] =(*UartMotor.pu16LWalkSpeed)>>8; 	/* M1�ٶȸ�λ */	
	UartWifi.Basic.tx_buff[8] =(*UartMotor.pu16LWalkSpeed);  		/* M1�ٶȵ�λ */	
	UartWifi.Basic.tx_buff[15] =(*UartMotor.pu16CutSpeed)>>8; 	  /* M3�ٶȸ�λ */	
	UartWifi.Basic.tx_buff[16] =(*UartMotor.pu16CutSpeed);  		  /* M3�ٶȵ�λ */		
	UartWifi.Basic.tx_buff[11]=(*UartMotor.pu16RWalkSpeed)>>8; 	/* M2�ٶȸ�λ */	
	UartWifi.Basic.tx_buff[12]=(*UartMotor.pu16RWalkSpeed);  		/* M2�ٶȵ�λ */			
	
	UartWifi.Basic.tx_buff[19] =MowerFSM.TimeStatistics.Out.u8TimeMin>>8;  /*����ʱ����λ������ */   	
	UartWifi.Basic.tx_buff[20] =MowerFSM.TimeStatistics.Out.u8TimeMin;     /* ����ʱ����λ������ */ 	
	UartWifi.Basic.tx_buff[21] =0; /* �ܹ���ʱ����λ */   	
	UartWifi.Basic.tx_buff[22] =0; /* ��````````	����ʱ����λ */	
	UartWifi.Basic.tx_buff[23] =strFault[0].Out.CurrentFault.u16ErrCode; /* ������ */		
	UartWifi.Basic.tx_buff[24] =0;   /* ��б�Ƕ� */	
	UartWifi.Basic.tx_buff[24] =0;   /* ���������Ͼ���,0.1M */		
  UartWifi.Basic.tx_buff[24] =0;   /* ���������Ͼ��� */		
	
	UartWifi.Basic.tx_buff[24] =CORE_SOFTWARE_MAIN_VERSION;   /* ����汾��λ */		
  UartWifi.Basic.tx_buff[24] =CORE_SOFTWARE_SUB_VERSION;   /* ����汾��λ */			
	
	check=Modbus_CRC16(UartWifi.Basic.tx_buff,(UartWifi.Basic.tx_buff[UL_LENG]-2));	/*У��*/
	UartWifi.Basic.tx_buff[UartWifi.Basic.tx_buff[UL_LENG]-2]=check;
	UartWifi.Basic.tx_buff[UartWifi.Basic.tx_buff[UL_LENG]-1]=PKG_END;  /* ֡β */			

	UartWifi.Basic.tx_size=UartWifi.Basic.tx_buff[UL_LENG];	 
	UartWifi.Basic.tx_idx=0;	
	
}

void vUartWifiDeal(UART_BASIC_STR  *pstr)
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