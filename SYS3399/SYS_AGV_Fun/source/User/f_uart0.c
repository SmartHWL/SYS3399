#include "f_uart0.h"

UART_PARAM_s  Uart;
UART_DATA_STR UartData;

uint8_t UartSlaveToMasterData[30] = {0};	//从机反馈数组


extern int16_t accel_data[3];
extern int16_t gyro_data[3];
extern short raw;
extern float temperature;
/*
累加和校验函数，输入为：
*CheckFrame：数据首地址
Length：数据长度
*/
uint8_t SummationCheck(uint8_t *CheckFrame, uint8_t Length){
  uint16_t sum = 0;
  while (Length--){
    sum += *(CheckFrame++);
  }
  return (uint8_t)sum&0x7f;  //取累加和后七位
}


/**********************************
   Uart数据接收
***********************************/
void CommDataRev(void)
{
	uint8_t i;
	i = UartData.RevNum;
	i++;
	if(i > (UART_MAX_LENGTH - 1))
		i = 0;

//	if(i == UartData.ReadNum)
//		return;			//缓存满

	UartData.RevBuf[UartData.RevNum] =  UART0->UARTDR_b.DATA;
	UartData.RevNum = i;
}

/**********************************
   Uart数据发送
   输入参数为：数据长度以及数组首地址
***********************************/
void UartSendDataToBuf(uint8_t length, uint8_t*cmd){
  uint8_t i;
  for (i=0;i<length;i++){
    UART0->UARTDR_b.DATA = cmd[i];
		while(UART0->UARTFR_b.TXFE==0); //等待数据发送完成
  }
}

/***********************************
		串口参数初始化
************************************/
void UartParamInit(void){
	UartData.RevNum 	      = 0;
	UartData.ReadNum        = 0;
	UartData.RevLenth       = 0;
	UartData.FrameLenth     = 0;
  UartData.UartFrameCount = 0;
}

/*****************************************
		串口接收数据帧解析  返回1为存在完整帧
******************************************/
uint8_t Look_test;
uint8_t McuUart0Read(uint8_t *cmd){  //cmd指向存放数据的数组
  uint8_t i = 0,j = 0;
	uint16_t CheckSum =0;
	uint8_t state = 0;
	uint8_t temp_rd;
	uint8_t count = 0;
  temp_rd = UartData.ReadNum;			//获取当前读取位置
	while(1)
	{
		if(temp_rd == UartData.RevNum)		//没有数据需要解析
		{
			return 0;	//执行至没有数据解析时返回
		}
		cmd[i] = UartData.RevBuf[temp_rd];		//需要解析的数据读入数组cmd
		temp_rd ++;		
		if(temp_rd > (UART_MAX_LENGTH-1))
		{
			temp_rd = 0;
		}
		i++;
    
		switch(state){	//不符合完整解析时则返回重新解析
			case 0:
				if(cmd[0]==0xFA){	       							//判断帧头
				 state=1;
				}
				else{
					i = 0;
					state = 0;
					UartData.ReadNum = temp_rd;					//不是合法数据,则等待定时器中断再次进入判断是否为帧头
				}
				break;
        
			case 1:												   //判断帧长 
	        if(cmd[1]==0)	{
						i = 0;
						state = 0;
						UartData.ReadNum = temp_rd;					//不是合法数据,则等待定时器中断再次进入判断是否为帧头
					}	
					else  state=2;
					break;

			default:
				break;
			}
    
			if(state == 2){				//截取完整一帧以及获得帧长
				if(cmd[i-1] == 0xf9){
					count++;     
				}
        
				if((cmd[1]+count) == i){   //(&&
					for(j=0;j<i-1;j++){
						CheckSum += cmd[j];
					}
          Look_test = CheckSum&0x7f;//查看通讯代码校验码，仅测试时用。

					if((CheckSum&0x7f) == cmd[i-1]){
						state = 3;
						UartData.RevLenth = i;         //获得未还原字节长度计数
					}
					if((CheckSum&0x7f)!=cmd[i-1]){
            i = 0;
            state = 0;
            UartData.ReadNum = temp_rd;					//不是合法数据,则等待定时器中断再次进入判断是否为帧头
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
        return 1;	  //执行至完整一帧数据时返回
		}
	}
}

/*数据处理，还原数据与帧长*/
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
  
  /*通讯异常检测部分*/
//  MotorErr.Communication_Err_Cnt++;
//  if(MotorErr.Communication_Err_Cnt > 12000){  //3s未收到广播信号，表明通讯异常
//    if(MotorErr.Err_Flag == 0)  MotorErr.Err_Flag |= COMMU_ERR;   //错误计数溢出，错误标记
//  }
  
  i = McuUart0Read(ReadMessage);  //处理接收数据截取一帧及帧长
  if(i == 0)  return;
  
  McuUart0deal(ReadMessage);  //还原数据以及实际帧长
  

}


/*****************************************
		传递参数至应用层
******************************************/
void UartSlaveResponseToMaster(uint16_t QueryNum)  //输入为查询码
{
  uint8_t CheckSum;

  if(QueryNum == 1){  //查询内部参数      
    UartSlaveToMasterData[0]  = 0xfa; // 帧头
    UartSlaveToMasterData[1]  = (3+UartData.UartSendAccelXLenth+UartData.UartSendAccelYLenth+UartData.UartSendAccelZLenth+
                                    UartData.UartSendGyroXLenth+UartData.UartSendGyroYLenth+UartData.UartSendGyroZLenth); // 数据长度
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
    UartSlaveToMasterData[UartSlaveToMasterData[1]-1] = CheckSum & 0x7F;  // 校验码
    UartSendDataToBuf(UartSlaveToMasterData[1], UartSlaveToMasterData);
      
    ClearUartSendData(); 
	}

}
/*****************************************
串口数据发送处理
******************************************/
uint8_t UartSendDataDeal(uint8_t *cmdptr, int16_t ptr)
{
    uint8_t lenth=0;
//    if(ptr==0)   return lenth;
    
    cmdptr[lenth] = (ptr & 0xFF00) >>8;    //高位数据
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
    
    cmdptr[lenth] = ptr & 0x00FF;       //低位数据
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
清除发送邮箱数据
******************************************/
void ClearUartSendData(void)
{
    for(uint8_t i=0;i<29;i++)
    {
        UartSlaveToMasterData[i]=0;
    }

}