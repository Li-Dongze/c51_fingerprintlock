#include <STC89C5xRC.H>
#include "config.h"
#include "bmp.h" 

/*****************************************����������***********************************************/

//������Ȼ���
void Task_Init(void);
void TaskRun(void);

//����������
void Led_Task(void);
void key_Task(void);
void Oled_Task(void);

/*****************************************����ȫ�ֱ���***********************************************/

//ledר�ñ���
uint8_t ucled;

//����ר�ñ���
uint8_t Key_Value;
uint8_t Key_Down;
uint8_t Key_Old;

//�������ר�ñ���
uint16_t TaskTimer[TASKNUM_MAX];
TaskStruct Task[] = {
    {Led_Task, 100},
	{key_Task, 20},
	{Oled_Task, 2000}
};
/*****************************************�û�ȫ�ֱ���***********************************************/
char str[10];
uint8_t eepromBuf[10];
int count = 0;

/****************************************************************************************************/


/*------------------------------------------������--------------------------------------------------*/
int main(void)
{
	Task_Init();
    
    delay_ms(1000);
    
	OLED_Init();//��ʼ��OLED
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
    
    EEPROM_Write("hello", 0, 5);
    
	Timer1Init();
	EA = 1;
	
	while(1)
	{
		TaskRun();
//        OLED_DrawBMP(0,0,128,64,BMP1);
//		delay_ms(500);
//		OLED_Clear();
//		OLED_ShowChinese(0,0,0,16);//��
//		OLED_ShowChinese(18,0,1,16);//��
//		OLED_ShowChinese(36,0,2,16);//԰
//		OLED_ShowChinese(54,0,3,16);//��
//		OLED_ShowChinese(72,0,4,16);//��
//		OLED_ShowChinese(90,0,5,16);//��
//		OLED_ShowChinese(108,0,6,16);//��
//		OLED_ShowString(8,2,"ZHONGJINGYUAN",16);
//		OLED_ShowString(20,4,"2014/05/01",16);
//		OLED_ShowString(0,6,"ASCII:",16);  
//		OLED_ShowString(63,6,"CODE:",16);
//		delay_ms(500);
//		OLED_Clear();
	}
}

/*-----------------------------------------�жϷ�����--------------------------------------------------*/
void Timer1_Rountine(void)	interrupt 3
{
	uint8_t i;
    
    // �ֶ���װ
    TL1 = 0x66;		//���ö�ʱ��ֵ
	TH1 = 0xFC;		//���ö�ʱ��ֵ
    
	//����ʱ���ݼ�
	for(i = 0; i < TASKNUM_MAX; i++)
		if(TaskTimer[i])
			TaskTimer[i]--;
}

/*-----------------------------------------��������--------------------------------------------------*/
void Led_Task(void)
{
    //P0 ^= 0x01;
}	

void key_Task(void)
{
	//���д���
	Key_Value = Key_Read();
	Key_Down = Key_Value&(Key_Value^Key_Old);
	Key_Old = Key_Value;
	
	switch(Key_Down)
	{
		case 1:
            P04 = ~P04;
        break;
        
        case 2:
            P05 = ~P05;
        break;
        
        case 3:
            P06 = ~P06;
        break;
        
        case 4:
            P07 = ~P07;			
		break;
        
        default:
        break;
	}
}

void Oled_Task(void)
{
    
    sprintf(str, "count = %d", count++);
    OLED_ShowString(0,0,str,16);
    
    EEPROM_Read(eepromBuf, 0, 5);
    OLED_ShowString(0,2,(char *)eepromBuf,16);
}

/*-----------------------------------------�������--------------------------------------------------*/
void Task_Init(void)
{
	uint8_t NTask;
	for(NTask = 0; NTask < sizeof(Task)/sizeof(Task[0]); NTask++)
	{
		TaskTimer[NTask] = Task[NTask].TaskPeriod;
	}
}

void TaskRun(void)
{
	uint8_t NTask;
	for(NTask = 0; NTask < sizeof(Task)/sizeof(Task[0]); NTask++)
	{
		if(TaskTimer[NTask] == 0)
		{
			TaskTimer[NTask] = Task[NTask].TaskPeriod;
			(Task[NTask].pTask)();
		}
	}
}















