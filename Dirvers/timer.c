#include "timer.h"

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��

	GPIO_Init_One_AF(GPIOB,GPIO_Pin_0, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);//TIM3->CH3	 
	GPIO_Init_One_AF(GPIOB,GPIO_Pin_1, GPIO_Mode_AF_PP,GPIO_Speed_50MHz);//TIM3->CH4
 
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ���low
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC4
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC3
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR4�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3

}

///1.ͨ�ö�ʱ������ģʽ��ʼ������ ,DEFAULT:TIM_CounterMode_Up
//TIMx:ͨ�ö�ʱ����2,3,4,5�� 
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIMx_Counter_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//timer base struct
       if(TIMx==TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
  else if(TIMx==TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
  else if(TIMx==TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
  else if(TIMx==TIM5) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
  	//��ʱ��TIMx��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIMx�ж�,��������ж�

	//�ж����ȼ�NVIC����
	      if(TIMx==TIM2) NVIC_Set_Init(TIM2_IRQn,0,3); 
  else if(TIMx==TIM3) NVIC_Set_Init(TIM3_IRQn,0,3);
  else if(TIMx==TIM4) NVIC_Set_Init(TIM4_IRQn,0,3); 
  else if(TIMx==TIM5) NVIC_Set_Init(TIM5_IRQn,0,3); 
	
	TIM_Cmd(TIMx, ENABLE);  //ʹ��TIMx
}

int TIM1_UP_IRQHandler(void)  
{
	if(TIM1->SR&0X0001)//5ms��ʱ�ж�
	{   
		TIM1->SR&=~(1<<0);                                       //===�����ʱ��1�жϱ�־λ
		control();
	}
	return 0;
}

void Timer1_Init(u16 arr,u16 psc)  
{  
	RCC->APB2ENR|=1<<11;//TIM1ʱ��ʹ��    
 	TIM1->ARR=arr;      //�趨�������Զ���װֵ   
	TIM1->PSC=psc;      //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	TIM1->DIER|=1<<0;   //��������ж�				
	TIM1->DIER|=1<<6;   //�������ж�	   
	TIM1->CR1|=0x01;    //ʹ�ܶ�ʱ��
	MY_NVIC_Init(1,3,TIM1_UP_IRQn,2);
}

