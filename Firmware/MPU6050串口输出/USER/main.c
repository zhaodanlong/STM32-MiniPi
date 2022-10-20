#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "mpu6050.h"
#include "usmart.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

int main(void)
{
    u8 t=0;
	u8 key=0;
	u8 GetData=1;
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;					//�¶�
	
    HAL_Init();                    	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9); //����ʱ��,72M
    delay_init(72);                 //��ʼ����ʱ����
  	uart_init(115200);				//��ʼ������115200
    LED_Init();                     //��ʼ��LED 
	  KEY_Init();						//��ʼ������
  	usmart_init(72);                //USMART��ʼ��
	  printf("ALIENTEK NANO STM32\r\n");
    printf("MPU6050 TEST\r\n");
    while(mpu_dmp_init())//MPU DMP��ʼ��
	{
	   printf("MPU6050 Error!!!\r\n");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
		
	}
    printf("MPU6050 OK\r\n");
	while(1)
	{
		key=KEY_Scan(0);
		if(key==WKUP_PRES)
		{
			GetData=!GetData;
			if(GetData)printf("GETDATA ON\r\n");
			else printf("GETDATA OFF\r\n");
		}
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			if(GetData)//GetData=0ʱ ����USMART����MPU6050�Ĵ���
			{
				
				if((t%10)==0)
				{ 		
					//tempֵ
					if(temp<0)
					{
						temp=-temp;		//תΪ����
						printf(" Temp:  -%d.%dC\r\n",temp/100,temp%10);
					}else
						printf(" Temp:  %d.%dC\r\n",temp/100,temp%10);
						
					//pitchֵ
					temp=pitch*10;			
					if(temp<0)
					{
						temp=-temp;		//תΪ����
						printf(" Pitch: -%d.%dC\r\n",temp/10,temp%10);
					}else
						printf(" Pitch:  %d.%dC\r\n",temp/10,temp%10);
					
					//rollֵ
					temp=roll*10;		
					if(temp<0)
					{
						temp=-temp;		//תΪ����
						printf(" Roll:  -%d.%dC\r\n",temp/10,temp%10);
					}else
						printf(" Roll:  %d.%dC\r\n",temp/10,temp%10);
					
					//yawֵ
					temp=yaw*10;
					if(temp<0)
					{
						temp=-temp;		//תΪ����
						printf(" Yaw:  -%d.%dC\r\n",temp/10,temp%10);
					}else
						printf(" Yaw:   %d.%dC\r\n",temp/10,temp%10);
				
					printf("\r\n");
					t=0;
					LED0=!LED0;//LED��˸
				}

			}
			
		}
		t++; 		

	}
}

