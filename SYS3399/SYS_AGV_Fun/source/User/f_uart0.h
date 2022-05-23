#ifndef __UART0_H__
#define __UART0_H__


//#include "SC32F58128_uart.h"
#include "stdint.h" 
#include "BSP_gpio.h"
#include "stdint.h"
//#include "Apply_Func.h"
#include "protect.h"


#define UART_SEND_BUF_SIZE		250
#define UART_REV_BUF_SIZE		100
#define UART_DATA_SIZE		20

typedef struct
{
	uint8_t SendBuf[UART_SEND_BUF_SIZE]; //串口发送缓存
//	uint8_t RevBuf[UART_REV_BUF_SIZE]; //串口接收缓存

//	uint8_t RevData[UART_DATA_SIZE];

	uint16_t  SBWriteIdx;     //发送缓存写入序号
	uint16_t  SBReadIdx;      //发送缓存读取序号
	uint16_t  UnSendCnt;    	//未发送字节数

	uint8_t	IntSendFlag;
	

//	uint16_t RBWriteIdx;
//	uint16_t RBReadIdx;
//	uint16_t UnRevCnt;    	

//	uint16_t RevStart;
//	uint16_t RevLen;
//	uint8_t	RevOK;
//	uint8_t Datalen;      //CDW added this on MAY 18th.
	
}UART_PARAM_s;

#define UART_MAX_LENGTH 100
//#define UART_SLABE_ADDRESS 0x01  //暂为宏定义  

typedef struct 
{
	uint8_t RevNum;
	uint8_t ReadNum;
	uint8_t RevBuf[UART_MAX_LENGTH];
  uint32_t UartFrameCount;
  uint8_t RevFrame[20];
	uint8_t RevLenth;
	uint8_t FrameLenth;
    
    uint8_t UartSendAccelXData[4];
    uint8_t UartSendAccelXLenth;     
    uint8_t UartSendAccelYData[4];
    uint8_t UartSendAccelYLenth;     
    uint8_t UartSendAccelZData[4];
    uint8_t UartSendAccelZLenth;     
    uint8_t UartSendGyroXData[4];
    uint8_t UartSendGyroXLenth;    
    uint8_t UartSendGyroYData[4];
    uint8_t UartSendGyroYLenth;     
    uint8_t UartSendGyroZData[4];
    uint8_t UartSendGyroZLenth;      
    uint8_t UartSendTempData[4]; 
    uint8_t UartSendTempLenth;      
}UART_DATA_STR;
extern UART_DATA_STR UartData;


extern UART_PARAM_s Uart;
extern uint8_t ReadMessage[30];
extern uint8_t UartSlaveToMasterData[30];	//从机反馈数组

extern uint8_t SummationCheck(uint8_t *CheckFrame, uint8_t Length);
extern void Uart0Init(void);
extern void CommDataRev(void);
extern void UartSendDataToBuf(uint8_t length, uint8_t*cmd);
extern void UartParamInit(void);
extern uint8_t McuUart0Read(uint8_t *cmd);
extern void McuUart0deal(uint8_t *cmdptr);
extern void UartRevMessageDeal(void);
extern void UartSlaveResponseToMaster(uint16_t QueryNum);  //输入为查询码
uint8_t UartSendDataDeal(uint8_t *cmdptr, int16_t ptr );
void ClearUartSendData(void);







#endif