#include "main.h"

#define ms20 3
#define deadbandValue 0
///////////////////////////////ȫ�ֱ�������/////////////////////////////////

//0-8Ϊ�Ÿ����Xֵ��9-17ΪY��ֵ
u8 point_Array[]={66,165,225,65,170,222,66,145,221,38,39,43,111,117,118,192,193,192};//���ڶ����9������

//���ڰ������Ʊ�־
u8 FlagX=1,FlagY=1,FlagPoint=1,FlagSpeed=1,control_Flag =1;

//���ڼ����ٶ�
int Last_pointX,Last_pointY;

//����洢��ǰλ�ú��ٶȵı���
int pointX,pointY,SpeedX,SpeedY;

//���ڴ洢pid������
int chang_PointY,chang_PointX,chang_SpeedY,chang_SpeedX;

//����Ŀ���X,Y
float globalValue_x,globalValue_y;

//����λ�����ٶȵĿ��ƽ���
int Flag_ms = 0,Flag2_ms = 0;

//��ʼ��Ŀ���,0 is null			//��ʼ���˶�ģʽ
u8 point_Flag = 5,					Mode_move = 0;
u8 A,B,C,D;

//С����Ŀ����������Χ�ھͼ�һ�������ڼ�ʱ����1Ϊ50ms��ȡ���ڶ�ʱ��1���ִ��һ��control();
int count_Flag= 0;

//����άƽ��Ŀ���ת��Ϊ�������򣬳���λ�û�Ҫ�����ٶȣ��ٶ�Ŀ��ֵΪ0
////�����PID�ṹ��
pid_k pidx,pidy,pidvx,pidvy;

int main(void)
{ 
	init();
	
	while(1)
	{
		DataScope();
		oled_show();
	}
} 

//��ʼ����ΪӲ�����ֺ��������
void init()
{
	delay_init();																	//��ʱ��ʼ��
	delay_ms(1000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//�жϳ�ʼ��
	USART1_init(115200,0);//uart_init,remap enable			//����1��ʼ��,openmvͨ��
	USART2_init(115200,0);//uart_init,remap enable			//����2��ʼ��,��λ��ͨ��
	OLED_Init();																	//oled��ʼ������ʾ�������
	SGear_Init();																	//�����ʼ��
	Timer1_Init(49,7199);															//��ʱ��1�жϳ�ʼ�������жϽ��п���
	MY_EXTI_Init();															//�����ⲿ�жϳ�ʼ���������ⲿ�˻�����
	//adc��ʼ��,pwm��ʼ��,��������ʼ��,�����ʼ��
	delay_ms(1000);

	//�ṹ�壬KP,KI,KD,Ŀ��ֵ
	PIDinit(&pidx,5,0,260,globalValue_x);
	pidx.deadband=deadbandValue,pidx.UP=0;
	PIDinit(&pidy,5,0,260,globalValue_y);
	pidy.deadband=deadbandValue,pidy.UP=0;
	
	PIDinit(&pidvx,40,0,0,1);
	pidvx.deadband=0,pidvx.UP=0;
	PIDinit(&pidvy,40,0,0,1);
	pidvy.deadband=0,pidvy.UP=0;	//pid��ʼ��
	//openmv_Init();
}

//�����ٶ�PID�����
void controlSpeed(void)
{
	//�����ٶ�
	SpeedX = pointX - Last_pointX;
	SpeedY = pointY - Last_pointY;
	
	chang_SpeedX = updatePID(SpeedX,&pidvx,2);
	chang_SpeedY = updatePID(SpeedY,&pidvy,2);
	
	//����˴�λ�ã������´μ����ٶ�
	Last_pointX = pointX;
	Last_pointY = pointY;
}

//����λ��PID�����
void controlPoint(void)
{
	//��ȡpid���
	chang_PointX = updatePID(pointX,&pidx,1);
	chang_PointY = updatePID(pointY,&pidy,1);

	//������������ͬʱ����ſɽ���������һ��������Ӱ����һ��
	//pid ���������ȼ�����
	//���ڼ�������Ŀ����ʱ��
	if(wucha(pointX,globalValue_x,deadbandValue)&&wucha(pointY,globalValue_y,deadbandValue))
	{
		count_Flag++;
		chang_PointX=0;
		chang_PointY=0;
		
		if(wucha(SpeedX,0,2)&&wucha(SpeedY,0,2))
		{
			//SGear0_Level=pwmoutX;
			//SGear1_Level=pwmoutY;
		}
	}
		
}

//���Ŀ��ƴ���
//�ɵ���λ�����ٶȿ��ƽ���
//��������ʵ��Ҳ��������
void control()
{
		Flag_ms++;
		Flag2_ms++;
	
		//�ı��˶�ģʽ
		chang_Mode();
		//�ı�Ŀ��㣬���������Ⱥ�˳�򣬲��������޸�
		chang_point();
	
		pointX = USART1_RX_BUF[1]*256+USART1_RX_BUF[2];
		pointY = USART1_RX_BUF[3]*256+USART1_RX_BUF[4];
	
		if(Flag_ms==2)			//5ms*2
		{		
			Flag_ms=0;
			controlPoint();
		}
		
		if(Flag2_ms==20)			//5ms*20
		{		
			Flag2_ms=0;
			controlSpeed();
		}
		
		//���X�������Σ���ôX�������PID���Ϊ0����
		chang_PointX = chang_PointX*FlagX*FlagPoint;
		chang_SpeedX = chang_SpeedX*FlagX*FlagSpeed;
		
		//���Y�������Σ���ôY�������PID���Ϊ0����
		chang_PointY = chang_PointY*FlagY*FlagPoint;	
		chang_SpeedY = chang_SpeedY*FlagY*FlagSpeed;
		
		if(control_Flag==1)
		{
			SGear_move(0,-chang_PointX-chang_SpeedX);
			SGear_move(1,-chang_PointY-chang_SpeedY);
		}
}

//openmv��ʼ������ȡ9��ֵ������б�Խ�������
//���������������λ�ÿɼ��������6����λ��
//ʵ���ϲ�׼
void openmv_Init(void)
{
	int i;
	
	//���յ��bug��Ϊһֱ�ڷ��͵�����굫��flagû�����
	//���������ֶ����һ��
	USART1_RX_flag=0;
	USART1_RX_count=0;
	
	//���ⷢ��һЩ����������openmv���;Ÿ�������
	USART1_send((u8*)0xaa);
	//�ȴ��������
	while(USART1_RX_flag==0);
		//USART2_send_len(USART1_RX_BUF,38);
	
	//�����ν����жϷ��ú���С�����긲��
	USART_Cmd(USART1, DISABLE); 
	for(i=1;i<36;i+=2){
		point_Array[(i/2)]=USART1_RX_BUF[i]*256+USART1_RX_BUF[i+1];
	}
	USART1_RX_flag=0;
	USART1_RX_count=0;
	USART_Cmd(USART1, ENABLE); 
	chang_point();
}

//1-9ģʽ�ֱ����9���������
void chang_point()
{
	if(point_Flag>0&&point_Flag<10)
	{
		globalValue_x=point_Array[point_Flag-1];
		globalValue_y=point_Array[point_Flag+8];
	}
	
	//P10 �ǹ��ɵ�
	if(point_Flag==10)
	{
		globalValue_x=0;
		globalValue_y=0;
	}
	else if(point_Flag==11)
	{
		globalValue_x=134;
		globalValue_y=114;
	}
	else if(point_Flag==12)
	{
		globalValue_x=139;
		globalValue_y=142;
	}
	else if(point_Flag==13)
	{
		globalValue_x=168;
		globalValue_y=159;
	}
	else if(point_Flag==14)
	{
		globalValue_x=205;
		globalValue_y=149;
	}
	else if(point_Flag==15)
	{
		globalValue_x=212;
		globalValue_y=119;
	}
	else if(point_Flag==16)
	{
		globalValue_x=199;
		globalValue_y=83;
	}
	else if(point_Flag==17)
	{
		globalValue_x=170;
		globalValue_y=77;
	}
	else if(point_Flag==18)
	{
		globalValue_x=137;
		globalValue_y=86;
	}
	else if(point_Flag==19)
	{
		globalValue_x=0;
		globalValue_y=0;
	}
	else if(point_Flag==20)
	{
		globalValue_x=0;
		globalValue_y=0;
	}
	else if(point_Flag==21)
	{
		globalValue_x=0;
		globalValue_y=0;
	}
	else
	{
		globalValue_x=point_Array[4];
		globalValue_y=point_Array[13];
	}
	
	chang_Global_Value(globalValue_x,&pidx);
	chang_Global_Value(globalValue_y,&pidy);
}

//�ú���������ʾ����ɱ���Ҫ��
void chang_Mode()
{
	//��ʼ��Ĭ��Ϊ��ģʽ����ģʽ���Կ�����������
	if(Mode_move==0)
	{
		
	}
	//����ģʽ1
	if(Mode_move==1)
	{
		//����2�����Ŀ����Ϊ2
		//ֻҪ�Ĳ������У�����ִ�еĺ�������ݲ����޸�Ŀ��ֵ
		if(point_Flag!=2)
		{
			point_Flag=2;
			count_Flag=0;
		}
		
	}
	
	//����ģʽ2
	if(Mode_move==2)
	{
		//���ڼ���ڸõ�ͣ��ʱ���Ƿ�����
		if(count_Flag==ms20)
		{
			//���ͣ����Ϊ1������ʱ�䣬����һ��5
			if(point_Flag==1)
			{
				point_Flag=5;
				count_Flag=0;
			}
			else if(point_Flag==5)
			{
				point_Flag=5;
				count_Flag=0;
			}
			//������ģʽ2ָ���ĵ��ϣ���ô���ǻص�ģʽ���ĳ�ʼ��1
			else
			{
				point_Flag=1;
				count_Flag=0;
			}
		}
	}
	
	//����ģʽ3
	if(Mode_move==3)
	{
		//���ﵱǰĿ��㹻2s��
		if(count_Flag==ms20)
		{
			//�ڵ�1����ʱ����ָ������һ��
			if(point_Flag==1)
			{
				point_Flag=4;
				count_Flag=0;
			}
			//���ڵ�1�ж��ڵ�4�Ļ���ȥ��һ��
			else if(point_Flag==4)
			{
				point_Flag=5;
				count_Flag=0;
			}
			//ͣ��������5
			else if(point_Flag==5)
			{
				point_Flag=5;
				count_Flag=0;
			}
			//������ָ�������˶���ģʽ��ʼ��
			else
			{
				point_Flag=1;
				count_Flag=0;
			}
		}
	}

	//����ģʽ4
	if(Mode_move==4)
	{
		if(count_Flag==ms20)
		{
			//��1����ʱ����ȥָ������һĿ���9
			if(point_Flag==1)
			{
				point_Flag=10;
				count_Flag=0;
			}
			if(point_Flag==10)
			{
				point_Flag=9;
				count_Flag=0;
			}
			//ͣ��������9
			if(point_Flag==9)
			{
				point_Flag=9;
				count_Flag=0;
			}
			else
			{
				point_Flag=1;
				count_Flag=0;
			}
		}
	}
	
	
	//����ģʽ5
	if(Mode_move==5)
	{
		if(count_Flag==ms20)
		{
			//�ڵ�1����ʱ����ָ������һ��
			if(point_Flag==1)
			{
				point_Flag=2;
				count_Flag=0;
			}
			//���ڵ�1�ж��ڵ�4�Ļ���ȥ��һ��
			else if(point_Flag==2)
			{
				point_Flag=6;
				count_Flag=0;
			}
			//���ڵ�1�ж��ڵ�4�Ļ���ȥ��һ��
			else if(point_Flag==6)
			{
				point_Flag=9;
				count_Flag=0;
			}
			else if(point_Flag==9)
			{
				point_Flag=9;
				count_Flag=0;
			}
			//������ָ�������˶���ģʽ��ʼ��
			else
			{
				point_Flag=1;
				count_Flag=0;
			}
		}
	}
	
	//����ģʽ6
	//��A����B����-C-D����Dͣ����
	if(Mode_move==6)
	{
		if(count_Flag==ms20)
		{
			//�ڵ�1����ʱ����ָ������һ��
			if(point_Flag==A)
			{
				point_Flag=B;
				count_Flag=0;
			}
			//���ڵ�1�ж��ڵ�4�Ļ���ȥ��һ��
			else if(point_Flag==B)
			{
				point_Flag=C;
				count_Flag=0;
			}
			//���ڵ�1�ж��ڵ�4�Ļ���ȥ��һ��
			else if(point_Flag==C)
			{
				point_Flag=D;
				count_Flag=0;
			}
			else if(point_Flag==D)
			{
				point_Flag=D;
				count_Flag=0;
			}
			//������ָ�������˶���ģʽ��ʼ��
			else
			{
				point_Flag=A;
				count_Flag=0;
			}
		}
	}
	
	
	//����ģʽ7�����Ƶ�5
	if(Mode_move==7)
	{
		if(count_Flag==ms20)
		{
			//�ڵ�1����ʱ����ָ������һ��
			if(point_Flag==4)
			{
				point_Flag=11;
				count_Flag=0;
			}
			//���ڵ�1�ж��ڵ�4�Ļ���ȥ��һ��
			else if(point_Flag==11)
			{
				point_Flag=12;
				count_Flag=0;
			}
			//���ڵ�1�ж��ڵ�4�Ļ���ȥ��һ��
			else if(point_Flag==12)
			{
				point_Flag=13;
				count_Flag=0;
			}
			else if(point_Flag==13)
			{
				point_Flag=14;
				count_Flag=0;
			}
			else if(point_Flag==14)
			{
				point_Flag=15;
				count_Flag=0;
			}
			else if(point_Flag==15)
			{
				point_Flag=16;
				count_Flag=0;
			}
			else if(point_Flag==16)
			{
				point_Flag=17;
				count_Flag=0;
			}
			else if(point_Flag==17)
			{
				point_Flag=18;
				count_Flag=0;
			}
			else if(point_Flag==18)
			{
				point_Flag=11;
				count_Flag=0;
			}
			//������ָ�������˶���ģʽ��ʼ��
			else
			{
				point_Flag=4;
				count_Flag=0;
			}
		}
	}
	
	if(Mode_move==8)
	{
		if(count_Flag==ms20)
		{
		}
	}
}

