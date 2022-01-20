#include "bluebooth.h"

void bluebooth_Init(int bound){
	USART1_init(bound,0);//=====��ʼ������1
	USART1_send("test\r\n");
	
	//���������Ƿ�ɹ�
//	if(test()==1){
//		//��������
//		int i =search();
//		//�Ƿ�����������
//		if(i>0){
//			//�����豸1�����Ƿ�ɹ�
//			if(contact(1)==1){
//				send("successful\r\n");
//			}else{
//				send("failed contact(1)\r\n");
//			}
//		}else{
//			send("failed search()\r\n");
//		}
//	}else{
//		send("failed test()\r\n");
//	}
}

//���ߣ�����
//ʱ�䣺20190625
//�������������Ͳ���ָ��ж�����������
//��������1������Ϊ0
int test(){
	int i;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x41;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x54;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0D;
	while((USART1->SR&0X40)==0);
	USART1->DR = 0x0A;
	//�ȴ����գ����ܵ�����ѭ��
	while(USART1_RX_flag!=1);
	if(USART1_RX_BUF[0]=='O'&&USART1_RX_BUF[1]=='K'){
		i = 1;
	}else{
		i = 0;
	}
	USART1_RX_flag = 0 ;
	USART1_RX_count = 0 ;
	return i;
}

//���ߣ�����
//ʱ�䣺20190625
//�������������������豸����
//������ĳ���ɹ����������ķ�����䣺
//OK
//+INQS
//+INQ:1 0x00158700768D
//+INQE
//Devices Found 1
//+Connecting  0x00158700768D
//+Connected  0x00158700768D
int search(){
	int i = 0;
	USART1_send("AT+INQ\r\n");
	while(USART1_RX_flag!=1);
	if(USART1_RX_BUF[0]=='O'&&USART1_RX_BUF[1]=='K'){
		USART1_RX_flag = 0 ;
		USART1_RX_count = 0 ;
		//ͨ������ʽ��֪Devices Found 1 ������������
		while(USART1_RX_BUF[0]!='D'){
			//ͨ��ͳ��+������
			if(USART1_RX_flag==1&&USART1_RX_BUF[0]=='+'){
				i++;
				//����������+�޸ģ���ֹ�ظ�����
				USART1_RX_BUF[0]='-';
				USART1_RX_flag = 0 ;
				USART1_RX_count = 0 ;
			}
		}
	}
	return i-2;
}

//���ߣ�����
//ʱ�䣺20190625
//��������������Ҫ���ڼ����豸������Ϊ��
//������ĳ���ɹ����������ķ�����䣺
//������ĳ���ɹ��������ķ�����䣺
//+Connecting  0x00158700768D
//+Connected  0x00158700768D
int contact(int i){
	switch (i){
		case 1: USART1_send("AT+CONN1\r\n"); break;
		case 2: USART1_send("AT+CONN2\r\n"); break;
		case 3: USART1_send("AT+CONN3\r\n"); break;
		case 4: USART1_send("AT+CONN4\r\n"); break;
		case 5: USART1_send("AT+CONN5\r\n"); break;
		case 6: USART1_send("AT+CONN6\r\n"); break;
		case 7: USART1_send("AT+CONN7\r\n"); break;
		default: USART1_send("AT+CONN1\r\n"); break;
	}
	while(USART1_RX_flag!=1);
	//������+��֪�����
	if(USART1_RX_BUF[0]=='+'){
		USART1_RX_flag = 0 ;
		USART1_RX_count = 0 ;
		while(USART1_RX_flag!=1);
		if(USART1_RX_BUF[0]=='+'){
			i=1;
		}else{
			i=0;
		}
		USART1_RX_flag = 0 ;
		USART1_RX_count = 0 ;
	}
	return i;
}

//���ߣ�����
//ʱ�䣺20190630  1853���
//��λ��������λ����Э��
void protocol(void){
	//�ж��Ƿ�������
	if(USART1_RX_flag==1){
		//�ж��ǲ��ǿ�������C
		if(USART1_RX_BUF[0]=='C'){
			//�ж��ǲ��ǿ�������
			if(USART1_RX_BUF[1]=='0'){
				if(USART1_RX_BUF[2]=='0'){
					//����������
					//Flag_Stop=0;
				}else if(USART1_RX_BUF[2]=='1'){
					//������ֹͣ
					//Flag_Stop=1;
				}else{
					//null;
				}
			}
			//�ж��ǲ��ǽǶ���ת����
			else if(USART1_RX_BUF[1]=='1'){
				//�ж�����ת
				if(USART1_RX_BUF[2]=='0'){
					//��ֵ���ٶȸ��Ƕ��ٶȱ���
					//speed = (float)USART1_RX_BUF[3];
				}else if(USART1_RX_BUF[2]=='1'){
					//��ֵ���ٶȸ��Ƕ��ٶȱ���
					//speed = 0 - (float)USART1_RX_BUF[3];
				}else{
					//null;
				}
			}
			//�ж��ǲ���λ����ת����
			else if(USART1_RX_BUF[1]=='2'){
				if(USART1_RX_BUF[2]=='0'){
					//velocity = (float)USART1_RX_BUF[3];
				}else if(USART1_RX_BUF[2]=='1'){
					//velocity = 0 - (float)USART1_RX_BUF[3];
				}else{
					//null;
				}
			}
			//�ж��ǲ��ǽǶ�PID��������
			else if(USART1_RX_BUF[1]=='3'){
				//�ж����ĸ�������KP,KI,KD
				if(USART1_RX_BUF[2]=='P'){
					//Balance_KP0= getNumber(3);
				}else if(USART1_RX_BUF[2]=='I'){
					//Balance_KI0= getNumber(3);					
				}else if(USART1_RX_BUF[2]=='D'){
					//Balance_KD0= getNumber(3);
				}else{
					//null;
				}				
			}			
			//�ж��ǲ���λ��PID��������
			else if(USART1_RX_BUF[1]=='4'){			
				if(USART1_RX_BUF[2]=='P'){					
					//Position_KP0= getNumber(3);
				}else if(USART1_RX_BUF[2]=='I'){
					//Position_KI0= getNumber(3);					
				}else if(USART1_RX_BUF[2]=='D'){
					//Position_KD0= getNumber(3);
				}else{
					//null;
				}				
			}else{			
			}		
		}else{		
		}
		//���ڽ��ձ�־����
		USART1_RX_flag =0;
		USART1_RX_count=0;
	}
}

//			switch ((int)USART1_RX_BUF[1])
//				case 1: break;
//				case 2: break;
//				case 3: break;
//				case 4: break;

//���ߣ�����
//ʱ�䣺20190630  18xx���
//��ȡЭ�������ֲ��ֵ�ֵ����$��־Ϊ����λ
//һ�����ֲ�����ʼλΪ3������ֵΪfloat
float getNumber(u8 i){
	float result=0;
	while(USART1_RX_BUF[i]!='$'){
		result = (float)USART1_RX_BUF[i]+result*10;
		i++;
	}
	return result;
}

//���ߣ�����
//ʱ�䣺20190630  18xx���
//���ڵ��ԣ��鿴���յ����
void returnProtocol(void){
	//�ж��Ƿ�������
	if(USART1_RX_flag==1){
		int i=0;
		int j;
		j=USART1_RX_count;
		while((USART1->SR&0X40)==0);
		USART1->DR = 'R';
		for(;i<j;i++){
			while((USART1->SR&0X40)==0);
			USART1->DR = USART1_RX_BUF[i];
		}
		while((USART1->SR&0X40)==0);
		USART1->DR = 0x0D;
		while((USART1->SR&0X40)==0);
		USART1->DR = 0x0A;
	}
}
