
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

/* 结构体指针声明 */
UARTWIFI_STR  UartWifi;

/* 函数声明 */
void vUartWifiInit(UARTWIFI_STR *pstr);
void vUartWifiLoad(void);	
void vUartWifiDeal(UART_BASIC_STR  *pstr);

/* 函数指针声明 */
UARTWIFI_FUN_STR  strUartWifiFun=
{
	&vUartWifiInit,
	&vUartWifiLoad,
	&vUartWifiDeal
};



void vUartWifiInit(UARTWIFI_STR *pstr)
{
	memset(&UartWifi,0,sizeof(UartWifi));   /*初始化*/
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


void vUartWifiLoad(void)
{
	uint8_t i=0,check=0;
	
	UartWifi.Basic.tx_buff[0]=0XFA; /* 帧头 */
	UartWifi.Basic.tx_buff[1]=BUFFER_TX_LENGTH; /* 帧长 */
	UartWifi.Basic.tx_buff[2] =0xF0; 	/* 命令帧 */
	
	UartWifi.Basic.tx_buff[3] =strSampleInfo.BatteryVol.Out.u16UdcX10; /* 电池电压 */  
	UartWifi.Basic.tx_buff[4] =strSampleInfo.ChargeVol.Out.u16UChargeX10;  /* 充电电压 */  
	UartWifi.Basic.tx_buff[5] =strSampleInfo.ChargeCurr.Out.s16IChargeX10; /* 充电电流 */  	   
	UartWifi.Basic.tx_buff[6] =MowerFSM.CtrlLogic.State.eMowerDisplayStatus;/* 显示界面 */  
	
	UartWifi.Basic.tx_buff[7] =(uint8_t)(UartMotor.Out.u16LeftMechSpeed>>8);/* 左电机转速 */    	  	
	UartWifi.Basic.tx_buff[8] =(uint8_t)(UartMotor.Out.u16LeftMechSpeed);   /* 左电机转速 */ 	
	UartWifi.Basic.tx_buff[9] = UartMotor.Out.u8LeftRealCurrentX10;/* 左电机实际电流 */   	
	UartWifi.Basic.tx_buff[10] =UartMotor.Out.u8LeftRealPowerX10; /* 左电机实际功率 */ 
	
	UartWifi.Basic.tx_buff[11] =(uint8_t)(UartMotor.Out.u16RightMechSpeed>>8);/* 右电机转速 */    	  	
	UartWifi.Basic.tx_buff[12] =(uint8_t)(UartMotor.Out.u16RightMechSpeed);   /* 右电机转速 */ 	
	UartWifi.Basic.tx_buff[13] =UartMotor.Out.u8RightRealCurrentX10;/* 右电机实际电流 */   	
	UartWifi.Basic.tx_buff[14] =UartMotor.Out.u8RightRealPowerX10;  /* 右电机实际功率 */ 	
	
	
	UartWifi.Basic.tx_buff[15] =(uint8_t)(UartMotor.Out.u16CutMechSpeed>>8);/* 刀盘电机转速 */    	  	
	UartWifi.Basic.tx_buff[16] =(uint8_t)(UartMotor.Out.u16CutMechSpeed);   /* 刀盘电机转速 */ 	
	UartWifi.Basic.tx_buff[17] =UartMotor.Out.u8CutRealCurrentX10;/* 刀盘电机实际电流 */   	
	UartWifi.Basic.tx_buff[18] =UartMotor.Out.u8CutRealPowerX10; /* 刀盘电机实际功率 */ 
	
	
	UartWifi.Basic.tx_buff[7] =(*UartMotor.pu16LWalkSpeed)>>8; 	/* M1速度高位 */	
	UartWifi.Basic.tx_buff[8] =(*UartMotor.pu16LWalkSpeed);  		/* M1速度低位 */	
	UartWifi.Basic.tx_buff[15] =(*UartMotor.pu16CutSpeed)>>8; 	  /* M3速度高位 */	
	UartWifi.Basic.tx_buff[16] =(*UartMotor.pu16CutSpeed);  		  /* M3速度低位 */		
	UartWifi.Basic.tx_buff[11]=(*UartMotor.pu16RWalkSpeed)>>8; 	/* M2速度高位 */	
	UartWifi.Basic.tx_buff[12]=(*UartMotor.pu16RWalkSpeed);  		/* M2速度低位 */			
	
	UartWifi.Basic.tx_buff[19] =MowerFSM.TimeStatistics.Out.u8TimeMin>>8;  /*本次时长高位，分钟 */   	
	UartWifi.Basic.tx_buff[20] =MowerFSM.TimeStatistics.Out.u8TimeMin;     /* 本次时长低位，分钟 */ 	
	UartWifi.Basic.tx_buff[21] =0; /* 总工作时长高位 */   	
	UartWifi.Basic.tx_buff[22] =0; /* 总````````	工作时长低位 */	
	UartWifi.Basic.tx_buff[23] =strFault[0].Out.CurrentFault.u16ErrCode; /* 故障码 */		
	UartWifi.Basic.tx_buff[24] =0;   /* 倾斜角度 */	
	UartWifi.Basic.tx_buff[24] =0;   /* 超声波避障距离,0.1M */		
  UartWifi.Basic.tx_buff[24] =0;   /* 超声波避障距离 */		
	
	UartWifi.Basic.tx_buff[24] =CORE_SOFTWARE_MAIN_VERSION;   /* 软件版本高位 */		
  UartWifi.Basic.tx_buff[24] =CORE_SOFTWARE_SUB_VERSION;   /* 软件版本低位 */			
	
	check=Modbus_CRC16(UartWifi.Basic.tx_buff,(UartWifi.Basic.tx_buff[UL_LENG]-2));	/*校验*/
	UartWifi.Basic.tx_buff[UartWifi.Basic.tx_buff[UL_LENG]-2]=check;
	UartWifi.Basic.tx_buff[UartWifi.Basic.tx_buff[UL_LENG]-1]=PKG_END;  /* 帧尾 */			

	UartWifi.Basic.tx_size=UartWifi.Basic.tx_buff[UL_LENG];	 
	UartWifi.Basic.tx_idx=0;	
	
}

void vUartWifiDeal(UART_BASIC_STR  *pstr)
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