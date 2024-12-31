#include "uart.h"

void Uart1_Init(void)	//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
//	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
//	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x20;		//���ö�ʱ��ģʽ
	TL1 = 0xFD;			//���ö�ʱ��ʼֵ
	TH1 = 0xFD;			//���ö�ʱ����ֵ
	ET1 = 0;			//��ֹ��ʱ���ж�
	TR1 = 1;			//��ʱ��1��ʼ��ʱ
	ES = 1;				//ʹ�ܴ���1�ж�
}

void UART_SendByte(uint8_t Byte)
{
    SBUF = Byte;
    while(TI == 0);
    TI = 0;
}

void UART_SendString(uint8_t *str)
{
    while(*str != '\0')
    {
        UART_SendByte(*(str++));
    }   
}





