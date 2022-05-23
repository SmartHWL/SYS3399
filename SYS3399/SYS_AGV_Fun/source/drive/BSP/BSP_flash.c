/**
  ******************************************************************************
  * @file    BSP_flash.c
  * @author  Silan - MC Team
  * @version 1.0.0
  * @date    2019/08/21
  * @brief   Flash Related Config Source File
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
	* -# 2019/08/21 | v1.0.0 | Lijianbo | Creat file.
	*
	******************************************************************************
  */

/*******************************************************************************
 * Include file
 ******************************************************************************/
#include "SC32F58128_iap.h"
#include "BSP_flash.h"
/*******************************************************************************
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
void Flash_For_Eeprom_Test(void);
/*******************************************************************************
 * Global function for the project
 ******************************************************************************/

/**
  * @brief  Flash Initialization.
*/
void BSP_FlashInit(void){
  Flash_For_Eeprom_Test();
}

void Flash_For_Eeprom_Test(void){
	uint8_t buff[6] = {223, 78, 33, 33, 33, 60};
  uint8_t r_buff[8] = {0,};
  
  InitEEPROM(EEPROM_START_ADDR, EEPROM_SIZE);

  WriteByteEEPROM(512 - 1, buff + 5);
  WriteByteEEPROM(512 + 0, buff);
  WriteByteEEPROM(512 + 1, buff + 1);
  WriteByteEEPROM(512 + 2, buff + 2);
  WriteByteEEPROM(512 + 3, buff + 3);
  WriteByteEEPROM(512 + 4, buff + 4);

  *r_buff = ReadByteEEPROM(512 + 0);
  *(r_buff + 1) = ReadByteEEPROM(512 + 1);
  *(r_buff + 2) = ReadByteEEPROM(512 + 2);
  *(r_buff + 3) = ReadByteEEPROM(512 + 3);
  *(r_buff + 4) = ReadByteEEPROM(512 + 4);
  *(r_buff + 5) = ReadByteEEPROM(512 - 1);
}
