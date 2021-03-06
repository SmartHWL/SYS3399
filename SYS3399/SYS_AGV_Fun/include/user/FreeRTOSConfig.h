/*
 * FreeRTOS V202107.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Prevent C code being included by the IAR assembler. */
#ifndef __IASMARM__
	#include <stdint.h>
	extern uint32_t SystemCoreClock;
#endif

#define configUSE_PREEMPTION			1
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0 //1
#define configCPU_CLOCK_HZ				( SystemCoreClock )
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES			( 5 )
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 60 )
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 25*1024 ) )
#define configMAX_TASK_NAME_LEN			( 5 )
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			1
#define configUSE_MUTEXES				1
#define configQUEUE_REGISTRY_SIZE		8
#define configCHECK_FOR_STACK_OVERFLOW	0 //2
#define configUSE_RECURSIVE_MUTEXES		1
#define configUSE_MALLOC_FAILED_HOOK	0 //1
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	1
//#define configGENERATE_RUN_TIME_STATS	1
#define configUSE_QUEUE_SETS			1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 			0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

///* Software timer definitions. */
//#define configUSE_TIMERS				1
//#define configTIMER_TASK_PRIORITY		( 2 )
//#define configTIMER_QUEUE_LENGTH		5
//#define configTIMER_TASK_STACK_DEPTH	( 80 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define INCLUDE_eTaskGetState			1

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names - or at least those used in the unmodified vector table. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
#define vUARTInterruptHandler  vUARTInterruptHandler  //UART12_IRQHandler

/* Bump up the priority of recmuCONTROLLING_TASK_PRIORITY to prevent false
positive errors being reported considering the priority of other tasks in the
system. */
#define recmuCONTROLLING_TASK_PRIORITY ( configMAX_PRIORITIES - 1 )

/* The UDP port to use for incoming command inputs.  The outgoing port is
set to ( configUDP_CLI_PORT_NUMBER + 1 ). */
#define configUDP_CLI_PORT_NUMBER	5001

/* The size of the global output buffer that is available for use when there
are multiple command interpreters running at once (for example, one on a UART
and one on TCP/IP).  This is done to prevent an output buffer being defined by
each implementation - which would waste RAM.  In this case, there is only one
command interpreter running, and it has its own local output buffer, so the
global buffer is just set to be one byte long as it is not used and should not
take up unnecessary RAM. */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE 512

#define configCLI_BAUD_RATE 115200


//#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS 1
//#define portGET_RUN_TIME_COUNTER_VALUE 1

#define configINCLUDE_QUERY_HEAP_COMMAND 1
#define configINCLUDE_TRACE_RELATED_CLI_COMMANDS 0


/* memory related */
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configSUPPORT_STATIC_ALLOCATION 0

extern volatile uint32_t ulHighFrequencyTimerTicks ;
/* run status related */
#define configGENERATE_RUN_TIME_STATS 1
#define configUSE_TRACE_FACILITY 1
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()     (ulHighFrequencyTimerTicks = 0ul) 
#define portGET_RUN_TIME_COUNTER_VALUE()             ulHighFrequencyTimerTicks

/* soft timer realted */
#define configUSE_TIMERS 1
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES -1)
#define configTIMER_QUEUE_LENGTH 5
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE*2)

#endif /* FREERTOS_CONFIG_H */













