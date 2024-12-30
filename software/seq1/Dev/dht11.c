#include "dht11.h"

//
typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  �з���8λ���ͱ���  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  �з���16λ���ͱ��� */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  �޷���32λ���ͱ��� */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  �з���32λ���ͱ��� */
typedef float          F32;      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         F64;      /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */
//
#define uchar unsigned char
#define uint unsigned int
#define   Data_0_time    4

//----------------------------------------------//
//----------------IO�ڶ�����--------------------//
//----------------------------------------------//
sbit  P2_0  = P2^0 ;

U8  U8FLAG, U8temp;
U8  U8comdata;
extern uint8_t  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;




void Delay(U16 j)
{      
    U8 i;
    for(;j>0;j--)
    { 	
        for(i=0;i<27;i++);

    }
}

void  Delay_10us(void)
{
    U8 i;
    i--;
    i--;
    i--;
    i--;
    i--;
    i--;
}
	
void  COM(void)
{
    U8 i;
  
    for(i=0;i<8;i++)	   
    {
        U8FLAG=2;	
        while((!P2_0)&&U8FLAG++);
        Delay_10us();
        Delay_10us();
        Delay_10us();
        U8temp=0;
        if(P2_0)U8temp=1;
        U8FLAG=2;
        while((P2_0)&&U8FLAG++);
        //��ʱ������forѭ��		  
        if(U8FLAG==1)break;
        //�ж�����λ��0����1	 

        // ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 

        U8comdata<<=1;
        U8comdata|=U8temp;        //0
    }//rof
}

//--------------------------------
//-----ʪ�ȶ�ȡ�ӳ��� ------------
//--------------------------------
//----���±�����Ϊȫ�ֱ���--------
//----�¶ȸ�8λ== U8T_data_H------
//----�¶ȵ�8λ== U8T_data_L------
//----ʪ�ȸ�8λ== U8RH_data_H-----
//----ʪ�ȵ�8λ== U8RH_data_L-----
//----У�� 8λ == U8checkdata-----
//----��������ӳ�������----------
//---- Delay();, Delay_10us();,COM(); 
//--------------------------------

void RH(void)
{
    EA = 0;
  //��������18ms 
   P2_0=0;
   Delay(180);
   P2_0=1;
 //������������������ ������ʱ20us
   Delay_10us();
   Delay_10us();
   Delay_10us();
   Delay_10us();
 //������Ϊ���� �жϴӻ���Ӧ�ź� 
   P2_0=1;
 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
   if(!P2_0)		 //T !	  
   {
       U8FLAG=2;
     //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
       while((!P2_0)&&U8FLAG++);
       U8FLAG=2;
     //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
       while((P2_0)&&U8FLAG++);
     //���ݽ���״̬		 
       COM();
       U8RH_data_H_temp=U8comdata;
       COM();
       U8RH_data_L_temp=U8comdata;
       COM();
       U8T_data_H_temp=U8comdata;
       COM();
       U8T_data_L_temp=U8comdata;
       COM();
       U8checkdata_temp=U8comdata;
       P2_0=1;
     //����У�� 
     
       U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
       if(U8temp==U8checkdata_temp)
       {
          U8RH_data_H=U8RH_data_H_temp;
          U8RH_data_L=U8RH_data_L_temp;
          U8T_data_H=U8T_data_H_temp;
          U8T_data_L=U8T_data_L_temp;
          U8checkdata=U8checkdata_temp;
       }//fi
   }//fi
    EA = 1;
}



