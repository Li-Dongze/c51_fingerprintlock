#include "i2c.h"

sbit scl = P2^7;
sbit sda = P2^6;

//I2C��ʱ
static void I2C_Delay(unsigned char n)
{
    do
    {
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();_nop_();		
    }
    while(n--);      	
}

//��ʼ�ź�
void I2CStart(void)
{
    sda = 1;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 0;
	I2C_Delay(DELAY_TIME);
    scl = 0;    
}

//�����ź�
void I2CStop(void)
{
    sda = 0;
    scl = 1;
	I2C_Delay(DELAY_TIME);
    sda = 1;
	I2C_Delay(DELAY_TIME);
}

//����һ�ֽ�����
void I2CSendByte(unsigned char byt)
{
    unsigned char i;
	
    for(i=0; i<8; i++){
        scl = 0;
		I2C_Delay(DELAY_TIME);
        if(byt & 0x80){
            sda = 1;
        }
        else{
            sda = 0;
        }
		I2C_Delay(DELAY_TIME);
        scl = 1;
        byt <<= 1;
		I2C_Delay(DELAY_TIME);
    }
	
    scl = 0;  
}

//��ȡһ�ֽ�����
unsigned char I2CReceiveByte(void)
{
	unsigned char da;
	unsigned char i;
	for(i=0;i<8;i++){   
		scl = 1;
		I2C_Delay(DELAY_TIME);
		da <<= 1;
		if(sda) 
			da |= 0x01;
		scl = 0;
		I2C_Delay(DELAY_TIME);
	}
	return da;    
}

//�����ȴ��ӻ�Ӧ��
unsigned char I2CWaitAck(void)
{
	unsigned char ackbit;
	
    scl = 1;
	I2C_Delay(DELAY_TIME);
    ackbit = sda; 
    scl = 0;
	I2C_Delay(DELAY_TIME);
	
	return ackbit;
}

//��������Ӧ��/��Ӧ��
void I2CSendAck(unsigned char ackbit)
{
    scl = 0;
    sda = ackbit; 
	I2C_Delay(DELAY_TIME);
    scl = 1;
	I2C_Delay(DELAY_TIME);
    scl = 0; 
	sda = 1;
	I2C_Delay(DELAY_TIME);
}

/*----------------------------------PCF8591-------------------------------------*/

//uint8_t PCF8591_Adc(uint8_t channel_control)
//{
//	uint8_t temp;
//	
//	I2CStart();
//	I2CSendByte(0x90);//����������ַ��׼��д
//	I2CWaitAck();
//	
//	I2CSendByte(channel_control);//�������ȷ�����ĸ�ͨ��
//	I2CWaitAck();
//	
//	I2CStart();
//	I2CSendByte(0x91);//����������ַ��׼����
//	I2CWaitAck();
//	
//	temp = I2CReceiveByte();
//	I2CSendAck(1);//�������ͷ�Ӧ��
//	I2CStop();
//	
//	return temp;
//}

//void PCF8591_Dac(uint8_t tans_data)
//{
//	I2CStart();
//	I2CSendByte(0x90);//����������ַ��׼��д
//	I2CWaitAck();
//	
//	I2CSendByte(0x41);//�������ʹ��
//	I2CWaitAck();
//	
//	I2CSendByte(tans_data);
//	I2CWaitAck();
//	I2CStop();
//}

/*----------------------------------EEPROM-------------------------------------*/

void EEPROM_Write(uint8_t *EEPROM_String, uint8_t addr, uint8_t num)
{
	I2CStart();
	I2CSendByte(0xA0);//����������ַ��׼��д
	I2CWaitAck();
	
	I2CSendByte(addr);//Ҫд�����ʼ��ַ
	I2CWaitAck();
	
	while(num--)
	{
		I2CSendByte(*EEPROM_String++);
		I2CWaitAck();
		I2C_Delay(200);
	}
	I2CStop();
}	

void EEPROM_Read(uint8_t *EEPROM_Buff, uint8_t addr, uint8_t num)
{
	I2CStart();
	I2CSendByte(0xA0);//����������ַ��׼��д
	I2CWaitAck();
	
	I2CSendByte(addr);//Ҫ��ȡ����ʼ��ַ
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0xA1);//����������ַ��׼����
	I2CWaitAck();
	
	while(num--)
	{
		*EEPROM_Buff++ = I2CReceiveByte();
		if(num)	I2CSendAck(0);
		else I2CSendAck(1);
	}
	I2CStop();
}



























