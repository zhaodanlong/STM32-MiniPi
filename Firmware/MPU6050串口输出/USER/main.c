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
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;					//温度
	
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9); //设置时钟,72M
    delay_init(72);                 //初始化延时函数
  	uart_init(115200);				//初始化串口115200
    LED_Init();                     //初始化LED 
	  KEY_Init();						//初始化按键
  	usmart_init(72);                //USMART初始化
	  printf("ALIENTEK NANO STM32\r\n");
    printf("MPU6050 TEST\r\n");
    while(mpu_dmp_init())//MPU DMP初始化
	{
	   printf("MPU6050 Error!!!\r\n");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
		
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
			temp=MPU_Get_Temperature();	//得到温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			if(GetData)//GetData=0时 用于USMART调试MPU6050寄存器
			{
				
				if((t%10)==0)
				{ 		
					//temp值
					if(temp<0)
					{
						temp=-temp;		//转为正数
						printf(" Temp:  -%d.%dC\r\n",temp/100,temp%10);
					}else
						printf(" Temp:  %d.%dC\r\n",temp/100,temp%10);
						
					//pitch值
					temp=pitch*10;			
					if(temp<0)
					{
						temp=-temp;		//转为正数
						printf(" Pitch: -%d.%dC\r\n",temp/10,temp%10);
					}else
						printf(" Pitch:  %d.%dC\r\n",temp/10,temp%10);
					
					//roll值
					temp=roll*10;		
					if(temp<0)
					{
						temp=-temp;		//转为正数
						printf(" Roll:  -%d.%dC\r\n",temp/10,temp%10);
					}else
						printf(" Roll:  %d.%dC\r\n",temp/10,temp%10);
					
					//yaw值
					temp=yaw*10;
					if(temp<0)
					{
						temp=-temp;		//转为正数
						printf(" Yaw:  -%d.%dC\r\n",temp/10,temp%10);
					}else
						printf(" Yaw:   %d.%dC\r\n",temp/10,temp%10);
				
					printf("\r\n");
					t=0;
					LED0=!LED0;//LED闪烁
				}

			}
			
		}
		t++; 		

	}
}

