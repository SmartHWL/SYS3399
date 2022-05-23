
#include "SC32F58128.h"
#include "sl_niv.h"
#include "SC32F58128_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_gpio.h"

#include "BSP_uart.h"
#include "UartToMotor.h"
#include "UartToPanel.h"
#include "UartToWifi.h"
#include "Control&Filter.h"
#include "UserConfig.h"
#include "f_uart0.h"
#include "BSP_iic.h"
#include "qmi8658c_iic.h"

/* 结构体指针声明 */
UARTMOTOR_STR  UartMotor;

/* 函数声明 */
void AppUart_Motor(void* parameter);
void vUartMotorInit(UARTMOTOR_STR *pstrUart, MOWER_FSM_STR *pstrFSM);
void vUartMotorLoad(void);	
void vUartMotorDeal(UART_BASIC_STR *pstr);
void vACKLoad(UART_BASIC_STR  *pstr);
void vUartClearFault(UART_BASIC_STR  *pstr);
/* 驱动函数声明 */;

void vUartSend(UART_BASIC_STR  *pstr,UART0_Type  *pstrUart);
void vUartReceive(UART_BASIC_STR  *pstr,UART0_Type *pstrUart,void (*pstrFun)(UART_BASIC_STR *pstr));

int16_t accel_data[3] ={0};
int16_t gyro_data[3]={0};
short raw =0;
float temperature=0;

/* 函数指针声明 */
UARTMOTOR_FUN_STR  strUartMotorFun=
{
	&vUartMotorInit,
	&vUartMotorLoad,
	&vUartMotorDeal,
	&vACKLoad,
  &vUartClearFault
};

UARTBASIC_FUN_STR  strUartBasicFun=
{
	&vUartSend,	
  &vUartReceive,

};


 void AppUart_Motor(void* parameter)/*后期统一形式*/	
{

	 static  uint16_t u8UartSendCnt;
	 strI2cQMIFun.pvI2CQMI6858C_Init();
    
    while (1)
    {		
			


        /*读取六轴陀螺仪数据*/
        strI2cQMIFun.pvI2CQMI_GetData();
        //			strI2cQMIFun.pvI2CQMI_Data_Process();
        accel_data[0] = qmi_get_date.accelx;
        accel_data[1] = qmi_get_date.accely;
        accel_data[2] = qmi_get_date.accelz;
        gyro_data[0] = qmi_get_date.gyrox;
        gyro_data[1] = qmi_get_date.gyroy;
        gyro_data[2] = qmi_get_date.gyroz;


        UartData.UartSendAccelXLenth = UartSendDataDeal(UartData.UartSendAccelXData,accel_data[0]);

        UartData.UartSendAccelYLenth = UartSendDataDeal(UartData.UartSendAccelYData,accel_data[1]);

        UartData.UartSendAccelZLenth = UartSendDataDeal(UartData.UartSendAccelZData,accel_data[2]); 

        UartData.UartSendGyroXLenth = UartSendDataDeal(UartData.UartSendGyroXData,gyro_data[0]); 

        UartData.UartSendGyroYLenth = UartSendDataDeal(UartData.UartSendGyroYData,gyro_data[1]); 

        UartData.UartSendGyroZLenth = UartSendDataDeal(UartData.UartSendGyroZData,gyro_data[2]); 

        //    UartData.UartSendTempLenth = UartSendDataDeal(UartData.UartSendTempData,raw); 
        u8UartSendCnt++;
        if(u8UartSendCnt>=1000)
        {
            u8UartSendCnt=0;

            UartSlaveResponseToMaster(1);
//            if((UartData.UartSendAccelXLenth+UartData.UartSendAccelYLenth+UartData.UartSendAccelZLenth+
//            UartData.UartSendGyroXLenth+UartData.UartSendGyroYLenth+UartData.UartSendGyroZLenth)!=0)
//            {UartSlaveResponseToMaster(1);}
//            else 
//            {}    
            
        }

			        
        // 通讯信息处理  
        UartRevMessageDeal(); //串口数据处理

    }

}

void vUartMotorInit(UARTMOTOR_STR *pstrUart, MOWER_FSM_STR *pstrFSM)
{

}

void vUartMotorLoad(void)
{
		
}

void vUartMotorDeal(UART_BASIC_STR  *pstr)
{
		
}

void vACKLoad(UART_BASIC_STR  *pstr)
{

}

void vUartClearFault(UART_BASIC_STR  *pstr)
{

}


void vUartSend(UART_BASIC_STR  *pstr,UART0_Type  *pstrUart)
{

}


void vUartReceive(UART_BASIC_STR  *pstr,UART0_Type *pstrUart,void(*pstrFun)(UART_BASIC_STR  *pstr))
{

}

/*============================================数据接收发送的驱动层============================================================*/

























