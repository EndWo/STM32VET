#include "spi.h"

#define temp_SPI_max 200
#define zhentou 0xaa		//֡ͷ
#define zhenwei 0x0d		//֡β

#if EN_SPI1  == 1

u8 temp_SPI1[temp_SPI_max];

void SPI1_IRQHandler(){
	static int point_Count_SPI1 = 0;
	if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET)
	{ 
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
			SPI_I2S_SendData(SPI1,temp_SPI1[point_Count_SPI1]); //ͨ������SPIx����һ������
		
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
			temp_SPI1[point_Count_SPI1]=SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
		
			SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_RXNE);//clear SPI_rx_FLAG
			point_Count_SPI1++;
   }
}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
void SPI1_ReadWriteString(u8 *temp)
{	
	SPI1_CS=0;	
	static int point_Count_SPI1;
		for(point_Count_SPI1=0;temp[point_Count_SPI1]!=zhenwei;point_Count_SPI1++){			 	
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ	  
			SPI_I2S_SendData(SPI1,temp[point_Count_SPI1]); //ͨ������SPIx����һ������

			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ	
			temp[point_Count_SPI1]=SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����				
		}
	SPI1_CS=1;
}
#endif

#if EN_SPI2  == 1

u8 temp_SPI2[temp_SPI_max];

void SPI2_IRQHandler(){
	static int point_Count_SPI2 = 0;
	if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET)
	{ 
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
			SPI_I2S_SendData(SPI2,temp_SPI2[point_Count_SPI2]); //ͨ������SPIx����һ������
		
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
			temp_SPI2[point_Count_SPI2]=SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
		
			SPI_I2S_ClearITPendingBit(SPI2,SPI_I2S_IT_RXNE);//clear SPI_rx_FLAG
			point_Count_SPI2++;
   }
}

//SPI2 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
void SPI2_ReadWriteString(u8 *temp)
{	
	SPI2_CS=0;
	
	static int point_Count_SPI2;
		for(point_Count_SPI2=0;temp[point_Count_SPI2]!=zhenwei;point_Count_SPI2++){			 	
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ	  
			SPI_I2S_SendData(SPI2,temp[point_Count_SPI2]); //ͨ������SPIx����һ������

			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ	
			temp[point_Count_SPI2]=SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����				
		}	
		
	SPI2_CS=1;	
}

#endif

#if EN_SPI3  == 1

u8 temp_SPI3[temp_SPI_max];

void SPI3_IRQHandler(){
	static int point_Count_SPI3 = 0;
	if(SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET)
	{ 
			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
			SPI_I2S_SendData(SPI3,temp_SPI3[point_Count_SPI3]); //ͨ������SPIx����һ������
		
			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
			temp_SPI3[point_Count_SPI3]=SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����
		
			SPI_I2S_ClearITPendingBit(SPI3,SPI_I2S_IT_RXNE);//clear SPI_rx_FLAG
			point_Count_SPI3++;
   }
}

void SPI3_ReadWriteString(u8 *temp)
{	
	SPI3_CS=0;
	
	static int point_Count_SPI3;
		for(point_Count_SPI3=0;temp[point_Count_SPI3]!=zhenwei;point_Count_SPI3++){			 	
			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ	  
			SPI_I2S_SendData(SPI3,temp[point_Count_SPI3]); //ͨ������SPIx����һ������

			while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ	
			temp[point_Count_SPI3]=SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����				
		}
		
	SPI3_CS=1;	
}

#endif


//SPI�ڳ�ʼ��
void SPIX_Init(SPI_TypeDef* SPIX,uint16_t SPI_Mode,u8 SPI_BaudRatePrescaler)
{	 
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	if(SPIX==((SPI_TypeDef *) SPI1_BASE)){

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );//PORTBʱ��ʹ��
		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI1ʱ��ʹ��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
		GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PB13/14/15����
		
	}else if(SPIX==((SPI_TypeDef *) SPI2_BASE)){
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE );//PORTBʱ��ʹ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE );//SPI2ʱ��ʹ��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
		GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����
		
	}else if(SPIX==((SPI_TypeDef *) SPI3_BASE)){
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE );//PORTBʱ��ʹ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE );//SPI2ʱ��ʹ��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
		GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);  //PB13/14/15����
	}

	if(SPI_Mode==SPI_Mode_Slave){
		
		NVIC_InitTypeDef NVIC_InitStructure;
		
		if(SPIX==((SPI_TypeDef *) SPI1_BASE))
		{
			NVIC_InitStructure.NVIC_IRQChannel=SPI1_IRQn;
		}
		else if(SPIX==((SPI_TypeDef *) SPI2_BASE))
		{
			NVIC_InitStructure.NVIC_IRQChannel=SPI2_IRQn;
		}
		else if(SPIX==((SPI_TypeDef *) SPI3_BASE))
		{
			NVIC_InitStructure.NVIC_IRQChannel=SPI3_IRQn;
		}
		
		SPI_I2S_ITConfig(SPIX, SPI_I2S_IT_RXNE, ENABLE);
		SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;		//����SPI����ģʽ:����Ϊ��SPI
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
		
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	
	if(SPI_Mode==SPI_Mode_Master){
		GPIO_InitTypeDef GPIO_InitStructure2;
		
		GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
		GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
		
		if(SPIX==((SPI_TypeDef *) SPI1_BASE))
		{
			GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_4;  // PB12 ����
			GPIO_Init(GPIOA, &GPIO_InitStructure2);
			GPIO_SetBits(GPIOA,GPIO_Pin_4);
		}
		else if(SPIX==((SPI_TypeDef *) SPI2_BASE))
		{
			GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_12;
			GPIO_Init(GPIOB, &GPIO_InitStructure2);
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
		}
		else if(SPIX==((SPI_TypeDef *) SPI3_BASE))
		{
			GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_15;
			GPIO_Init(GPIOA, &GPIO_InitStructure2);
			GPIO_SetBits(GPIOA,GPIO_Pin_15);
		}

		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	}
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ

	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPIX->CR1&=0XFFC7;
	SPIX->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ�
	
	SPI_Init(SPIX, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPIX,ENABLE); 
}
