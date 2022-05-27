#ifndef __USERTASK_H_
#define __USERTASK_H_


#include "SC32F58128.h"
#include "SC32F58128_spi.h"

#include "qmi8658c_iic.h"
#include "BSP_gpio.h"
#include "BSP_spi.h"


#include "FreeRTOS.h"
#include "task.h"



#define Max(x,y)	(((x)>(y))?(x):(y))             //取大值函数(适用所有数据类型)
#define Min(x,y)	(((x)<(y))?(x):(y))             //取小值函数(适用所有数据类型)
#define abs(x)  	(((x)>0)?(x):-(x))             //取绝对值的函数(适用所有数据类型)



extern void UserTASK(void* parameter);









#endif






