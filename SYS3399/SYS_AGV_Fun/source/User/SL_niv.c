/**
  ******************************************************************************
  * @file    SL_niv.c
  * @author  Silan - MC Team
  * @version 1.0.0
  * @date    2017/07/19
  * @brief   Interrupt Service Routine Source File
  * @details None
	* @note    None
  ******************************************************************************
  * @attention
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
  * AS A RESULT, SILAN MICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
  * CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
  * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
	*
  ******************************************************************************
	* Change History
	* -# 2017/07/19 | v1.0.0 | Wangjianlong | Creat file.
	*
	******************************************************************************
  */

/*******************************************************************************
 * Include file
 ******************************************************************************/
#include "SC32F58128.h"
#include "sl_niv.h"
#include "SC32F58128_timer.h"
#include "BSP_dma.h"
#include "BSP_uart.h"
#include "timer4Tick.h"
#include "osc.h"
#include "BSP_gpio.h"
#include "BSP_iic.h"

#include "Boundary.h"
#include "UartToMotor.h"
#include "UartToPanel.h"
#include "UartToWifi.h"

/******************************************************************************
 * Global data for the project
 ******************************************************************************/


/*******************************************************************************
 * Local data for the current file
 ******************************************************************************/


/*******************************************************************************
 * Local prototypes function
 ******************************************************************************/


/*******************************************************************************
 * Local function for the current file
 ******************************************************************************/


/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief  This function handles NMI exception.
* @param  None
* @retval None
*/
void NMI_Handler(void)
{
}

/**
* @brief  This function handles Hard Fault exception.
* @param  None
* @retval None
*/




void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	
	
//	__asm mov  pc,lr
  while (1)
  {
    /* Add user code */
  }
}

/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None
*/
void SysTick_Handler_notuse(void)
{
  g_u32SysTickNumb++;
}


/**
* @brief  This function handles SVC Handler.
* @param  None
* @retval None
*/
void SVC_Handler_notuse(void)
{

}


/**
* @brief  This function handles PendSV Handler.
* @param  None
* @retval None
*/
void PendSV_Handler_notuse(void)
{

}

/******************************************************************************/
/*                 Peripherals Interrupt Handlers                             */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file .                                               */
/******************************************************************************/
/**
* @brief  This function handles UART0 receive interrupt request.
* @param  None
* @retval None
*/
void UART0_RX_IRQHandler(void)
{


}

/**
* @brief  This function handles UART0 interrupt request.
* @param  None
* @retval None
*/
void UART0_IRQHandler(void)
{
  volatile uint8_t tt;
	if(UART0->UARTMIS_b.Receive_MI == 1){  //�����жϽ��յ�������
    //CommDataRev();
		UART0->UARTICR_b.C_Receive_IM = 1;  //��������жϱ�־
	}
  
	if(UART0->UARTMIS_b.Transmit_MI == 1){ //�����жϽ��յ�������
//    CommDataSend();
		UART0->UARTICR_b.C_Transmit_IM = 1; //��������жϱ�־
	}
  
  if(UART0->UARTMIS_b.OE_MI ==1){
    tt = UART0->UARTDR_b.DATA;
    UART0->UARTICR = 0xffff;
    UART0->UARTDR  = 0;
  }
	
}

/**
* @brief  This function handles UART1&2 interrupt requests.
* @param  None
* @retval None
*/
void UART12_IRQHandler(void)
{

	
}

/**
* @brief  This function handles SSP0&1 interrupt requests.
* @param  None
* @retval None
*/
void SSP01_IRQHandler(void)
{

}

/**
* @brief  This function handles SSP2&3 interrupt requests.
* @param  None
* @retval None
*/
void SSP23_IIC_IRQHandler(void)
{
		static	uint8_t NackNum = 0;
	uint8_t temp8;
	
	temp8 = (uint8_t) I2C->SR_b.STAT;
	
	switch (temp8)
	{
		case 0x00:	/* ���ߴ��� */
			I2C_deal(0,2,2);
			break;
		
		case 0x08:	/* I2CSTA = 0x08;�Ѵ��Ϳ�ʼλ������ӻ���ַ������ACK */
            if(IICInfo.CmdRead == I2C_Read)
            {
                strI2cMsg.status = I2C_MSG_STATUS_READ_BUSY;
            }
            else
            {
                strI2cMsg.status = I2C_MSG_STATUS_WRITE_BUSY;
            }
			I2C->DATA_b.DATA = IICInfo.DevIdRaw ;            /* ��������ַ */
			break;
		
		case 0x10:	/* �����ظ���ʼλ */
			/* ���ʹӻ���ַ����������Ϊ����/���� */
			I2C->DATA_b.DATA = IICInfo.DevId;
			I2C_deal(2,0,2);
			break;
		
		case 0x18:	/* �����豸��ַ��ACK�����ж� */
//            IICInfo.TxCnt++;   /* �����ߵ�ַ */
            I2C->DATA_b.DATA = IICInfo.TxData[IICInfo.TxCnt];
            IICInfo.TxCnt++;
            I2C_deal(0,0,2);
			break;
		
		case 0x20:	/* ������ӻ���ַ��������Ϊ���ͣ�δ���յ�ACK */
			I2C_deal(1,0,2); /* �����ظ���ʼλ */
			break;
		
		case 0x28:	/* I2CDATA�����ѷ��ͣ����յ�ACK */
            if(IICInfo.CmdRead == I2C_Read)
            {
                if(IICInfo.TxCnt >= 2)
                {
                    IICInfo.DevId = IICInfo.DevIdRaw | I2C_Read;
                    I2C_deal(1,0,2);
                } else 
                {
                    /* ����û�����꣬�����µ����ݣ�����ACK */
                    I2C->DATA_b.DATA = IICInfo.TxData[IICInfo.TxCnt];
                    IICInfo.TxCnt++;
                    I2C_deal(0,0,2);        
                }
            } 
            else 
            {
                if (IICInfo.TxCnt == IICInfo.TxMax)
                {
                    /* �����ѷ������,������ֹ��־��STO�Զ���0 */
                    I2C_deal(0,1,2);
                    strI2cMsg.status = I2C_MSG_STATUS_RW_OK;
                }
                else
                {
                    /* ����û�����꣬�����µ����ݣ�����ACK */
                    I2C->DATA_b.DATA = IICInfo.TxData[IICInfo.TxCnt];
                    IICInfo.TxCnt++;
                    I2C_deal(0,0,2);
                }
            }
			break;
		
		case 0x30: /* I2CDATA�����ѷ���, δ���յ�ACK */
			I2C_deal(0,1,2); /* ����ֹͣ��־��STO�Զ���0 */
			break;
		
		case 0x38: // �ٲö�ʧ
			I2C_deal(0,0,2);//I2C�����ͷ�
			strI2cMsg.status = I2C_MSG_STATUS_IDLE;
			break;
		
		case 0x40:	/* I2CSTA = 0x40H;������ӻ���ַ��������Ϊ���գ��յ�ACK */
			/* ������һ�����ݣ�����ACK */
			if (IICInfo.RxCnt == IICInfo.RxMax)
			{
				I2C_deal(0,0,0);
			}
			else
			{
				I2C_deal(0,0,1); /* ��֪�ӻ���һ�����ݹ��� */
			}
			break;
		
		case 0x48:	/* I2CSTA = 0x40H;������ӻ���ַ��������Ϊ���գ�δ�յ�ACK */
			I2C_deal(1,0,2); /* �����߿���ʱ���ٴη�����ʼλ */
			break;
		
		case 0x50:	/* I2CSTA = 0x50�����յ����ݲ�����ACK */
			/* �����ȡ������ */
			IICInfo.RxData[IICInfo.RxCnt] = (uint8_t)I2C->DATA_b.DATA;
			IICInfo.RxCnt++;
			if (IICInfo.RxCnt == IICInfo.RxMax )//- 1
			{
				I2C_deal(0,0,0);
			}
			else
			{
				I2C_deal(0,0,1); /* ��֪�ӻ���һ�����ݹ��� */
			}
			break;
		
		case 0x58:	/* I2CSTA = 0x58;���յ����ݣ�δ����ACK */
			IICInfo.RxData[IICInfo.RxCnt] = (uint8_t)I2C->DATA_b.DATA;
			I2C_deal(0,1,2); /* ������ֹ��־��STO�Զ���0 */
//			for (i = 0; i <= IICInfo.RxCnt; i++)
//			{
//				strI2cMsg.u8RxBuffer[i] = IICInfo.RxData[i];
//			}
			strI2cMsg.status = I2C_MSG_STATUS_RW_OK;
			break;	
		default:
			break;		
	}  
	I2C_ClearSI;
}
/**
* @brief  This function handles CAN interrupt request.
* @param  None
* @retval None
*/
void CAN_IRQHandler(void)
{

}
 /**
* @brief  This function handles ECAP0 interrupt request.
* @param  None
* @retval None
*/
void ECAP0_IRQHandler(void)
{


}

/**
* @brief  This function handles ECAP1 interrupt request.
* @param  None
* @retval None
*/
void ECAP1_IRQHandler(void)
{


}
/**
* @brief  This function handles ECAP2 interrupt request.
* @param  None
* @retval None
*/
void ECAP2_IRQHandler(void)
{

}

/**
* @brief  This function handles EQPE0 interrupt request.
* @param  None
* @retval None
*/
void EQEP0_IRQHandler(void)
{
}


/**
* @brief  This function handles EQEP1 interrupt request.
* @param  None
* @retval None
*/
void EQEP1_IRQHandler(void)
{
}

/**
* @brief  This function handles GPIO interrupt request.
* @param  None
* @retval None
*/
void GPIO_IRQHandler(void)
{

}


/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void DMA_IRQHandler(void)
{
//	/* DMA Channel 0 Done ISR */
//  if(IS_DMA0_CHNL_APINT)
//  {
//    CLR_DMA0_INT_APFLG();
//    SendNotSendData2UartTx();
//  }
}


/**
* @brief  This function handles COPROC interrupt request.
* @param  None
* @retval None
*/
void COPROC_IRQHandler(void)
{

}


/**
* @brief  This function handles sysrem interrupt request.
* @param  None
* @retval None
*/
void SYS_IRQHandler(void)
{

}


/**
* @brief  This function handles reserved interrupt request.
* @param  None
* @retval None
*/
void rsv1_IRQHandler(void)
{

}

volatile uint32_t ulHighFrequencyTimerTicks ;
/**
* @brief  This function handles TIMER0 interrupt request.
* @param  None
* @retval None
*/
void TIMER0_IRQHandler(void)
{
//  uint32_t u32Tmp = 0;

//  /* Kick WDT */
//  u32Tmp = WDT0->WDOGLOAD;  // Read Load Value
//  WDT0->WDOGLOAD = u32Tmp;  // Kick Dog

//  WAVE_FillWaveBuff();
  ulHighFrequencyTimerTicks++;
  TIMER[0]->CTRL_b.OV = 0;
}

/**
* @brief  This function handles TIMER1 interrupt request.
* @param  None
* @retval None
*/
void TIMER1_IRQHandler(void)
{


}



/**
* @brief  This function handles TIMER2&3&4 interrupt requests.
* @param  None
* @retval None
*/
void TIMER234_IRQHandler(void)
{

}

/**
* @brief  This function handles ADC interrupt 1 request.
* @param  None
* @retval None
					________
				 |				|
				 |				|
_________|        |					 _________ 
									|					|
									|					|
									|_________| 
	

									 _________ 
									|					|
									|					|
________				  |					|________
				|				  |
				|				  |					
				|_________| 

*/
void ADC1_IRQHandler(void)
{
	
	   if(ADC->ADCINTFLG_b.ADCINT1)
    {
			
//			strBoundaryFun.pvBoundary_ADCIsr(&Boundary);
			ADC->ADCINTFLGCLR_b.ADCINT1=1;	
     }

}
/**
* @brief  This function handles ADC interrupt 2 request.
* @param  None
* @retval None
*/
void ADC2_IRQHandler(void)
{

}


/**
* @brief  This function handles ADC interrupt 3 request.
* @param  None
* @retval None
*/
void ADC3_IRQHandler(void)
{


}

/**
* @brief  This function handles all ADC interrupt requests.
* @param  None
* @retval None
*/
void ADC_IRQHandler(void)
{


}

/**
* @brief  This function handles PWM0 interrupt request.
* @param  None
* @retval None
*/
void PWM0_IRQHandler(void)
{

}

/**
* @brief  This function handles PWM3 interrupt request.
* @param  None
* @retval None
*/
void PWM3_IRQHandler(void)
{


}

/**
* @brief  This function handles PWM4 interrupt request.
* @param  None
* @retval None
*/
void PWM4_IRQHandler(void)
{


}

/**
* @brief  This function handles PWM7 interrupt request.
* @param  None
* @retval None
*/
void PWM7_IRQHandler(void)
{


}

/**
* @brief  This function handles all PWM interrupt requests.
* @param  None
* @retval None
*/
void PWM1256_IRQHandler(void)
{


}

/**
* @brief  This function handles ERU0 interrupt request.
* @param  None
* @retval None
*/
void ERU0_IRQHandler(void)
{


}

/**
* @brief  This function handles ERU1 interrupt request.
* @param  None
* @retval None
*/
void ERU1_IRQHandler(void)
{


}

/**
* @brief  This function handles ERU2 interrupt request.
* @param  None
* @retval None
*/
void ERU2_IRQHandler(void)
{


}

/**
* @brief  This function handles ERU3 interrupt request.
* @param  None
* @retval None
*/
void ERU3_IRQHandler(void)
{


}

/*---------------------------------- End of the file ---------------------------------*/
