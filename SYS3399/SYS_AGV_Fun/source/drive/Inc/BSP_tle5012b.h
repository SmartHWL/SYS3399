/**************************************************************************//**
 * @file     BSP_tle5012b.h
 * @brief    SPI Related Config Header File
 * @version  V1.00
 * @date     18. September 2018
 *
 * @note
 *
 ******************************************************************************/
#ifndef __BSP_TLE5012B_H__
#define __BSP_TLE5012B_H__
/*
TLE5012B_E1000 接线说明:

1:NC
2:SCK <= 120R => stm32f103 PA5
3:CSQ (CS) <= 120R => stm32f103 PB0
4:DATA <= 120R => stm32f103 PA6
  DATA <= 120R => stm32f103 PA7
  DATA <= 2.2K => Vdd 3.3V
  PS:因是三线 SPI , MOSI , MISO 并线使用

5:NC
6:Vdd 3.3V
7:GND
8:NC
*/
/********************************************************************************
 * Include section
 *******************************************************************************/
#include "stdint.h"
/********************************************************************************
 * Constants and macros
 *******************************************************************************/
 #define CS SPI                        GPIO_19
 
/* command for TLE5012 */
//  RW  cfg/def        ADDR  Number of Data Words
//b'1____0000____0____000010_____0001
#define READ_STATUS           0x8001			// b'1_0000_0_000000_0000 0x8000
#define READ_ANGLE_VALUE      0x8021			// b'1_0000_0_000010_0000 0x8020
#define READ_SPEED_VALUE      0x8031			// b'1_0000_0_000011_0000 0x8030 

#define WRITE_MODE1_VALUE     0x5060      // b'0_1010_0_000110_0001
#define MODE1_VALUE           0x0001                        
                                                            
#define WRITE_MODE2_VALUE     0x5081      // b'0_1010_0_001000_0001 0x5081
#define MODE2_VALUE           0x0809                        
                                                            
#define WRITE_MODE3_VALUE     0x5091      // b'0_1010_0_001001_0001
#define MODE3_VALUE           0x0000                        
                                                            
#define WRITE_MODE4_VALUE     0x50E0      // b'0_1010_0_001110_0001
#define MODE4_VALUE           0x0098      // 9bit 512

#define WRITE_IFAB_VALUE      0x50B1
#define IFAB_VALUE            0x000D
/* Functionality mode */
#define REFERESH_ANGLE      0

/********************************************************************************
 * Typedefs, structs and enums
 *******************************************************************************/
/** @defgroup SPI_Comm_status
  * @{
  */
typedef struct AVAL_STRUCT{
  int16_t  ANG_VAL : 15;
  uint16_t RD_AV   : 1;
}AVAL_STRUCT_s;
/**
  * @}
  */
/********************************************************************************
* Variables
*******************************************************************************/


/********************************************************************************
* prototypes of the functions
*******************************************************************************/
void SPI5012B_Init(void);
uint16_t ReadAngle(void);
uint16_t ReadSpeed(void);
uint16_t ReadValue(uint16_t u16Value);
uint16_t TlE5012W_Reg(uint16_t Reg_CMD, uint16_t Reg_Data);
/*---------------------------------- End of the file ---------------------------------*/
#endif /* __BSP_TLE5012B_H__ */

