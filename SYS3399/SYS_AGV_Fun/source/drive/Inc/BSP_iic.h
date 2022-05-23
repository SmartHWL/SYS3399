#ifndef __BSP_IIC_H_
#define __BSP_IIC_H_
#include "SC32F58128_iic.h"
#include "stdbool.h"
#include "Control&Filter.h"

#define	Bufferlength	60
#define I2C_MSG_BUFFER_SIZE  Bufferlength	/* ������� */
#define I2C_SUCCESS   1
#define I2C_FAIL      0
extern IIC_InfoDef IICInfo;


enum
{
	RW_I2C_BUS_WRITE  =  0,   /* д */
	RW_I2C_BUS_READ   =  1,   /* �� */
	RW_I2C_BUS_ACK    =  2    /* ��ѯ */
};

enum
{
	RW_QMI8658_WRITE  =   0,   /* д */
	RW_QMI8658_READ   =   1    /* �� */
};

enum I2C_MSG_STATUS
{
    I2C_MSG_STATUS_IDLE,                   // ����
    I2C_MSG_STATUS_WRITE_BUSY,             // EEPROM�ڲ����ڲ�д����
    //I2C_MSG_STATUS_SEND_NOSTOP_BUSY,       // DSP���ڷ���Ҫ��ȡ�����ݿ�����
    //I2C_MSG_STATUS_RESTART,                // DSP����Ҫ��ȡ���������
    I2C_MSG_STATUS_READ_BUSY,              // DSP׼����������
    I2C_MSG_STATUS_RW_OK                   // ��д���
};
// I2C Message Structure
struct I2C_MSG 
{
  enum I2C_MSG_STATUS status;             	/* I2C״̬ */
  uint16_t u16bytes;                     	/* BUFF�ֽ��� */ 
  uint8_t u8HighAddr;                      	/* ��ַ��8λ */
  uint8_t u8LowAddr;						/* ��ַ��8λ */
  uint8_t u8TxBuffer[I2C_MSG_BUFFER_SIZE];  /* ���ͻ������� */
  uint8_t u8RxBuffer[I2C_MSG_BUFFER_SIZE]; 	/* ���ܻ������� */                                  
};
extern struct I2C_MSG strI2cMsg;



void BSP_I2cInit(void);
void vRw_IIC(uint8_t u8Mode,uint8_t adrValue,uint8_t reValue,uint8_t setValue);
#endif
