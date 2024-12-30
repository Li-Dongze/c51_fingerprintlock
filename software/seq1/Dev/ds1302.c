#include "ds1302.h"

sbit SCK = P1^7;
sbit SDA = P2^3;
sbit RST = P1^3;

//SPIдһ���ֽ�
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//��һ����ַдһ���ֽ�
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//��һ����ַ��һ���ֽ�
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

void Set_Rtc(uint8_t *ucRtc)
{
	uint8_t temp;
	
	Write_Ds1302_Byte(0x8e, 0x00);//����д
	
	temp = (((ucRtc[0]/10)<<4)|(ucRtc[0]%10));//Сʱ
	Write_Ds1302_Byte(0x84, temp);
	temp = (((ucRtc[1]/10)<<4)|(ucRtc[1]%10));//����
	Write_Ds1302_Byte(0x82, temp);
	temp = (((ucRtc[2]/10)<<4)|(ucRtc[2]%10));//��
	Write_Ds1302_Byte(0x80, temp);
	
	Write_Ds1302_Byte(0x8e, 0x80);//��ֹд
}	

void Read_Rtc(uint8_t *ucRtc)
{
	uint8_t temp;
	
	temp = Read_Ds1302_Byte(0x85);
	ucRtc[0] = (((temp>>4)*10)+(temp&0x0f));//Сʱ
	temp = Read_Ds1302_Byte(0x83);
	ucRtc[1] = (((temp>>4)*10)+(temp&0x0f));//����
	temp = Read_Ds1302_Byte(0x81);
	ucRtc[2] = (((temp>>4)*10)+(temp&0x0f));//��
}























