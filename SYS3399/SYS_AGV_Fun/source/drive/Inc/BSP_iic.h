#ifndef __BSP_IIC_H_
#define __BSP_IIC_H_
#include "SC32F58128_iic.h"
#include "stdbool.h"
#include "Control&Filter.h"

#define	Bufferlength	60
#define I2C_MSG_BUFFER_SIZE  Bufferlength	/* 缓存个数 */
#define I2C_SUCCESS   1
#define I2C_FAIL      0
extern IIC_InfoDef IICInfo;


enum
{
	RW_I2C_BUS_WRITE  =  0,   /* 写 */
	RW_I2C_BUS_READ   =  1,   /* 读 */
	RW_I2C_BUS_ACK    =  2    /* 查询 */
};

enum
{
	RW_QMI8658_WRITE  =   0,   /* 写 */
	RW_QMI8658_READ   =   1    /* 读 */
};

enum I2C_MSG_STATUS
{
    I2C_MSG_STATUS_IDLE,                   // 空闲
    I2C_MSG_STATUS_WRITE_BUSY,             // EEPROM内部正在擦写数据
    //I2C_MSG_STATUS_SEND_NOSTOP_BUSY,       // DSP正在发送要读取的数据控制字
    //I2C_MSG_STATUS_RESTART,                // DSP发送要读取控制字完毕
    I2C_MSG_STATUS_READ_BUSY,              // DSP准备接收数据
    I2C_MSG_STATUS_RW_OK                   // 读写完毕
};
// I2C Message Structure
struct I2C_MSG 
{
  enum I2C_MSG_STATUS status;             	/* I2C状态 */
  uint16_t u16bytes;                     	/* BUFF字节数 */ 
  uint8_t u8HighAddr;                      	/* 地址高8位 */
  uint8_t u8LowAddr;						/* 地址低8位 */
  uint8_t u8TxBuffer[I2C_MSG_BUFFER_SIZE];  /* 发送缓存数组 */
  uint8_t u8RxBuffer[I2C_MSG_BUFFER_SIZE]; 	/* 接受缓存数组 */                                  
};
extern struct I2C_MSG strI2cMsg;



void BSP_I2cInit(void);
void vRw_IIC(uint8_t u8Mode,uint8_t adrValue,uint8_t reValue,uint8_t setValue);
#endif
