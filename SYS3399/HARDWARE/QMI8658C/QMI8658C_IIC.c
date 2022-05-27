#include "qmi8658c_iic.h"
#include "BSP_iic.h"

/* 结构体声明 */
QMI_DATA qmi_get_date;
QUATERnion quaternion;
MQI_DATE_ZEROCHECK mqi_date_zerocheck;


/* 函数声明 */
void vI2CQMI6858C_Init(void);
void vI2CQMI_GetData(void);
//void vI2CQMI_Data_Process(void);

/* 函数指针声明 */
I2CQMI_FUN_STR  strI2cQMIFun=
{
	&vI2CQMI6858C_Init,
	&vI2CQMI_GetData,
//	&vI2CQMI_Data_Process,
};


/************************************************* 
 Function:    vI2CQMI_Init
 Description: QMI6858C陀螺仪初始化
 Input:      
 Output:      
 Return: 
 Others: 
*************************************************/
void vI2CQMI6858C_Init()
{
	BSP_I2cInit();
	while(strI2cMsg.status != I2C_MSG_STATUS_IDLE);
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL1_ADD,0x20);
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL2_ADD,0x24);//加速度计设置 满量程为8g  输出数据速率470hz
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL3_ADD,0x64);//陀螺仪设置 满量程为1024dps，输出数据速率为470hz   (X/32768*1024*0.0174532925)
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL5_ADD,0x77);//传感器过滤器  启用加速度计和陀螺仪的低通滤波 ODR的13.37%
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL7_ADD,0x00);
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL7_ADD,0x03);//启动加速度计和陀螺仪
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
}

/************************************************* 
 Function:    u16Average
 Description: 取中间值求平均值
 Input:      *a：待求数组；num：待求数个数； n:是否带绝对值取平均值
             为计算与0的偏差 选择带绝对值取平均值
 Output:      
 Return:     return：平均值
 Others: 
*************************************************/
static int_least16_t u16Average(int_least16_t *a,uint8_t num,uint8_t n)
{
	uint8_t i=0,j=0,z=0;
	uint8_t nums;
	int_least32_t sum=0;
	int_least16_t average=0,temp;
	nums=num-((num/4)<<1);
	//冒泡排序算法：进行 n-1 轮比较
  for(i=0; i<num-1; i++)
	{
		for(j=0; j<num-1-i; j++)
		{
			if(a[j] > a[j+1])
			{
				temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			}
    }
  }
	//排序后取中间1/2数据进行平均值计算
	for(z=num/4;z<(num-num/4);z++)
	{
		if(n)
			sum=sum+abs(a[z]);
		else
			sum=sum+a[z];			
	}
	average=sum/nums;
	return average;
}

/************************************************* 
 Function:    u16zero_check
 Description: 去零偏
 Input:      adr：被去零偏的值 ；adv：初始值的平均值
 Output:      
 Return:     return：去除零偏后的值
 Others: 
*************************************************/
static int_least16_t u16zero_check(int_least16_t buff,int_least16_t adv)  
{
	if(buff&0x8000)     //判断数值是正数还是负数
		return (buff+adv);
	else 
		return (buff-adv);
	
}

/************************************************* 
 Function:    vQMI_GetnData
 Description: 采集num组数据
 Input:      
 Output:      
 Return:     get_flag：num组数据是否采集完成
 Others: 
*************************************************/
static uint8_t vQMI_GetnData(uint8_t num)
{    
    static uint8_t i	=	0;
	static uint8_t j	=	0;
	uint8_t finish_flag;
	static uint8_t get_flag	=	0;
	static uint16_t mqi_buff[12];
    
	if(i<num)
	{
		if(get_flag)
		{
			j = 0;
			get_flag = 0;
		}
		if(j<=11)
		{
			vRw_IIC(RW_QMI8658_READ,QMI8658C_ADDress,QMI_AX_L_ADD+j,0);
		}					
		else
		{
			get_flag = 1;
			//采集到的数据去零偏			
			mqi_date_zerocheck.ax[i]=u16zero_check((mqi_buff[1]<<8)|mqi_buff[0],mqi_date_zerocheck.ax_adv);
			mqi_date_zerocheck.ay[i]=u16zero_check((mqi_buff[3]<<8)|mqi_buff[2],mqi_date_zerocheck.ay_adv);
//			mqi_date_zerocheck.az[i]=(mqi_buff[5]<<8)|mqi_buff[4];//平放不动时az不为0 不需要去零偏
//			mqi_date_zerocheck.az[i]=u16zero_check((mqi_buff[5]<<8)|mqi_buff[4],mqi_date_zerocheck.az_adv);  //修改偏置
			mqi_date_zerocheck.az[i]=u16zero_check((mqi_buff[5]<<8)|mqi_buff[4],4*mqi_date_zerocheck.az_adv);  //修改偏置
            
			mqi_date_zerocheck.gx[i]=u16zero_check((mqi_buff[7]<<8)|mqi_buff[6],mqi_date_zerocheck.gx_adv);
			mqi_date_zerocheck.gy[i]=u16zero_check((mqi_buff[9]<<8)|mqi_buff[8],mqi_date_zerocheck.gy_adv);
			mqi_date_zerocheck.gz[i]=u16zero_check((mqi_buff[11]<<8)|mqi_buff[10],mqi_date_zerocheck.gz_adv);
			i++;
		}
		if(I2C_MSG_STATUS_IDLE == strI2cMsg.status)
		{
			mqi_buff[j]=IICInfo.RxData[0];
			j++;
		}		
		finish_flag=0;
	}
	else
	{
		i=0;
		finish_flag=1;
	}
	return finish_flag;
}

/************************************************* 
 Function:    vI2CQMI_GetData
 Description: 获取陀螺仪数据
 Input:   
 Output:      
 Return: 
 Others: 
*************************************************/
void vI2CQMI_GetData()
{
	static uint8_t i=0;
	static uint8_t j=0;
	static uint8_t zerodata_flag;
	if(!zerodata_flag)  
	{	
		if(vQMI_GetnData(QMI_CHECKLEN))  //启动时平放不动，获取初始的多个值计算零偏
		{
			mqi_date_zerocheck.ax_adv=u16Average(mqi_date_zerocheck.ax,QMI_CHECKLEN,1);
			mqi_date_zerocheck.ay_adv=u16Average(mqi_date_zerocheck.ay,QMI_CHECKLEN,1);
			mqi_date_zerocheck.az_adv=u16Average(mqi_date_zerocheck.az,QMI_CHECKLEN,1);
			mqi_date_zerocheck.gx_adv=u16Average(mqi_date_zerocheck.gx,QMI_CHECKLEN,1);
			mqi_date_zerocheck.gy_adv=u16Average(mqi_date_zerocheck.gy,QMI_CHECKLEN,1);
			mqi_date_zerocheck.gz_adv=u16Average(mqi_date_zerocheck.gz,QMI_CHECKLEN,1);
			zerodata_flag=1;
		}
	}
	else if(zerodata_flag)
	{
		if(vQMI_GetnData(QMI_SAMLEN))
		{
			qmi_get_date.accelx=u16Average(mqi_date_zerocheck.ax,QMI_SAMLEN,0);
		  qmi_get_date.accely=u16Average(mqi_date_zerocheck.ay,QMI_SAMLEN,0);
	  	qmi_get_date.accelz=u16Average(mqi_date_zerocheck.az,QMI_SAMLEN,0);
	  	qmi_get_date.gyrox=u16Average(mqi_date_zerocheck.gx,QMI_SAMLEN,0);
	  	qmi_get_date.gyroy=u16Average(mqi_date_zerocheck.gy,QMI_SAMLEN,0);
	  	qmi_get_date.gyroz=u16Average(mqi_date_zerocheck.gz,QMI_SAMLEN,0);			
  	}
	}
}

///************************************************* 
// Function:    vI2CQMI_Data_process
// Description: 陀螺仪数据处理
// Input:   
// Output:      
// Return: 
// Others: 
//*************************************************/
//void vI2CQMI_Data_Process()
//{
//	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;//四元数的元素，代表估计方向
//	float exInt=0.0f,eyInt=0.0f,ezInt=0.0f;//按比例缩小积分误差
//	float norm;
//	float ax,ay,az;
//	float gx,gy,gz;
//	float q0temp,q1temp,q2temp,q3temp;//四元数暂存变量，求解微分方程时要用
//	float vx,vy,vz;
//	float ex,ey,ez;
//	
//	ax=(float)qmi_get_date.accelx;
//	ay=(float)qmi_get_date.accely;
//	az=(float)qmi_get_date.accelz;
//	
//	gx=(float)qmi_get_date.gyrox;
//	gy=(float)qmi_get_date.gyroy;
//	gz=(float)qmi_get_date.gyroz;
////	if(ax*ay*az==0) return;
//	norm=sqrt(ax*ax+ay*ay+az*az);
//	
//	//单位化
//	ax=ax/norm;
//	ay=ay/norm;
//	az=az/norm;
//	
//	//估计方向的重力
//	vx=2*(q1*q3-q0*q2);
//	vy=2*(q0*q1+q2*q3);
//	vz=q0*q0-q1*q1-q2*q2+q3*q3;
//	
//	//错误的领域和方向传感器测量参考方向之间的交叉乘积的总和
//	ex=(ay*vz-az*vy);
//	ey=(az*vx-ax*vz);
//	ez=(ax*vy-ay*vx);
//	
//	//积分误差比例积分增益
//	exInt=exInt+ex*Ki;
//	eyInt=eyInt+ey*Ki;
//	ezInt=ezInt+ez*Ki;
//	
//	//调整后的陀螺仪测量
//	gx=gx+Kp*ex+exInt;
//	gy=gy+Kp*ey+eyInt;
//	gz=gz+Kp*ez+ezInt;
//	
//	q0temp=q0;//暂存当前值用于计算
//  q1temp=q1;
//  q2temp=q2;
//  q3temp=q3;
//	
//	//整合四元数率和正常化
//	q0=q0temp+(-q1temp*gx-q2temp*gy-q3temp*gz)*HALFT;
//	q1=q1temp+(q0temp*gx+q2temp*gz-q3temp*gy)*HALFT;
//	q2=q2temp+(q0temp*gy-q1temp*gz+q3temp*gx)*HALFT;
//	q3=q3temp+(q0temp*gz+q1temp*gy-q2temp*gx)*HALFT;
//	
//	//正常化四元
//	norm=sqrt(q0*q0+q1*q1+q2*q2+q3*q3);
//	q0=q0/norm;
//	q1=q1/norm;
//	q2=q2/norm;
//	q3=q3/norm;
//
//	quaternion.Pitch=asin(-2*q1*q3+2*q0*q2)*57.3;//pitch，转换为度数
//	quaternion.Roll=atan2(2*q2*q3+2*q0*q1,-2*q1*q1-2*q2*q2+1)*57.3;//rollv
//	quaternion.Yaw=atan2(2*(q1*q2+q0*q3),q0*q0+q1*q1-q2*q2-q3*q3)*57.3;

//}

