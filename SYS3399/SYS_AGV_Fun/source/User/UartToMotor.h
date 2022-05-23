#ifndef __UART_MOTOR_H_
#define __UART_MOTOR_H_
#include "stdint.h"
#include "string.h"
#include "MowerFSM.h"

#define PKG_START 	    0xFA
#define PKG_ESC 				0xF9
#define PKG_END 				0xAF
#define BUFFER_LENGTH	  0x14//20
#define BUFFER_TX_LENGTH	  32



#define UL_START		     0
#define UL_LENG		       1
#define UL_CMD		       2

#define UL_MOTORID		   2

#define UL_CURRENT_H		 3
#define UL_CURRENT_L		 4

#define IDLEFTWALK   1
#define IDCUTTING    2
#define IDRIGHTWALK  3


typedef struct  
{
	uint8_t rx_state;	
	uint8_t rx_time;	
	uint8_t rx_esc;	
	uint8_t rx_idx;
	uint8_t rx_buff[BUFFER_LENGTH];
	
	uint8_t tx_state;	
	uint8_t tx_time;	
	uint8_t tx_size;	
	uint8_t tx_esc;	
	uint8_t tx_idx;	
	uint8_t tx_buff[BUFFER_TX_LENGTH];
	
  uint16_t sendtime_count;	
	uint16_t WatchDogCnt;			
}UART_BASIC_STR;




typedef struct  
{	
	
	UART_BASIC_STR Basic;
	
	struct  
	{
	  uint16_t u16Q14LeftCurrent;
		uint8_t  u8LeftRealCurrentX10;
		uint8_t  u8LeftRealPowerX10;		
		
	  uint16_t u16Q14CutCurrent;			
		uint8_t  u8CutRealCurrentX10;		
		uint8_t  u8CutRealPowerX10;
		
	  uint16_t u16Q14RightCurrent;		
		uint8_t  u8RightRealCurrentX10;	
		uint8_t  u8RightRealPowerX10;
		
	  uint16_t u16LeftMechSpeed;
	  uint16_t u16CutMechSpeed;			
	  uint16_t u16RightMechSpeed;		
		
		uint8_t u8ErrorCodeLeft;	
		uint8_t u8ErrorCodeCut;	
		uint8_t u8ErrorCodeRight;			
		
	}Out;
	
	uint8_t  RequestID;	
	uint8_t *pu8LWalkDir;
	bool    *pbCutDir;		
	uint8_t *pu8RWalkDir;		

	uint16_t *pu16LWalkSpeed;
	uint16_t *pu16CutSpeed;		
	uint16_t *pu16RWalkSpeed;	

}UARTMOTOR_STR ;




typedef struct{
 void (*pvUartSend)(UART_BASIC_STR  *pstr,UART0_Type  *pstrUart );
 void (*pvUartReceive)(UART_BASIC_STR  *pstr,UART0_Type *pstrUart,void (*pstrFun)(UART_BASIC_STR  *pstr));		
}UARTBASIC_FUN_STR;


typedef struct{
 void (*pvUartMotorInit)(UARTMOTOR_STR *pstrUart, MOWER_FSM_STR *pstrFSM);
 void (*pvUartMotorLoad)(void);		
 void (*pvUartMotorDeal)(UART_BASIC_STR *pstr);	
 void (*pvACKLoad)(UART_BASIC_STR  *pstr);
 void (*pvUartClearFault)(UART_BASIC_STR  *pstr);
}UARTMOTOR_FUN_STR;


extern UARTMOTOR_STR  UartMotor;
extern UARTBASIC_FUN_STR  strUartBasicFun;
extern UARTMOTOR_FUN_STR  strUartMotorFun;


extern void AppUart_Motor(void* parameter);

#endif


