#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
int main(void)
{
//	float t=0;
	delay_init();
	LED_Init();//LED初始化
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	LED=0;
	while(1)
	{
		  LCD_ShowString(12,25,"Hello WOrld",RED,BLUE,24,0);
//		LCD_ShowString(10,20,"LCD_W:",RED,WHITE,16,0);
//		LCD_ShowIntNum(58,20,LCD_W,3,RED,WHITE,16);
//		LCD_ShowString(10,40,"LCD_H:",RED,WHITE,16,0);
//		LCD_ShowIntNum(58,40,LCD_H,3,RED,WHITE,16);
//		LCD_ShowFloatNum1(10,60,t,4,RED,WHITE,16);
//		t+=0.1;
//		LCD_ShowPicture(100,20,40,40,gImage_1);
	}
}

