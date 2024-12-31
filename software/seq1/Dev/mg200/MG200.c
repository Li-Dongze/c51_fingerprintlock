#include "MG200.h"

typedef struct {
    unsigned char buff[8];
    unsigned char TC_Flag;
} MG200_REC;

MG200_REC mg200_rec;
/*
函数功能：MG200发送数据包
形参：
u8 cmd   发送发送指令码
u8 parameter 发送参数
返回值：void
函数说明：
*/
void Mg200_SendData(u8 cmd,u8 parameter)
{
	u8 checksum;
	
	UART_SendByte(0x6c);//起始码
	UART_SendByte(0x63);//发送地址 --- MCU
	UART_SendByte(0x62);//接收地址 --- MG200
	UART_SendByte(cmd);//指令码
	UART_SendByte(parameter);//参数
	UART_SendByte(0x00);//预留
	/*计算校验和*/
	checksum = (0x63 + 0x62 + cmd + parameter + 0x00) & 0xff;
	UART_SendByte(checksum);//校验和	
}

/*
函数功能：MG200接收数据包
形参：
u8 cmd    接收指令码   注意：接收指令码跟发送指令码一样
u8 *result  接收返回值 --- 结果
u8 *parameter 接收参数
返回值：u8   接收正常函数返回0      接收非正常函数返回非0
函数说明：
*/
u8 Mg200_RecData(u8 cmd,u8 *result,u8 *parameter)
{
	
	u8 checksum;
	
	//等待数据接收完成   TC_Flag置1为数据接收完成
	while(!mg200_rec.TC_Flag);
	mg200_rec.TC_Flag = 0;//清空标志位
	
	if(mg200_rec.buff[0] != 0x6c)
	{
		return 1;
	}		
	if(mg200_rec.buff[1] != 0x62)
	{
		return 2;
	}
	if(mg200_rec.buff[2] != 0x63)
	{
		return 3;
	}
	if(mg200_rec.buff[3] != cmd)
	{
		return 4;
	}
	
	*result = mg200_rec.buff[4];//接收返回值
	*parameter = mg200_rec.buff[5];//接收参数
	
	if(mg200_rec.buff[6] != 0x00)
	{
		return 5;
	}
 
	checksum = (0x62 + 0x63 + cmd + *result + *parameter + 0x00) & 0xff;
	if(mg200_rec.buff[7] != checksum)
	{
		return 6;
	}
	return 0;//接收正常返回0
}

/*
函数功能：提取指纹特征量
形参：u8 num   决定第几次提取指纹特征量    0 - 4  
返回值：u8    返回result结果
函数说明：
*/
u8 CaptureAndExtract(u8 num)
{
	
	u8 result;
	u8 parameter;
	
	printf("请按下手指\r\n");
	//等待手指按下
	while(!MG200_DETECT);
	
	Mg200_SendData(0x51,num);//发送数据包
	if(Mg200_RecData(0x51,&result,&parameter) == 0)
	{
		//接收数据正常
		switch(result)
		{
			case 0x00:printf("抓取指纹图像及特征量提取成功\r\n");break;
			default:printf("抓取指纹图像及特征量提取失败\r\n");break;
		}			
	}
	else
	{
		printf("通信失败\r\n");
		return 0xff;
	}
	
	return result;
	
}

/*
函数功能：注册指纹
形参：void
返回值：u8    返回result结果
0:注册成功     非0：注册失败
先提取三次指纹特征量
函数说明：
*/
u8 Enroll(u8 num)
{
	u8 ret;
	u8 result;
	u8 parameter;
	
	printf("第一次提取指纹特征量\r\n");
	do
	{
		ret = CaptureAndExtract(0x00);
	}while(ret);
	printf("第二次提取指纹特征量\r\n");
	do
	{
		ret = CaptureAndExtract(0x01);
	}while(ret);
	printf("第三次提取指纹特征量\r\n");
	do
	{
		ret = CaptureAndExtract(0x02);
	}while(ret);
	
	//提取指纹特征量完成，开始发送注册指纹指令
	
	Mg200_SendData(0x7f,num);//发送数据包
	if(Mg200_RecData(0x7f,&result,&parameter) == 0)
	{
		//接收数据正常
		switch(result)
		{
			case 0x00:printf("注册指纹成功！！！！！\r\n");break;
			default:printf("注册指纹失败\r\n");break;
		}			
	}
	else
	{
		printf("通信失败\r\n");
		return 0xff;
	}
	return result;
}


/*
函数功能：比对指纹
形参：void
返回值：返回值 非0 ：对比成功，且为ID号     0：比对失败
u8   参数    非零：ID号（比对成功）      零：比对失败（找不到ID）
先提取一次指纹特征量
函数说明：
*/
u8 Match1N(void)
{
	u8 result;
	u8 parameter;
	
	if(MG200_DETECT)//手指按下之后再进行指纹比对
	{
		while(CaptureAndExtract(0x00));//提取一次指纹特征量
		//提取指纹特征量完成，开始发送比对指纹指令
		Mg200_SendData(0x71,0x00);//发送数据包
		if(Mg200_RecData(0x71,&result,&parameter) == 0)
		{
			//接收数据正常
			switch(result)
			{
				case 0x00:printf("比对指纹成功,ID号:%d\r\n",parameter);break;
				default:printf("比对失败\r\n");break;
			}			
		}
		else
		{
			printf("通信失败\r\n");
			return 0;
		}
	}
	return parameter;
	//返回值 非0 ：对比成功，且为ID号     0：比对失败
}