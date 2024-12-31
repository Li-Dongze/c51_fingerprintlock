#ifndef __MG200_H
#define __MG200_H

#include "config.h"

#define u8 unsigned char
#define u16 unsigned int

sbit MG200_DETECT = P0^0;//定义detect引脚

void Mg200_SendData(u8 cmd,u8 parameter);//发送数据包
u8 Mg200_RecData(u8 cmd,u8 *result,u8 *parameter);//接受数据包
u8 CaptureAndExtract(u8 num);//提取指纹特征量
u8 Enroll(u8 num);//注册指纹
u8 Match1N(void);//比对指纹


#endif
