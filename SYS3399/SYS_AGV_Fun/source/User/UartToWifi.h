#ifndef __UART_WIFI_H_
#define __UART_WIFI_H_
#include "stdint.h"
#include "string.h"
#include "MowerFSM.h"
#include "UartToMotor.h"


typedef struct  
{	
	
	UART_BASIC_STR Basic;

} UARTWIFI_STR ;



typedef struct{
 void (*pvUartWifiInit)(UARTWIFI_STR *pstr);	
 void (*pvUartWifiLoad)(void);		
 void (*pvUartWifiDeal)(UART_BASIC_STR  *pstr);
}UARTWIFI_FUN_STR;

extern UARTWIFI_STR  		  UartWifi;  
extern UARTWIFI_FUN_STR   strUartWifiFun;


#endif


