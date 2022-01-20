#include "I2C.h"

#define GPIO_Pin_SDA (I2CX->GPIO_Pin_SDA)
#define GPIO_Pin_SCL (I2CX->GPIO_Pin_SCL)
#define GPIOX (I2CX->GPIOX)

/************************************************************************************************************************
ʱ�䣺20190730  2013
���ߣ�����&����
��ʼ��ģ��IIC���ṹ�帳ֵ������ʹ��
����ط���
	�ṹ���Ա���������⣬��GPIOEΪ�����ڿ⺯����Ϊ((GPIO_TypeDef *) GPIOE_BASE)����GPIO_TypeDef���͵�ָ��
	������ֱ�Ӷ���ΪGPIO_TypeDef�ṹ�壬Ȼ��ʹ��&ȡָ������ȡ��������������ģ�����GPIO_TypeDef���͵�ָ��
************************************************************************************************************************/
void I2C_Soft_Init(I2C_Soft* I2CX,GPIO_TypeDef* GPIOX0,uint16_t GPIO_Pin_SCL0,uint16_t GPIO_Pin_SDA0)
{
	GPIOX = GPIOX0;
	GPIO_Pin_SCL = GPIO_Pin_SCL0;
	GPIO_Pin_SDA = GPIO_Pin_SDA0;
	
	GPIO_Init_One_AF(GPIOX,GPIO_Pin_SDA|GPIO_Pin_SCL,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	
	GPIO_SetBits(GPIOX,GPIO_Pin_SDA|GPIO_Pin_SCL);
}


void IIC_Delay()
{
	delay_us(2);
}

void IIC_Start(I2C_Soft* I2CX)
{
	SDA_OUT(I2CX);     //sda�����
	IIC_SDA(I2CX,1);	  	  
	IIC_SCL(I2CX,1);
	IIC_Delay();
 	IIC_SDA(I2CX,0);//START:when CLK is high,DATA change form high to low 
	IIC_Delay();
	IIC_SCL(I2CX,0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}

void IIC_Stop(I2C_Soft* I2CX)
{
	SDA_OUT(I2CX);//sda�����
	IIC_SCL(I2CX,0);
	IIC_SDA(I2CX,0);//STOP:when CLK is high DATA change form low to high
 	IIC_Delay();
	IIC_SCL(I2CX,1); 
	IIC_SDA(I2CX,1);//����I2C���߽����ź�
	IIC_Delay();
}

void IIC_Send_Byte(I2C_Soft* I2CX,u8 txd)
{
  u8 t;   
	SDA_OUT(I2CX); 	    
	IIC_SCL(I2CX,0);//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{
			IIC_SDA(I2CX,(txd&0x80)>>7);
			txd<<=1; 	  
			IIC_SCL(I2CX,1);
			IIC_Delay(); 
			IIC_SCL(I2CX,0);	
			IIC_Delay();
	}
}

u8 IIC_Read_Byte(I2C_Soft* I2CX,unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN(I2CX);//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
    IIC_SCL(I2CX,0); 
		IIC_Delay();
		IIC_SCL(I2CX,1);
    receive<<=1;
		if(READ_SDA(I2CX))receive++;   
		IIC_Delay(); 
   }					 
	if(!ack)
		IIC_NAck(I2CX);//����nACK
	else
		IIC_Ack(I2CX); //����ACK 
	
	return receive;
}

u8 IIC_Wait_Ack(I2C_Soft* I2CX)
{
	u8 ucErrTime=0;
	SDA_IN(I2CX);      //SDA����Ϊ����  
	IIC_SDA(I2CX,1);IIC_Delay();	   
	IIC_SCL(I2CX,1);IIC_Delay();	 
	while(READ_SDA(I2CX))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop(I2CX);
			return 1;
		}
	}
	IIC_SCL(I2CX,0);//ʱ�����0 	   
	return 0;
}

void IIC_Ack(I2C_Soft* I2CX)
{
	IIC_SCL(I2CX,1);
	SDA_OUT(I2CX);
	IIC_SDA(I2CX,0);
	IIC_Delay();
	IIC_SCL(I2CX,1);
	IIC_Delay();
	IIC_SCL(I2CX,0);
}

void IIC_NAck(I2C_Soft* I2CX)
{
	IIC_SCL(I2CX,0);
	SDA_OUT(I2CX);
	IIC_SDA(I2CX,0);
	IIC_Delay();
	IIC_SCL(I2CX,1);
	IIC_Delay();
	IIC_SCL(I2CX,0);
}


u8 READ_SDA(I2C_Soft* I2CX)
{
	u8 result=GPIO_ReadInputDataBit(GPIOX,GPIO_Pin_SDA);

	return result;
}

void IIC_SDA(I2C_Soft* I2CX,u8 Level)
{
	if(Level==0)
	{
		GPIO_ResetBits(GPIOX,GPIO_Pin_SDA);
	}
	else if(Level==1)
	{
		GPIO_SetBits(GPIOX,GPIO_Pin_SDA);
	}
}

void IIC_SCL(I2C_Soft* I2CX,u8 Level)
{
	if(Level==0)
	{
		GPIO_ResetBits(GPIOX,GPIO_Pin_SCL);
	}
	else if(Level==1)
	{
		GPIO_SetBits(GPIOX,GPIO_Pin_SCL);
	}
}

void SDA_IN(I2C_Soft* I2CX)
{
	GPIO_Init_One_IN(GPIOX,GPIO_Pin_SDA,GPIO_Mode_IPD);
}

void SDA_OUT(I2C_Soft* I2CX)
{
	GPIO_Init_One_AF(GPIOX,GPIO_Pin_SDA,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
}

