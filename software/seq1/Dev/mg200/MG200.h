#ifndef __MG200_H
#define __MG200_H

#include "config.h"

#define u8 unsigned char
#define u16 unsigned int

sbit MG200_DETECT = P0^0;//����detect����

void Mg200_SendData(u8 cmd,u8 parameter);//�������ݰ�
u8 Mg200_RecData(u8 cmd,u8 *result,u8 *parameter);//�������ݰ�
u8 CaptureAndExtract(u8 num);//��ȡָ��������
u8 Enroll(u8 num);//ע��ָ��
u8 Match1N(void);//�ȶ�ָ��


#endif
