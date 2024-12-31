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
uint16_t pdata TaskTimer[TASKNUM_MAX];
TaskStruct pdata Task[] = {
    {Led_Task, 100},
	{key_Task, 10},
	{Oled_Task, 1000}
};
/*****************************************�û�ȫ�ֱ���***********************************************/
//char str[10];
//uint8_t eepromBuf[10];
//int count = 0;

uint8_t menuKey_flag;

/****************************************************************************************************/


/*------------------------------------------������--------------------------------------------------*/
int main(void)
{
	Task_Init();
    
    //delay_ms(1000);
    
	OLED_Init();//��ʼ��OLED
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
    
    menu_init();
    
    //EEPROM_Write("hello", 0, 5);
    
	Timer0_Init();
    Uart1_Init();
	EA = 1;
    
    //sprintf(str, "hello\n");
    UART_SendString("hello");
	
	while(1)
	{
		TaskRun();
	}
}

/*-----------------------------------------�жϷ�����--------------------------------------------------*/
void Timer0_Rountine(void)	interrupt 1
{
	uint8_t i;
    
    // �ֶ���װ
    TL0 = 0x66;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
    
	//����ʱ���ݼ�
	for(i = 0; i < TASKNUM_MAX; i++)
		if(TaskTimer[i])
			TaskTimer[i]--;
}

void Uart1_Isr(void) interrupt 4
{
	if (RI)				//��⴮��1�����ж�
	{
        
		RI = 0;			//�������1�����ж�����λ
	}
    
//    if (TI)				//��⴮��1�����ж�
//    {
//        
//        TI = 0;
//    }
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
		case 13:
            //P04 = ~P04;
            menuKey_flag = 13;
        break;
        
        case 14:
            //P05 = ~P05;
            menuKey_flag = 14;
        break;
        
        case 15:
            //P06 = ~P06;
            menuKey_flag = 15; 
        break;
        
        case 16:
            //P07 = ~P07;			
            menuKey_flag = 16;
		break;
        
        default:
        break;
	}

    //menu(Key_Down);
}

void Oled_Task(void)
{
//    // oled����
//    sprintf(str, "count = %d", count++);
//    OLED_ShowString(0,0,str,16);
//    
//    // eeprom����
//    EEPROM_Read(eepromBuf, 0, 5);
//    OLED_ShowString(0,2,(char *)eepromBuf,16);
       
        menu(menuKey_flag);
        menuKey_flag = 0;
    
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















