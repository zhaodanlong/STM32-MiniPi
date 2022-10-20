#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//PAJ7620U2 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/7/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//ѡ��PAJ7620U2 BANK����
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0�Ĵ�������
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1�Ĵ�������
	}
			
}

//PAJ7620U2����
u8 paj7620u2_wakeup(void)
{ 
	u8 data=0x0a;
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	data = GS_Read_Byte(0x00);//��ȡ״̬
	if(data!=0x20) return 0; //����ʧ��
	
	return 1;
}

//PAJ7620U2��ʼ��
//����ֵ��0:ʧ�� 1:�ɹ�
u8 paj7620u2_init(void)
{
	u8 i;
	u8 status;
	
	GS_i2c_init();//IIC��ʼ��
    status = paj7620u2_wakeup();//����PAJ7620U2
	if(!status) return 0;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//��ʼ��PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	return 1;
}

//���˵�
void paj7620u2_test_ui(void)
{
	printf("KEY1:   Gesture test   ");//����ʶ�����
    printf("KEY0:   Ps test        ");//�ӽ��������	
	printf("\r\n");
}

//����ʶ�����
void Gesture_test(void)
{
	u8 i;
    u8 status;
	u8 data[2]={0x00};
	u16 gesture_data;
	u8 ledflash=0;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	i=0;

	printf("Gesture test\r\n");
	
	while(1)
	{
	
        status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//��ȡ����״̬			
		if(!status)
		{   
			gesture_data =(u16)data[1]<<8 | data[0];
			if(gesture_data) 
			{
				switch(gesture_data)
				{
					case GES_UP:               printf("Up\r\n");            ledflash=1;      break; //����
					case GES_DOWM:             printf("Dowm\r\n");          ledflash=1;      break; //����
					case GES_LEFT:             printf("Left\r\n");          ledflash=1;      break; //����
					case GES_RIGHT:            printf("Right\r\n");         ledflash=1;      break; //����
					case GES_FORWARD:          printf("Forward\r\n");       ledflash=1;      break; //��ǰ
					case GES_BACKWARD:         printf("Backward\r\n");      ledflash=1;      break; //���
					case GES_CLOCKWISE:        printf("Clockwise\r\n");     ledflash=1;      break; //˳ʱ��
					case GES_COUNT_CLOCKWISE:  printf("AntiClockwise\r\n"); ledflash=1;      break; //��ʱ��
					case GES_WAVE:             printf("Wave\r\n");          ledflash=1;      break; //�Ӷ�
					default:  ledflash=0; break;	
				}	
                if(ledflash)//DS1��˸
				{   
					LED1=0;delay_ms(80);LED1=1;delay_ms(80);
					LED1=0;delay_ms(80);LED1=1;delay_ms(80);
					ledflash=0;
				}						
			}
			
		}
		delay_ms(50);
		i++;
		if(i==5)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}		   
	}
}

//�ӽ�������
void Ps_test(void)
{
    u8 i=0;
	u8 t=0;
	u8 data[2]={0x00};
	u8 obj_brightness=0;
	u16 obj_size=0;
	
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<PROXIM_SIZE;i++)
	{
		GS_Write_Byte(proximity_arry[i][0],proximity_arry[i][1]);//�ӽ����ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	i=0;

    printf("Ps test\r\n");
	
	while(1)
	{	

		
		obj_brightness = GS_Read_Byte(PAJ_GET_OBJECT_BRIGHTNESS);//��ȡ��������
		data[0] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_1);//��ȡ�����С
		data[1] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_2);
		obj_size = ((u16)data[1] & 0x0f)<<8 | data[0];

		
		delay_ms(100);
		
		t++;
		if(t==10)//ÿ1S��ӡ�������
		{
			t=0;
			printf("Brightness: %d\r\n",obj_brightness);
            printf("Size: %d\r\n",obj_size);
			
		}
		i++;
		if(i==5)
		{
		    LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}
	}
	
}
//PAJ7620U2����������
void paj7620u2_sensor_test(void)
{   
	 u8 i=0;
	 u8 key;
	
     paj7620u2_test_ui();//���˵���ʾ
	 while(1)
	 {
		 key = KEY_Scan(0);//����ɨ��
		 if(key)
		 {
			 switch(key)
			 {
				 case KEY1_PRES:  Gesture_test();   break;//���Ƽ��ģʽ
				 case KEY0_PRES:  Ps_test();        break;//�ӽ����ģʽ  
			 }
		 }
		delay_ms(50);
		i++;
		if(i==5)
		{
		    LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}
		 
	 }
}
