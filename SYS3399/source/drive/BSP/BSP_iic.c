#include "SC32F58128.h"
#include "SC32F58128_iic.h"
#include "BSP_iic.h"
#include "BSP_gpio.h"
#include "UserConfig.h"
IIC_InfoDef IICInfo;
struct I2C_MSG strI2cMsg;

/************************************************* 
 Function:    BSP_IsI2cReady
 Description: �ж�IIC�Ƿ��ڿ���״̬
 Input:   
 Output:      
 Return: 
 Others: 
*************************************************/
bool BSP_IsI2cReady(void)
{
    bool bRet = FALSE;
    
    if(I2C->CSR_b.SI == FALSE)
    {
        bRet = TRUE;
    }
    
    return bRet;
}

/************************************************* 
 Function:    BSP_I2cStart
 Description: ����I2C
 Input:   
 Output:      
 Return: 
 Others: 
*************************************************/
void BSP_I2cStart(void)
{
     I2C->CSET_b.STA = 1; 
}

/************************************************* 
 Function:    BSP_I2cInit
 Description: I2C��ʼ��
 Input:   
 Output:      
 Return: 
 Others: 
*************************************************/
void BSP_I2cInit(void)
{
    	GPIO_MuxSel(GPIO_32,IIC_SDA_32);
  GPIO_MuxSel(GPIO_33,IIC_SCL_33);
	GPIO_PullUpDownSel(GPIO_32, PULL_UP);
	GPIO_PullUpDownSel(GPIO_33, PULL_UP);
    
//	GPIO_MuxSel(GPIO_28,IIC_SDA_28);
//  GPIO_MuxSel(GPIO_29,IIC_SCL_29);
//	GPIO_PullUpDownSel(GPIO_28, PULL_UP);
//	GPIO_PullUpDownSel(GPIO_29, PULL_UP);
//����IIC�ж�
	NVIC_ClearPendingIRQ(SSP23_IIC_IRQn);
	NVIC_SetPriority(SSP23_IIC_IRQn,0);
	NVIC_EnableIRQ(SSP23_IIC_IRQn);
	
//IIC��Ƶ
	I2C_Init(I2C_Division_Factor_2048);
}


/************************************************* 
 Function:    u16RwI2cBus
 Description: ��дI2C����
 Input:      u8Mode: ��/д/��ѯ
 Output:      
 Return: 
 Others: 
*************************************************/
uint16_t u16RwI2cBus(uint8_t u8Mode,uint8_t adrValue,uint8_t reValue,uint8_t setValue)
{
    uint8_t i;
    /* ����Ƿ�׼���� */
    if(FALSE == BSP_IsI2cReady())
    {
			return I2C_FAIL;
    }
    /* ��������Ƿ��� ����״̬ */
    if (strI2cMsg.status != I2C_MSG_STATUS_IDLE)
    {
        return I2C_FAIL;
    }

    IICInfo.DevIdRaw = (adrValue << 1);
		IICInfo.TxData[0]=reValue;
		
    
    if (u8Mode == RW_I2C_BUS_WRITE)
    {
			IICInfo.RxCnt = 0;
			IICInfo.TxCnt = 0;
			IICInfo.TxMax = 2;
			IICInfo.CmdRead = I2C_Write;
			IICInfo.DevId = IICInfo.DevIdRaw;
			IICInfo.TxData[1]=setValue;
			BSP_I2cStart(); 
    }
    else if (u8Mode == RW_I2C_BUS_READ) 
    {
			IICInfo.RxCnt = 0;
      IICInfo.TxCnt = 0;
      IICInfo.CmdRead = I2C_Read;
      IICInfo.DevId = IICInfo.DevIdRaw;
      IICInfo.RxMax = 1; 
			
      BSP_I2cStart(); 
    }

    return I2C_SUCCESS;
}

/************************************************* 
 Function:    u8Rw_IIC
 Description: IIC��д����
 Input:      
 Output:      
 Return: 
 Others: 
*************************************************/
void vRw_IIC(uint8_t u8Mode,uint8_t adrValue,uint8_t reValue,uint8_t setValue)
{
	
	if (I2C_MSG_STATUS_IDLE == strI2cMsg.status)
	{
		if (RW_QMI8658_WRITE == u8Mode)
		{
			if (u16RwI2cBus(RW_I2C_BUS_WRITE,adrValue,reValue,setValue) == I2C_SUCCESS)
			{
				strI2cMsg.status = I2C_MSG_STATUS_WRITE_BUSY;
			}
		}
		else //if (RW_EEPROM_READ == mode)
		{
			if(u16RwI2cBus(RW_I2C_BUS_READ,adrValue,reValue,setValue) == I2C_SUCCESS)
			{
				strI2cMsg.status = I2C_MSG_STATUS_READ_BUSY;
			}
		}
	}	
	else if (I2C_MSG_STATUS_RW_OK == strI2cMsg.status) // ��д�������
	{
		strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	}
}


