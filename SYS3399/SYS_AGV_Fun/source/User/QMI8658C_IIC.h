#ifndef __QMI8658C_IIC_H__
#define __QMI8658C_IIC_H__

#include <SC32F58128.h>
#include "math.h"
//#include "stdlib.h"
#include "SC32F58128.h"
#include "SC32F58128_Gpio.h"
#include "SC32F58128_iic.h"

#define QMI_CHECKLEN  6
#define QMI_SAMLEN    6

/*******QMI8658C相应寄存器地址********/
/*设备地址*/
#define QMI8658C_ADDress  0x6B

/*控制寄存器*/
#define QMI_CTRL1_ADD 0x02 //传感器使能，配置SPI3或4线
#define QMI_CTRL2_ADD 0x03 //配置加速度计，启用自检
#define QMI_CTRL3_ADD 0x04 //配置陀螺仪，启用自检
#define QMI_CTRL5_ADD 0x06 //传感器过滤器
#define QMI_CTRL7_ADD 0x08  //启动传感器
#define QMI_CTRL9_ADD 0x0A  //主机命令

/*数据寄存器*/
#define QMI_AX_L_ADD  0x35
//#define QMI_AX_H_ADD  0x36
//#define QMI_AY_L_ADD  0x37
//#define QMI_AY_H_ADD  0x38
//#define QMI_AZ_L_ADD  0x39
//#define QMI_AZ_H_ADD  0x3A

//#define QMI_GX_L_ADD  0x3B
//#define QMI_GX_H_ADD  0x3C
//#define QMI_GY_L_ADD  0x3D
//#define QMI_GY_H_ADD  0x3E
//#define QMI_GZ_L_ADD  0x3F
//#define QMI_GZ_H_ADD  0x40


/***陀螺仪数据计算相关参数***/
#define Kp 100.0f//比例增益支配率收敛到加速度计/磁强计
#define Ki 0.002f//积分增益支配率的陀螺仪偏见的衔接
#define HALFT 0.001f//采样周期的一半


typedef struct 
{
	int_least16_t mqi_buff[12];
	int_least16_t ax[QMI_CHECKLEN];
	int_least16_t ay[QMI_CHECKLEN];
	int_least16_t az[QMI_CHECKLEN];
	int_least16_t gx[QMI_CHECKLEN];
	int_least16_t gy[QMI_CHECKLEN];
	int_least16_t gz[QMI_CHECKLEN];
	int_least16_t ax_adv;
	int_least16_t ay_adv;
	int_least16_t az_adv;
	int_least16_t gx_adv;
	int_least16_t gy_adv;
	int_least16_t gz_adv;
}MQI_DATE_ZEROCHECK;
extern MQI_DATE_ZEROCHECK mqi_date_zerocheck;


typedef struct 
{
	int_least16_t accelx;
	int_least16_t accely;
	int_least16_t accelz;
	int_least16_t gyrox;
	int_least16_t gyroy;
	int_least16_t gyroz;
}QMI_DATA;
extern  QMI_DATA qmi_get_date;


typedef struct
{
	float Yaw;//偏航角     沿z轴转时变化
	float Pitch;//俯仰角   沿y轴转时变化
	float Roll;//翻滚角    沿x轴转时变化
} QUATERnion;
extern  QUATERnion quaternion;



typedef struct{
 void (*pvI2CQMI6858C_Init)(void);
 void (*pvI2CQMI_GetData)(void);		
// void (*pvI2CQMI_Data_Process)(void);	
}I2CQMI_FUN_STR;
extern I2CQMI_FUN_STR  strI2cQMIFun;


#endif
