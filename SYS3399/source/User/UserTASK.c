#include "UserTASK.h"

int16_t accel_data[3] ={0};
int16_t gyro_data[3]={0};
uint16_t u16SPItestData=0x55;
uint16_t u16SPIRecData=0;
void UserTASK(void* parameter){

	 strI2cQMIFun.pvI2CQMI6858C_Init();

	 while(1){

	 /*∂¡»°¡˘÷·Õ”¬›“« ˝æ›*/
     strI2cQMIFun.pvI2CQMI_GetData();

     //strI2cQMIFun.pvI2CQMI_Data_Process();
     accel_data[0] = qmi_get_date.accelx;
     accel_data[1] = qmi_get_date.accely;
     accel_data[2] = qmi_get_date.accelz;

     gyro_data[0] = qmi_get_date.gyrox;
     gyro_data[1] = qmi_get_date.gyroy;
     gyro_data[2] = qmi_get_date.gyroz;

		 SPI_CS_EN;
		 u16SPIRecData=SPI_DEV_SendByte(u16SPItestData);
		 SPI_CS_DIS;
		 
		 vTaskDelay(10);
	 }
}




