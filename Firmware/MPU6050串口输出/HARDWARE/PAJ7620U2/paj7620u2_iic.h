#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H

#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//PAJ7620U2 IIC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/7/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


#define GS_SDA_IN()  {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=8<<12;}
#define GS_SDA_OUT() {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=3<<12;}

//IO��������	 
#define GS_IIC_SCL    PAout(2) 		//SCL
#define GS_IIC_SDA    PAout(3) 		//SDA	 
#define GS_READ_SDA   PAin(3) 		//����SDA 

u8 GS_Write_Byte(u8 REG_Address,u8 REG_data);
u8 GS_Read_Byte(u8 REG_Address);
u8 GS_Read_nByte(u8 REG_Address,u16 len,u8 *buf);
void GS_i2c_init(void);
void GS_WakeUp(void);

#endif


