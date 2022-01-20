#ifndef __OLED_H
#define __OLED_H
#include "sys.h"
/**************************************************************************
���ܣ�ģ��I2C��ʾoled
ʱ�䣺20190712 
���ߣ�����
**************************************************************************/
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//OLED�����ú���
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   				   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);

void OLED_WR_Byte(u8 dat,u8 cmd);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);

#endif
