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

/* 结构体指针声明 */
UARTPANEL_STR  UartPanel;

/* 函数声明 */
void vUartPanelInit(UARTPANEL_STR *pstr);
void vUartPanelLoad(void);	
void vUartPanelDeal(UART_BASIC_STR  *pstr);

/* 函数指针声明 */
UARTPANEL_FUN_STR  strUartPanelFun=
{
	&vUartPanelInit,
	&vUartPanelLoad,
	&vUartPanelDeal
};

void vUartPanelInit(UARTPANEL_STR *pstr)
{  
	memset(&UartPanel,0,sizeof(UartPanel));   /*初始化*/	
}




/************************************************* 
 Function:    uart0_load
 Description: 加载数据
			 0xFA   0x14     
       帧头   帧长       

 Input:      
 Output:      
 Return: 
 Others: 
*************************************************/

void vUartPanelLoad(void)
{
	uint8_t i=0,check=0;
	
	UartPanel.Basic.tx_buff[UL_START]=0XFA; /* 帧头 */
	UartPanel.Basic.tx_buff[UL_LENG]=BUFFER_LENGTH; /* 帧长 */
	UartPanel.Basic.tx_buff[UL_CMD]  		  =0x0F0; 	 /* 命令 */	
	UartPanel.Basic.tx_buff[UL_YEAR]  		=0x00; 	   /* 年 */
	UartPanel.Basic.tx_buff[UL_MONTH] 		=0x01;     /* 月 */		
	UartPanel.Basic.tx_buff[UL_DAY]   		=0x01;  	 /* 日*/		
	UartPanel.Basic.tx_buff[UL_HOUR]  		=0x01; 	   /* 时*/	
	UartPanel.Basic.tx_buff[UL_MINUTE] 		=0x01;     /* 秒*/	
	UartPanel.Basic.tx_buff[UL_ERRORCODE] =strFault[0].Out.CurrentFault.u16ErrCode;  	 /* 故障码*/			
	UartPanel.Basic.tx_buff[UL_LCDONOFF]  =MowerFSM.u8onoff;  	/* 屏幕亮灭指示*/			
	UartPanel.Basic.tx_buff[UL_PINCODE]   =0x00;   				 /* PIN码*/	
	UartPanel.Basic.tx_buff[UL_LANGUAGESELECT]   =0x00;   /* 语言格式*/			
	UartPanel.Basic.tx_buff[UL_TIMEFORMATSELECT] =0x00;   /* 时间格式*/			
	UartPanel.Basic.tx_buff[UL_DAYFORMATSELECT]  =0x00;   /* 日期格式*/			
	UartPanel.Basic.tx_buff[UL_VERSION_HI]  =CORE_SOFTWARE_MAIN_VERSION;    /* 版本号高位*/	
	UartPanel.Basic.tx_buff[UL_VERSION_LOW] =CORE_SOFTWARE_SUB_VERSION;    /* 版本号低位*/			
	UartPanel.Basic.tx_buff[UL_CHARGEPERCENTAGE] =strSampleInfo.BatteryVol.Out.u16UdcPercentage;   /* 电量百分比*/			
	UartPanel.Basic.tx_buff[UL_STOPBIT] = MowerFSM.CtrlLogic.State.eMowerDisplayStatus;   /* 停止位*/		
	
	UartPanel.Basic.tx_buff[18]=0X1;  /* checksum */
	UartPanel.Basic.tx_buff[19]=PKG_END; /* 帧尾 */

	check=Modbus_CRC16(UartPanel.Basic.tx_buff,(UartPanel.Basic.tx_buff[UL_LENG]-2));	/*校验*/
	UartPanel.Basic.tx_buff[UartPanel.Basic.tx_buff[UL_LENG]-2]=check;
	UartPanel.Basic.tx_buff[UartPanel.Basic.tx_buff[UL_LENG]-1]=PKG_END;  /* 帧尾 */			

	UartPanel.Basic.tx_size=UartPanel.Basic.tx_buff[UL_LENG];	 
	UartPanel.Basic.tx_idx=0;	
	
}



void vUartPanelDeal(UART_BASIC_STR  *pstr)
{
	if(pstr->rx_buff[UL_CMD]==CMD_START)   /* 启动指令 */
	{
		
	  	UartPanel.Out.u8Command=CMD_START;
		  UartPanel.Out.u32WorkTimeOfOneShotTime=pstr->rx_buff[UL_WORKTIME]*TIMEMIN(5);
		  UartPanel.Out.bReturnofOneShotTime=pstr->rx_buff[UL_RETURNENABLE];	
		
			strUartMotorFun.pvACKLoad(pstr);/* 发送ACK指令 */	
		
	}
	else if(pstr->rx_buff[UL_CMD]==CMD_HOME)/* Home指令 */
	{							
		  UartPanel.Out.u8Command=CMD_HOME;
			strUartMotorFun.pvACKLoad(pstr);		
	}
	else if(pstr->rx_buff[UL_CMD]==CMD_STOP)/* 停止指令 */
	{	
		
			UartPanel.Out.u8Command=CMD_STOP;		
			strUartMotorFun.pvACKLoad(pstr);		
		
	}
	else if(pstr->rx_buff[UL_CMD]==CMD_FAULTCLEAR)/* 故障清除指令 */
	{		
			UartPanel.Out.bFaultReset=TRUE;
			strUartMotorFun.pvACKLoad(pstr);				
	}
	else
	{
		
	}

}