#include "key.h"

//uint8_t Key_Read_BTN(void)
//{
//	uint8_t Key_Value;
//	
//	if(P33 == 0)	Key_Value = 4;
//	else if(P32 == 0)	Key_Value = 5;
//	else if(P31 == 0)	Key_Value = 6;
//	else if(P30 == 0)	Key_Value = 7;
//	else Key_Value = 0;
//	
//	return Key_Value;
//}	

uint8_t Key_Read(void)
{
	uint8_t Key_Value;
	uint16_t Key_New;
	
	P13 = 0; P12 = 1; P11 = 1; P10 = 1;
	Key_New = P0&0x0f;
	P13 = 1; P12 = 0; P11 = 1; P10 = 1;
	Key_New = (Key_New<<4)|(P0&0x0f);
	P13 = 1; P12 = 1; P11 = 0; P10 = 1;
	Key_New = (Key_New<<4)|(P0&0x0f);
	P13 = 1; P12 = 1; P11 = 1; P10 = 0;
	Key_New = (Key_New<<4)|(P0&0x0f);
	
	switch(~Key_New)
	{
		case 0x8000:	Key_Value = 13;	break;
		case 0x4000:	Key_Value = 9;	break;
		case 0x2000:	Key_Value = 5;	break;
		case 0x1000:	Key_Value = 1;	break;
		
		case 0x0800:	Key_Value = 14;	break;
		case 0x0400:	Key_Value = 10;	break;
		case 0x0200:	Key_Value = 6;	break;
		case 0x0100:	Key_Value = 2;	break;
		
		case 0x0080:	Key_Value = 15;	break;
		case 0x0040:	Key_Value = 11;	break;
		case 0x0020:	Key_Value = 7;	break;
		case 0x0010:	Key_Value = 3;	break;
		
		case 0x0008:	Key_Value = 16;	break;
		case 0x0004:	Key_Value = 12;	break;
		case 0x0002:	Key_Value = 8;	break;
		case 0x0001:	Key_Value = 4;	break;
		
		default:			Key_Value = 0;  break;
	}
	
	return Key_Value;
}

























