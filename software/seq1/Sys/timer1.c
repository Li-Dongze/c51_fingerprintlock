#include "timer1.h"

void Timer0_Init(void)		//1����@12.000MHz
{
	//AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
    TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TMOD |= 0x01;			//���ö�ʱ��ģʽ
	TL0 = 0x66;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ
	ET0 = 1;
}


























