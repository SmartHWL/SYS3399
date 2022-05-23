
#include "SC32F58128.h"
#include "devInit.h"
#include "timer4Tick.h"
#include "osc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_gpio.h"

/* Standard includes. */
#include <stdio.h>
#include "string.h"
#include "serial.h"

/* User includes. */
#include "UartToMotor.h"
#include "UartToPanel.h"
#include "UartToWifi.h"
#include "MowerFSM.h"
#include "MotorSpeedCalc.h"
#include "SampleInfo.h"
#include "Protect.h"
/******************************************************************************
 * Global data for the project
 ******************************************************************************/
static void AppTaskCreate(void);/* AppTask任务 */

 /* 创建任务句柄 */
static TaskHandle_t AppTask_Handle = NULL;

extern uint8_t SerialState;
extern void vOutputString( const char * const pcMessage );

int main_c1(void)
{
	return 0;
}


int main(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

  /* Device Init */
  INIT_DeviceInit();

  vRegisterSampleCLICommands();

  /* 创建AppTaskCreate任务 */

	
												
  xTaskCreate((TaskFunction_t )AppUart_Motor,  /* 任务入口函数 */
                        (const char*    )"AppUart_Motor",/* 任务名字 */
                        (uint16_t       )128,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTask_Handle);/* 任务控制块指针 */

												
												
  /* 启动任务调度 */ 												
  if (1) { //(pdPASS == xReturn){
//    vUARTCommandConsoleStart(512,1);
		vTaskStartScheduler(); /* 启动任务，开启调度 */
	}else{
    return -1;
	}

  while(1);   /* 正常不会执行到这里 */


}


/*---------------------------------- End of the file ---------------------------------*/
