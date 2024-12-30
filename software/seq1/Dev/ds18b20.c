#include "ds18b20.h"

sbit DQ = P1^4;

//��ʱ
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0;i<12;i++);
	}
}

//дһ���ֽ�
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//��һ���ֽ�
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//��ʼ�ź�
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}

uint16_t Read_Temperature(void)
{
	uint8_t high, low;
	
	init_ds18b20();
	Write_DS18B20(0xcc);//ѡ����
	Write_DS18B20(0x44);//ת���¶�
	
	init_ds18b20();
	Write_DS18B20(0xcc);//ѡ����
	Write_DS18B20(0xbe);//��ȡ�¶�
	
	low = Read_DS18B20();
	high = Read_DS18B20();
	
	return (high<<8)|low;
}


























