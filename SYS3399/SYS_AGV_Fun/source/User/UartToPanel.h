#ifndef __UART_PANEL_H_
#define __UART_PANEL_H_
#include "stdint.h"
#include "string.h"
#include "MowerFSM.h"
#include "UartToMotor.h"

#define LCD_OFF  		   0 /*屏幕不亮不显示*/   
#define LCD_WELCOME    1 /*屏幕亮,进入开机界面 （进入密码配置）*/
#define LCD_BYEBYE     2 /*屏幕进入关机界面*/
#define LCD_ENERGY_CONSERVATION  2 /*进入待机界面（LCD不亮）*/


#define CMD_STOP  0xF0
#define CMD_START 0xF2
#define CMD_HOME  0xF1
#define CMD_FAULTCLEAR  0xF7


#define UL_START		     0
#define UL_LENG		       1
#define UL_CMD		       2

#define UL_YEAR		       3
#define UL_MONTH		     4
#define UL_DAY		       5
#define UL_HOUR		       6
#define UL_MINUTE		     7
#define UL_ERRORCODE		 8
#define UL_LCDONOFF	  	 9
#define UL_PINCODE	  	 10
#define UL_LANGUAGESELECT	   11
#define UL_TIMEFORMATSELECT	 12
#define UL_DAYFORMATSELECT	 13
#define UL_VERSION_HI	       14
#define UL_VERSION_LOW	     15
#define UL_CHARGEPERCENTAGE	 16
#define UL_STOPBIT	         17


/*从机发送启动命令*/  
/* 0xFA  0XF7  0xF1(启动命令)   OX1（返回充电桩使能）  0x01(工作时长，单位30分钟，) CRC 0xAF   */  

#define UL_CMD	          2
#define UL_RETURNENABLE	  3
#define UL_WORKTIME	      4

typedef struct  
{	
	
	UART_BASIC_STR Basic;
		
  struct
	{
		
		uint8_t  u8Command;
		uint8_t  u8CommandBAK;		
		bool bFaultReset;		
		bool bReturnofOneShotTime;				
		uint32_t  u32WorkTimeOfOneShotTime;
		
	}Out;
	
} UARTPANEL_STR;



typedef struct{
 void (*pvUartPanelInit)(UARTPANEL_STR *pstr);	
 void (*pvUartPanelLoad) (void);	
 void (*pvUartPanelDeal)(UART_BASIC_STR  *pstr);	
}UARTPANEL_FUN_STR;

extern UARTPANEL_STR  		UartPanel;  
extern UARTPANEL_FUN_STR  strUartPanelFun;

#endif


