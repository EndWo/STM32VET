#include "steeringGear.h"

int pwmoutX;
int pwmoutY;

u16 SGear0_Level = 1565;
u16 SGear1_Level = 1410;

/******
SGear0  С��ƽ��ֵԼ1500ʱ����������㵹����С�����X��С�����˶�
SGear1  С��ƽ��ֵԼ1500ʱ������ǰ���㵹����С�����Y��С�����˶�
*************/
void SGear_Init()
{
	//ʹ�ö�ʱ��3���pwm��
	TIM3_PWM_Init(PWM_DUTY-1,TIMER_PSC-1);//f=50hz.
    SGear_Level_Init();
}

//SGear move function
void SGear_move(u8 SGear_NO,int change)
{
	int pwm_val;

	if(SGear_NO==0)
	{
		pwm_val = SGear0_Level + change;
	}
  else if(SGear_NO==1)
	{
		pwm_val = SGear1_Level + change;
	}
	
	SGear_Output(SGear_NO,pwm_val);
}

//ˮƽλ�ó�ʼ��
void SGear_Level_Init()
{
	SGear_move(0,0);
	SGear_move(1,0);
}

void SGear_Output(u8 SGear_NO,int pwm)
{
	if(pwm > PWM_HIGH_MAX)  pwm = PWM_HIGH_MAX; //�ж�pwm_val�Ƿ񳬳���Χ
  if(pwm < PWM_HIGH_MIN)  pwm = PWM_HIGH_MIN;
	
	if(SGear_NO==0)
	{
		SGear0=pwm;
		pwmoutX = pwm;
	}
	  
  else if(SGear_NO==1)
  {
		SGear1=pwm;
		pwmoutY = pwm;
	}
}

//���Զ����ת
void SGear_Test ()
{
		delay_ms(1000);
		SGear0=1500;
		//SGear1=1500;

		delay_ms(1000);
		SGear0=700;
		//SGear1=700;

		delay_ms(1000);
		SGear0=1500;
		//SGear1=1500;

		delay_ms(1000);
		SGear0=2200;
		//SGear1=2200;

}

//ˮƽλ�ò��Ե���ʹ��
void SGear_Level_Test()
{
	SGear0=1420;
	SGear1=1500;
}

