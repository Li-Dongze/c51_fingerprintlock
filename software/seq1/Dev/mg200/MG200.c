#include "MG200.h"

typedef struct {
    unsigned char buff[8];
    unsigned char TC_Flag;
} MG200_REC;

MG200_REC mg200_rec;
/*
�������ܣ�MG200�������ݰ�
�βΣ�
u8 cmd   ���ͷ���ָ����
u8 parameter ���Ͳ���
����ֵ��void
����˵����
*/
void Mg200_SendData(u8 cmd,u8 parameter)
{
	u8 checksum;
	
	UART_SendByte(0x6c);//��ʼ��
	UART_SendByte(0x63);//���͵�ַ --- MCU
	UART_SendByte(0x62);//���յ�ַ --- MG200
	UART_SendByte(cmd);//ָ����
	UART_SendByte(parameter);//����
	UART_SendByte(0x00);//Ԥ��
	/*����У���*/
	checksum = (0x63 + 0x62 + cmd + parameter + 0x00) & 0xff;
	UART_SendByte(checksum);//У���	
}

/*
�������ܣ�MG200�������ݰ�
�βΣ�
u8 cmd    ����ָ����   ע�⣺����ָ���������ָ����һ��
u8 *result  ���շ���ֵ --- ���
u8 *parameter ���ղ���
����ֵ��u8   ����������������0      ���շ������������ط�0
����˵����
*/
u8 Mg200_RecData(u8 cmd,u8 *result,u8 *parameter)
{
	
	u8 checksum;
	
	//�ȴ����ݽ������   TC_Flag��1Ϊ���ݽ������
	while(!mg200_rec.TC_Flag);
	mg200_rec.TC_Flag = 0;//��ձ�־λ
	
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
	
	*result = mg200_rec.buff[4];//���շ���ֵ
	*parameter = mg200_rec.buff[5];//���ղ���
	
	if(mg200_rec.buff[6] != 0x00)
	{
		return 5;
	}
 
	checksum = (0x62 + 0x63 + cmd + *result + *parameter + 0x00) & 0xff;
	if(mg200_rec.buff[7] != checksum)
	{
		return 6;
	}
	return 0;//������������0
}

/*
�������ܣ���ȡָ��������
�βΣ�u8 num   �����ڼ�����ȡָ��������    0 - 4  
����ֵ��u8    ����result���
����˵����
*/
u8 CaptureAndExtract(u8 num)
{
	
	u8 result;
	u8 parameter;
	
	printf("�밴����ָ\r\n");
	//�ȴ���ָ����
	while(!MG200_DETECT);
	
	Mg200_SendData(0x51,num);//�������ݰ�
	if(Mg200_RecData(0x51,&result,&parameter) == 0)
	{
		//������������
		switch(result)
		{
			case 0x00:printf("ץȡָ��ͼ����������ȡ�ɹ�\r\n");break;
			default:printf("ץȡָ��ͼ����������ȡʧ��\r\n");break;
		}			
	}
	else
	{
		printf("ͨ��ʧ��\r\n");
		return 0xff;
	}
	
	return result;
	
}

/*
�������ܣ�ע��ָ��
�βΣ�void
����ֵ��u8    ����result���
0:ע��ɹ�     ��0��ע��ʧ��
����ȡ����ָ��������
����˵����
*/
u8 Enroll(u8 num)
{
	u8 ret;
	u8 result;
	u8 parameter;
	
	printf("��һ����ȡָ��������\r\n");
	do
	{
		ret = CaptureAndExtract(0x00);
	}while(ret);
	printf("�ڶ�����ȡָ��������\r\n");
	do
	{
		ret = CaptureAndExtract(0x01);
	}while(ret);
	printf("��������ȡָ��������\r\n");
	do
	{
		ret = CaptureAndExtract(0x02);
	}while(ret);
	
	//��ȡָ����������ɣ���ʼ����ע��ָ��ָ��
	
	Mg200_SendData(0x7f,num);//�������ݰ�
	if(Mg200_RecData(0x7f,&result,&parameter) == 0)
	{
		//������������
		switch(result)
		{
			case 0x00:printf("ע��ָ�Ƴɹ�����������\r\n");break;
			default:printf("ע��ָ��ʧ��\r\n");break;
		}			
	}
	else
	{
		printf("ͨ��ʧ��\r\n");
		return 0xff;
	}
	return result;
}


/*
�������ܣ��ȶ�ָ��
�βΣ�void
����ֵ������ֵ ��0 ���Աȳɹ�����ΪID��     0���ȶ�ʧ��
u8   ����    ���㣺ID�ţ��ȶԳɹ���      �㣺�ȶ�ʧ�ܣ��Ҳ���ID��
����ȡһ��ָ��������
����˵����
*/
u8 Match1N(void)
{
	u8 result;
	u8 parameter;
	
	if(MG200_DETECT)//��ָ����֮���ٽ���ָ�Ʊȶ�
	{
		while(CaptureAndExtract(0x00));//��ȡһ��ָ��������
		//��ȡָ����������ɣ���ʼ���ͱȶ�ָ��ָ��
		Mg200_SendData(0x71,0x00);//�������ݰ�
		if(Mg200_RecData(0x71,&result,&parameter) == 0)
		{
			//������������
			switch(result)
			{
				case 0x00:printf("�ȶ�ָ�Ƴɹ�,ID��:%d\r\n",parameter);break;
				default:printf("�ȶ�ʧ��\r\n");break;
			}			
		}
		else
		{
			printf("ͨ��ʧ��\r\n");
			return 0;
		}
	}
	return parameter;
	//����ֵ ��0 ���Աȳɹ�����ΪID��     0���ȶ�ʧ��
}