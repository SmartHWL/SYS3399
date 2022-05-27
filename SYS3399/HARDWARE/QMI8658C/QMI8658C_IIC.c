#include "qmi8658c_iic.h"
#include "BSP_iic.h"

/* �ṹ������ */
QMI_DATA qmi_get_date;
QUATERnion quaternion;
MQI_DATE_ZEROCHECK mqi_date_zerocheck;


/* �������� */
void vI2CQMI6858C_Init(void);
void vI2CQMI_GetData(void);
//void vI2CQMI_Data_Process(void);

/* ����ָ������ */
I2CQMI_FUN_STR  strI2cQMIFun=
{
	&vI2CQMI6858C_Init,
	&vI2CQMI_GetData,
//	&vI2CQMI_Data_Process,
};


/************************************************* 
 Function:    vI2CQMI_Init
 Description: QMI6858C�����ǳ�ʼ��
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
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL2_ADD,0x24);//���ٶȼ����� ������Ϊ8g  �����������470hz
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL3_ADD,0x64);//���������� ������Ϊ1024dps�������������Ϊ470hz   (X/32768*1024*0.0174532925)
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL5_ADD,0x77);//������������  ���ü��ٶȼƺ������ǵĵ�ͨ�˲� ODR��13.37%
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL7_ADD,0x00);
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
	vRw_IIC(RW_QMI8658_WRITE,QMI8658C_ADDress,QMI_CTRL7_ADD,0x03);//�������ٶȼƺ�������
	while(strI2cMsg.status != I2C_MSG_STATUS_RW_OK);
	strI2cMsg.status = I2C_MSG_STATUS_IDLE;
}

/************************************************* 
 Function:    u16Average
 Description: ȡ�м�ֵ��ƽ��ֵ
 Input:      *a���������飻num�������������� n:�Ƿ������ֵȡƽ��ֵ
             Ϊ������0��ƫ�� ѡ�������ֵȡƽ��ֵ
 Output:      
 Return:     return��ƽ��ֵ
 Others: 
*************************************************/
static int_least16_t u16Average(int_least16_t *a,uint8_t num,uint8_t n)
{
	uint8_t i=0,j=0,z=0;
	uint8_t nums;
	int_least32_t sum=0;
	int_least16_t average=0,temp;
	nums=num-((num/4)<<1);
	//ð�������㷨������ n-1 �ֱȽ�
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
	//�����ȡ�м�1/2���ݽ���ƽ��ֵ����
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
 Description: ȥ��ƫ
 Input:      adr����ȥ��ƫ��ֵ ��adv����ʼֵ��ƽ��ֵ
 Output:      
 Return:     return��ȥ����ƫ���ֵ
 Others: 
*************************************************/
static int_least16_t u16zero_check(int_least16_t buff,int_least16_t adv)  
{
	if(buff&0x8000)     //�ж���ֵ���������Ǹ���
		return (buff+adv);
	else 
		return (buff-adv);
	
}

/************************************************* 
 Function:    vQMI_GetnData
 Description: �ɼ�num������
 Input:      
 Output:      
 Return:     get_flag��num�������Ƿ�ɼ����
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
			//�ɼ���������ȥ��ƫ			
			mqi_date_zerocheck.ax[i]=u16zero_check((mqi_buff[1]<<8)|mqi_buff[0],mqi_date_zerocheck.ax_adv);
			mqi_date_zerocheck.ay[i]=u16zero_check((mqi_buff[3]<<8)|mqi_buff[2],mqi_date_zerocheck.ay_adv);
//			mqi_date_zerocheck.az[i]=(mqi_buff[5]<<8)|mqi_buff[4];//ƽ�Ų���ʱaz��Ϊ0 ����Ҫȥ��ƫ
//			mqi_date_zerocheck.az[i]=u16zero_check((mqi_buff[5]<<8)|mqi_buff[4],mqi_date_zerocheck.az_adv);  //�޸�ƫ��
			mqi_date_zerocheck.az[i]=u16zero_check((mqi_buff[5]<<8)|mqi_buff[4],4*mqi_date_zerocheck.az_adv);  //�޸�ƫ��
            
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
 Description: ��ȡ����������
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
		if(vQMI_GetnData(QMI_CHECKLEN))  //����ʱƽ�Ų�������ȡ��ʼ�Ķ��ֵ������ƫ
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
// Description: ���������ݴ���
// Input:   
// Output:      
// Return: 
// Others: 
//*************************************************/
//void vI2CQMI_Data_Process()
//{
//	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;//��Ԫ����Ԫ�أ�������Ʒ���
//	float exInt=0.0f,eyInt=0.0f,ezInt=0.0f;//��������С�������
//	float norm;
//	float ax,ay,az;
//	float gx,gy,gz;
//	float q0temp,q1temp,q2temp,q3temp;//��Ԫ���ݴ���������΢�ַ���ʱҪ��
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
//	//��λ��
//	ax=ax/norm;
//	ay=ay/norm;
//	az=az/norm;
//	
//	//���Ʒ��������
//	vx=2*(q1*q3-q0*q2);
//	vy=2*(q0*q1+q2*q3);
//	vz=q0*q0-q1*q1-q2*q2+q3*q3;
//	
//	//���������ͷ��򴫸��������ο�����֮��Ľ���˻����ܺ�
//	ex=(ay*vz-az*vy);
//	ey=(az*vx-ax*vz);
//	ez=(ax*vy-ay*vx);
//	
//	//������������������
//	exInt=exInt+ex*Ki;
//	eyInt=eyInt+ey*Ki;
//	ezInt=ezInt+ez*Ki;
//	
//	//������������ǲ���
//	gx=gx+Kp*ex+exInt;
//	gy=gy+Kp*ey+eyInt;
//	gz=gz+Kp*ez+ezInt;
//	
//	q0temp=q0;//�ݴ浱ǰֵ���ڼ���
//  q1temp=q1;
//  q2temp=q2;
//  q3temp=q3;
//	
//	//������Ԫ���ʺ�������
//	q0=q0temp+(-q1temp*gx-q2temp*gy-q3temp*gz)*HALFT;
//	q1=q1temp+(q0temp*gx+q2temp*gz-q3temp*gy)*HALFT;
//	q2=q2temp+(q0temp*gy-q1temp*gz+q3temp*gx)*HALFT;
//	q3=q3temp+(q0temp*gz+q1temp*gy-q2temp*gx)*HALFT;
//	
//	//��������Ԫ
//	norm=sqrt(q0*q0+q1*q1+q2*q2+q3*q3);
//	q0=q0/norm;
//	q1=q1/norm;
//	q2=q2/norm;
//	q3=q3/norm;
//
//	quaternion.Pitch=asin(-2*q1*q3+2*q0*q2)*57.3;//pitch��ת��Ϊ����
//	quaternion.Roll=atan2(2*q2*q3+2*q0*q1,-2*q1*q1-2*q2*q2+1)*57.3;//rollv
//	quaternion.Yaw=atan2(2*(q1*q2+q0*q3),q0*q0+q1*q1-q2*q2-q3*q3)*57.3;

//}

