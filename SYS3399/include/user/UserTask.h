#ifndef __USERTASK_H_
#define __USERTASK_H_


#include "SC32F58128.h"
#include "SC32F58128_spi.h"

#include "qmi8658c_iic.h"
#include "BSP_gpio.h"
#include "BSP_spi.h"


#include "FreeRTOS.h"
#include "task.h"



#define Max(x,y)	(((x)>(y))?(x):(y))             //ȡ��ֵ����(����������������)
#define Min(x,y)	(((x)<(y))?(x):(y))             //ȡСֵ����(����������������)
#define abs(x)  	(((x)>0)?(x):-(x))             //ȡ����ֵ�ĺ���(����������������)



extern void UserTASK(void* parameter);









#endif






