#include "f_uart0.h"

UART_PARAM_s  Uart;
UART_DATA_STR UartData;

uint8_t UartSlaveToMasterData[30] = {0};	//�ӻ���������


extern int16_t accel_data[3];
extern int16_t gyro_data[3];
extern short raw;
extern float temperature;
/*
�ۼӺ�У�麯��������Ϊ��
*CheckFrame�������׵�ַ
Length�����ݳ���
*/
uint8_t SummationCheck(uint8_t *CheckFrame, uint8_t Length){
  uint16_t sum = 0;
  while (Length--){
    sum += *(CheckFrame++);
  }
  return (uint8_t)sum&0x7f;  //ȡ�ۼӺͺ���λ
}


/**********************************
   Uart���ݽ���
***********************************/
void CommDataRev(void)
{
	uint8_t i;
	i = UartData.RevNum;
	i++;
	if(i > (UART_MAX_LENGTH - 1))
		i = 0;

//	if(i == UartData.ReadNum)
//		return;			//������

	UartData.RevBuf[UartData.RevNum] =  UART0->UARTDR_b.DATA;
	UartData.RevNum = i;
}

/**********************************
   Uart���ݷ���
   �������Ϊ�����ݳ����Լ������׵�ַ
***********************************/
void UartSendDataToBuf(uint8_t length, uint8_t*cmd){
  uint8_t i;
  for (i=0;i<length;i++){
    UART0->UARTDR_b.DATA = cmd[i];
		while(UART0->UARTFR_b.TXFE==0); //�ȴ����ݷ������
  }
}

/***********************************
		���ڲ�����ʼ��
************************************/
void UartParamInit(void){
	UartData.RevNum 	      = 0;
	UartData.ReadNum        = 0;
	UartData.RevLenth       = 0;
	UartData.FrameLenth     = 0;
  UartData.UartFrameCount = 0;
}

/*****************************************
		���ڽ�������֡����  ����1Ϊ��������֡
******************************************/
uint8_t Look_test;
uint8_t McuUart0Read(uint8_t *cmd){  //cmdָ�������ݵ�����
  uint8_t i = 0,j = 0;
	uint16_t CheckSum =0;
	uint8_t state = 0;
	uint8_t temp_rd;
	uint8_t count = 0;
  temp_rd = UartData.ReadNum;			//��ȡ��ǰ��ȡλ��
	while(1)
	{
		if(temp_rd == UartData.RevNum)		//û��������Ҫ����
		{
			return 0;	//ִ����û�����ݽ���ʱ����
		}
		cmd[i] = UartData.RevBuf[temp_rd];		//��Ҫ���������ݶ�������cmd
		temp_rd ++;		
		if(temp_rd > (UART_MAX_LENGTH-1))
		{
			temp_rd = 0;
		}
		i++;
    
		switch(state){	//��������������ʱ�򷵻����½���
			case 0:
				if(cmd[0]==0xFA){	       							//�ж�֡ͷ
				 state=1;
				}
				else{
					i = 0;
					state = 0;
					UartData.ReadNum = temp_rd;					//���ǺϷ�����,��ȴ���ʱ���ж��ٴν����ж��Ƿ�Ϊ֡ͷ
				}
				break;
        
			case 1:												   //�ж�֡�� 
	        if(cmd[1]==0)	{
						i = 0;
						state = 0;
						UartData.ReadNum = temp_rd;					//���ǺϷ�����,��ȴ���ʱ���ж��ٴν����ж��Ƿ�Ϊ֡ͷ
					}	
					else  state=2;
					break;

			default:
				break;
			}
    
			if(state == 2){				//��ȡ����һ֡�Լ����֡��
				if(cmd[i-1] == 0xf9){
					count++;     
				}
        
				if((cmd[1]+count) == i){   //(&&
					for(j=0;j<i-1;j++){
						CheckSum += cmd[j];
					}
          Look_test = CheckSum&0x7f;//�鿴ͨѶ����У���룬������ʱ�á�

					if((CheckSum&0x7f) == cmd[i-1]){
						state = 3;
						UartData.RevLenth = i;         //���δ��ԭ�ֽڳ��ȼ���
					}
					if((CheckSum&0x7f)!=cmd[i-1]){
            i = 0;
            state = 0;
            UartData.ReadNum = temp_rd;					//���ǺϷ�����,��ȴ���ʱ���ж��ٴν����ж��Ƿ�Ϊ֡ͷ
						CheckSum = 0;
          }
				}
			}
			
			if(state == 3){
        UartData.ReadNum = temp_rd;
        UartData.UartFrameCount++;
        CheckSum = 0 ;		
        i = 0;			
        state = 0;									
        return 1;	  //ִ��������һ֡����ʱ����
		}
	}
}

/*���ݴ�����ԭ������֡��*/
void McuUart0deal(uint8_t *cmdptr)
{
	uint8_t i,j=0,a=0;
  
  for(i=0;i<UartData.RevLenth;i++)
  {
    if(cmdptr[j]==0xf9)
      {
        UartData.RevFrame[i]=cmdptr[j]+cmdptr[j+1];
        j=j+2;
        a++;
      }
    else if(cmdptr[j]!=0xf9)
      {
        UartData.RevFrame[i]=cmdptr[j];
        j++;
      }
  }
  
	UartData.FrameLenth=UartData.RevLenth-a;
  
  for(i=UartData.RevLenth-a;i<20;i++)
  {
    UartData.RevFrame[i]=0;
  }
}


uint8_t ReadMessage[30];
void UartRevMessageDeal(void)
{
  uint8_t i = 0;
  
  /*ͨѶ�쳣��ⲿ��*/
//  MotorErr.Communication_Err_Cnt++;
//  if(MotorErr.Communication_Err_Cnt > 12000){  //3sδ�յ��㲥�źţ�����ͨѶ�쳣
//    if(MotorErr.Err_Flag == 0)  MotorErr.Err_Flag |= COMMU_ERR;   //������������������
//  }
  
  i = McuUart0Read(ReadMessage);  //����������ݽ�ȡһ֡��֡��
  if(i == 0)  return;
  
  McuUart0deal(ReadMessage);  //��ԭ�����Լ�ʵ��֡��
  

}


/*****************************************
		���ݲ�����Ӧ�ò�
******************************************/
void UartSlaveResponseToMaster(uint16_t QueryNum)  //����Ϊ��ѯ��
{
  uint8_t CheckSum;

  if(QueryNum == 1){  //��ѯ�ڲ�����      
    UartSlaveToMasterData[0]  = 0xfa; // ֡ͷ
    UartSlaveToMasterData[1]  = (3+UartData.UartSendAccelXLenth+UartData.UartSendAccelYLenth+UartData.UartSendAccelZLenth+
                                    UartData.UartSendGyroXLenth+UartData.UartSendGyroYLenth+UartData.UartSendGyroZLenth); // ���ݳ���
    for(uint8_t i=0;i<UartData.UartSendAccelXLenth;i++)                             
      {UartSlaveToMasterData[2+i]=UartData.UartSendAccelXData[i];}              //ACC X
    for(uint8_t i=0;i<UartData.UartSendAccelYLenth;i++)
      {UartSlaveToMasterData[2+UartData.UartSendAccelXLenth+i]=UartData.UartSendAccelYData[i];}       //ACC Y
    for(uint8_t i=0;i<UartData.UartSendAccelZLenth;i++)
      {UartSlaveToMasterData[2+UartData.UartSendAccelXLenth+UartData.UartSendAccelYLenth+i]=UartData.UartSendAccelZData[i];}   //ACC Z
      
    for(uint8_t i=0;i<UartData.UartSendGyroXLenth;i++)
      {UartSlaveToMasterData[2+UartData.UartSendAccelXLenth+UartData.UartSendAccelYLenth+UartData.UartSendAccelZLenth+i]=UartData.UartSendGyroXData[i];}   //Gyro X      
    for(uint8_t i=0;i<UartData.UartSendGyroYLenth;i++)
      {UartSlaveToMasterData[2+UartData.UartSendAccelXLenth+UartData.UartSendAccelYLenth+UartData.UartSendAccelZLenth+
                                UartData.UartSendGyroXLenth+i]=UartData.UartSendGyroYData[i];}   //Gyro Y
   for(uint8_t i=0;i<UartData.UartSendGyroZLenth;i++)
      {UartSlaveToMasterData[2+UartData.UartSendAccelXLenth+UartData.UartSendAccelYLenth+UartData.UartSendAccelZLenth+
                                UartData.UartSendGyroXLenth+UartData.UartSendGyroYLenth+i]=UartData.UartSendGyroZData[i];}   //Gyro Y      
      
    CheckSum = SummationCheck(UartSlaveToMasterData, UartSlaveToMasterData[1]-1);
    UartSlaveToMasterData[UartSlaveToMasterData[1]-1] = CheckSum & 0x7F;  // У����
    UartSendDataToBuf(UartSlaveToMasterData[1], UartSlaveToMasterData);
      
    ClearUartSendData(); 
	}

}
/*****************************************
�������ݷ��ʹ���
******************************************/
uint8_t UartSendDataDeal(uint8_t *cmdptr, int16_t ptr)
{
    uint8_t lenth=0;
//    if(ptr==0)   return lenth;
    
    cmdptr[lenth] = (ptr & 0xFF00) >>8;    //��λ����
    lenth++;
    if(cmdptr[lenth-1] == 0xFA)
    {
        cmdptr[lenth-1] = 0xF9;
        cmdptr[lenth] = 0x01;
            lenth++;
    }
    else if(cmdptr[lenth-1] == 0xF9)
    {
        cmdptr[lenth-1] = 0xF9;
        cmdptr[lenth] = 0x00;
            lenth++;
    }
    
    cmdptr[lenth] = ptr & 0x00FF;       //��λ����
       lenth++;    
    if(cmdptr[lenth-1] == 0xFA)
    {
        cmdptr[lenth-1] = 0xF9;
        cmdptr[lenth] = 0x01;
            lenth++;
    }
    else if(cmdptr[lenth-1] == 0xF9)
    {
        cmdptr[lenth-1] = 0xF9;
        cmdptr[lenth] = 0x00;
            lenth++;
    }
    return lenth;
}

/*****************************************
���������������
******************************************/
void ClearUartSendData(void)
{
    for(uint8_t i=0;i<29;i++)
    {
        UartSlaveToMasterData[i]=0;
    }

}