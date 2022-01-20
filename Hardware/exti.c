#include "exti.h"

#define xiaodouMS 15
#define Mode1Count 0x70
#define Mode2Count 0x08
u8 mode_flag=0x00;
int mode_value[12];

extern pid_k pidx,pidy,pidvx,pidvy;

/**************************************************************************
�������ܣ��ⲿ�жϳ�ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void MY_EXTI_Init(void)
{
	KEY_Init();
	Ex_NVIC_Config(GPIO_E,0,FTIR);		//�½��ش���
	Ex_NVIC_Config(GPIO_E,1,FTIR);		//�½��ش���
	Ex_NVIC_Config(GPIO_E,2,FTIR);		//�½��ش���
	Ex_NVIC_Config(GPIO_E,3,FTIR);		//�½��ش���
	Ex_NVIC_Config(GPIO_E,4,FTIR);		//�½��ش���

	MY_NVIC_Init(2,2,EXTI0_IRQn,2);  	//��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,2,EXTI1_IRQn,2);  	//��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,2,EXTI2_IRQn,2);  	//��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,2,EXTI3_IRQn,2);  	//��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,2,EXTI4_IRQn,2);  	//��ռ2�������ȼ�2����2
	
	for(int i=0;i<12;i++)
	{
		mode_value[i]=1;
	}
}


void EXTI0_IRQHandler(void)
{			
	delay_ms(xiaodouMS);   //����
	//pause or start
	button_flag_control(0);
  EXTI->PR=1<<0; //���LINE0�ϵ��жϱ�־λ  
}

void EXTI1_IRQHandler(void)
{			
	delay_ms(xiaodouMS);   //���� 
	//sub
	button_flag_control(4);
 	EXTI->PR=1<<1;     //���LINE1�ϵ��жϱ�־λ  
}

void EXTI2_IRQHandler(void)
{
	delay_ms(xiaodouMS);   //����
	//ģʽ2
	button_flag_control(2);
	EXTI->PR=1<<2;     //���LINE2�ϵ��жϱ�־λ
}

void EXTI3_IRQHandler(void)
{
	delay_ms(xiaodouMS);   //����
	//add
	button_flag_control(3);	
	EXTI->PR=1<<3;     //���LINE3�ϵ��жϱ�־λ
}

void EXTI4_IRQHandler(void)
{
	delay_ms(xiaodouMS);   //����
	//ģʽ1
	button_flag_control(1);
	EXTI->PR=1<<4;     //���LINE4�ϵ��жϱ�־λ
}

void KEY_Init(void)
{
	RCC->APB2ENR|=1<<6;

	GPIOE->CRL&=0XFFF00000; 
	GPIOE->CRL|=0X00088888;
	
	GPIOE->ODR|=1<<0; //PE 0
	GPIOE->ODR|=1<<1; //PE 1
	GPIOE->ODR|=1<<2; //PE 2
	GPIOE->ODR|=1<<3; //PE 3
	GPIOE->ODR|=1<<4; //PE 4
} 

/*****************************
�����߼�������
	������5����һ�����ƿ��أ�������������ģʽ�л����������ڸ�ģʽ�µĿ���
	mode_flagΪu8,��Ϊ��λ��λ����/��Ϊ���鲻ͬģʽ��־��
	����һ��ģʽ��һ��ģʽ��־Ϊ����
	���¼�/������֮���ȡmode_flag���ж�ģʽ֮��������Ӧ����
*****************************/
void button_flag_control(u8 buttonX)
{
	//����Ӳ������
	if(buttonX==0)
	{
		//�����ܿ���
		if(control_Flag ==1)
		{
			control_Flag = 0;
		}else
		{
			control_Flag = 1;
		}
	}
	//ģʽ1ѡ��
	else if(buttonX==1)
	{
		//����һģʽλ����
		mode_flag &=0xf0;
		
		//��������Ҷ�����6��ģʽ���������Ӧ�Ļ�ģʽ1
		if(mode_flag ==Mode1Count)
		{
			mode_flag =0x00;
		}
		mode_flag += 0x10;

	}
	//ģʽ2ѡ��
	else if(buttonX==2)
	{
		//����һģʽλ����
		mode_flag &=0x0f;
		//��������Ҷ�����8��ģʽ���������Ӧ�Ļ�ģʽ1
		if(mode_flag ==Mode2Count)
		{
			mode_flag =0x00;
		}
		mode_flag += 0x01;
		
	}
	//ģʽѡ��ļӰ���
	else if(buttonX==3)
	{
		//����0��1��������������ְ����ĸ���
		//��������������������ִ�а���
		button_control(0);
	}
	//ģʽѡ��ļ�����
	else if(buttonX==4)
	{
		button_control(1);
	}

}

//AddΪ0������¼�
//AddΪ1������¼�
//�����ʵ�ʵĴ���ִ�в���
void button_control(u8 Add)
{
	//��������Ƕ�9�������
	if(mode_flag ==0x10)
	{
		//openmv_Init();
		mode_flag=0x00;
	}
	
	//������ð���ˮƽ
	else if(mode_flag ==0x20)
	{
		control_Flag=0;
		SGear_move(1,0);
		SGear_move(0,0);
	}
	
	//���������ʾģʽ
	else if(mode_flag ==0x30)
	{
		if(Add == 0)
		{
			//�ı�ģʽ
			Mode_move++;
			//�����������ʾ����Ŀ����Ϊ1
			if(Mode_move==10)
			{
				Mode_move=1;
			}
		}

		//���������ڿ���Y�����Ƿ���Ч
		if(Add == 1)
		{
			Mode_move--;
			//������ڿ���ʾ����Ŀ����Ϊ9
			//��������1-9ѭ��
			if(Mode_move<1)
			{
				Mode_move=9;
			}
		}
	}
	
	//����ǿ���x��y�����Ƿ���Ч
	else if(mode_flag ==0x40)
	{
		//�Ӱ������ڿ���X�����Ƿ���Ч
		if(Add == 0)
		{
			if(FlagX  ==1)
			{
				FlagX  = 0;
			}else
			{
				FlagX  = 1;
			}
		}

		//���������ڿ���Y�����Ƿ���Ч
		if(Add == 1)
		{
			if(FlagY ==1)
			{
				FlagY = 0;
			}else
			{
				FlagY = 1;
			}
		}
	}
	
	//����ǿ����ٶȺ�λ�û���Ч
	else if(mode_flag ==0x50)
	{
		//�Ӱ������ڿ���λ���Ƿ���Ч
		if(Add == 0)
		{
			if(FlagPoint ==1)
			{
				FlagPoint = 0;
			}else
			{
				FlagPoint = 1;
			}
		}

		//���������ڿ����ٶ��Ƿ���Ч
		if(Add == 1)
		{
			if(FlagSpeed ==1)
			{
				FlagSpeed = 0;
			}else
			{
				FlagSpeed = 1;
			}
		}
	}
	//change pages ��Ļ̫С����ҳ����
	else if(mode_flag ==0x60)
	{
		
		if(page_flag==0)
		{
			page_flag=1;
		}
		else
		{
			page_flag=0;
		}
	}
	//change piont flag
	else if(mode_flag ==0x70)
	{
		if(Add == 0)
		{
			//�ı�ģʽ
			point_Flag++;
			//�����������ʾ����Ŀ����Ϊ1
			if(point_Flag==10)
			{
				point_Flag=1;
			}
		}

		//���������ڿ���Y�����Ƿ���Ч
		if(Add == 1)
		{
			point_Flag--;
			//������ڿ���ʾ����Ŀ����Ϊ9
			//��������1-9ѭ��
			if(point_Flag<1)
			{
				point_Flag=9;
			}
		}
		
		chang_point();
	}
	
	//λ�û�KP����
	else if(mode_flag ==0x01)
	{
		if(Add == 0)
		{
			pidx.KP++;
			pidy.KP++;
		}

		if(Add == 1)
		{
			pidx.KP--;
			pidy.KP--;
		}
	}
	
	//λ�û�KI����
	else if(mode_flag ==0x02)
	{
		if(Add == 0)
		{
			pidx.KI+=0.0001;
			pidy.KI+=0.0001;
		}

		if(Add == 1)
		{
			pidx.KI-=0.0001;
			pidy.KI-=0.0001;
		}
	}
	
	//λ�û�KD����
	else if(mode_flag ==0x03)
	{
		if(Add == 0)
		{
			pidx.KD+=10;
			pidy.KD+=10;
		}

		if(Add == 1)
		{
			pidx.KD-=10;
			pidy.KD-=10;
		}
	}
	
	//�ٶȻ�KP����
	else if(mode_flag ==0x04)
	{
		if(Add == 0)
		{
			pidvx.KP++;
			pidvy.KP++;
		}

		if(Add == 1)
		{
			pidvx.KP--;
			pidvy.KP--;
		}
	}
	
	//�ٶȻ�KI����
	else if(mode_flag ==0x05)
	{
		if(Add == 0)
		{
			pidvx.KI+=0.0001;
			pidvy.KI+=0.0001;
		}

		if(Add == 1)
		{
			pidvx.KI-=0.0001;
			pidvy.KI-=0.0001;
		}
	}
	
	//�ٶȻ�KD����
	else if(mode_flag ==0x06)
	{
		if(Add == 0)
		{
			pidvx.KD+=10;
			pidvy.KD+=10;
		}

		if(Add == 1)
		{
			pidvx.KD-=10;
			pidvy.KD-=10;
		}
	}
	
	//���X�������PWM���޸�
	else if(mode_flag ==0x07)
	{
		//��ͣPID���������
		control_Flag=0;
		
		if(Add == 0)
		{
			pwmoutX+=10;
		}

		if(Add == 1)
		{
			pwmoutX-=10;
		}
		
		SGear_Output(0,pwmoutX);
	}
	
	//���Y�������PWM���޸�
	else if(mode_flag ==0x08)
	{
		//��ͣPID���������
		control_Flag=0;
		
		if(Add == 0)
		{
			pwmoutY+=10;
		}

		if(Add == 1)
		{
			pwmoutY-=10;
		}
		
		SGear_Output(1,pwmoutY);
	}
}

//������ʾ����
//�������л�����ģʽ����һ������
//���ڸ��û���ʾ��ǰģʽ
void button_display(void)
{
	if(mode_flag==0x00)
	{
		OLED_ShowString(70,52,"       ") ; 
		OLED_Refresh_Gram();
	}
	//��������Ƕ�9�������
	//����
	if(mode_flag ==0x10)
	{
		OLED_ShowString(70,52,"duPoint") ; 
		OLED_Refresh_Gram();
	}
	
	//������ð���ˮƽ
	else if(mode_flag ==0x20)
	{
		OLED_ShowString(70,52,"level  ") ; 
		OLED_Refresh_Gram();
	}
	
	//���������ʾģʽ
	else if(mode_flag ==0x30)
	{
		OLED_ShowString(70,52,"Demo   ") ; 
		OLED_Refresh_Gram();
	}
	
	//����ǿ���x��y�����Ƿ���Ч
	else if(mode_flag ==0x40)
	{
		OLED_ShowString(70,52,"X&Y    ") ; 
		OLED_Refresh_Gram();
	}
	
	//����ǿ����ٶȺ�λ�û���Ч
	//pΪλ�ã�vΪ�ٶ�
	else if(mode_flag ==0x50)
	{
		OLED_ShowString(70,52,"P&V    ") ; 
		OLED_Refresh_Gram();
	}
	else if(mode_flag ==0x60)
	{
		OLED_ShowString(70,52,"pages  ") ; 
		OLED_Refresh_Gram();
	}
	else if(mode_flag ==0x70)
	{
		OLED_ShowString(70,52,"point  ") ; 
		OLED_Refresh_Gram();
	}
	
	//λ�û�KP����
	//PP��point KP
	else if(mode_flag ==0x01)
	{
		OLED_ShowString(70,52,"PP     ") ; 
		OLED_Refresh_Gram();
	}
	
	//λ�û�KI����
	//PI��point KI
	else if(mode_flag ==0x02)
	{
		OLED_ShowString(70,52,"PI     ") ; 
		OLED_Refresh_Gram();
	}
	
	//λ�û�KD����
	//PD��point KD
	else if(mode_flag ==0x03)
	{
		OLED_ShowString(70,52,"PD     ") ; 
		OLED_Refresh_Gram();
	}
	
	//�ٶȻ�KP����
	//VP��Speed KP
	else if(mode_flag ==0x04)
	{
		OLED_ShowString(70,52,"VP     ") ; 
		OLED_Refresh_Gram();
	}
	
	//�ٶȻ�KI����
	//VI��Speed KI
	else if(mode_flag ==0x05)
	{
		OLED_ShowString(70,52,"VI     ") ; 
		OLED_Refresh_Gram();
	}
	
	//�ٶȻ�KD����
	//VD��Speed KD
	else if(mode_flag ==0x06)
	{
		OLED_ShowString(70,52,"VD     ") ; 
		OLED_Refresh_Gram();
	}
	
	//���X�������PWM���޸�
	//DJX��X����Ķ�� DuoJi
	else if(mode_flag ==0x07)
	{
		OLED_ShowString(70,52,"DJX    ") ; 
		OLED_Refresh_Gram();
	}
	
	//���Y�������PWM���޸�
	else if(mode_flag ==0x08)
	{
		OLED_ShowString(70,52,"DJY    ") ; 
		OLED_Refresh_Gram();
	}
}

/************************************************************
��ô���ӹ��ܣ�������ģʽ
	����ģʽ����Ψһ��mode_flag
			Ŀǰֻ��0x01-0x0f
			0x10-0xf0 �ۼ�30��
	�޸����		
		#define Mode1Count 0x60
		#define Mode2Count 0x08
		�������ÿ��ģʽ������ֵ
	��void button_control(u8 Add)��Ϊ��ӦID����߼�
	��void button_display(void)��Ϊ��ӦID�����ʾ����
�Ƽ��߼��ڽ��޸����ڿ��Ƶı�־λ�����ߵ��ò����򵥵ĺ�������
*************************************************************/
