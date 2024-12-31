#include <STC89C5xRC.H>
#include "config.h"
#include "bmp.h" 

/*****************************************任务函数声明***********************************************/

//任务调度机制
void Task_Init(void);
void TaskRun(void);

//具体任务函数
void Led_Task(void);
void key_Task(void);
void Oled_Task(void);

/*****************************************外设全局变量***********************************************/

//led专用变量
uint8_t ucled;

//按键专用变量
uint8_t Key_Value;
uint8_t Key_Down;
uint8_t Key_Old;

//任务调度专用变量
uint16_t TaskTimer[TASKNUM_MAX];
TaskStruct Task[] = {
    {Led_Task, 100},
	{key_Task, 20},
	{Oled_Task, 2000}
};
/*****************************************用户全局变量***********************************************/
char str[10];
uint8_t eepromBuf[10];
int count = 0;

/****************************************************************************************************/


/*------------------------------------------主函数--------------------------------------------------*/
int main(void)
{
	Task_Init();
    
    delay_ms(1000);
    
	OLED_Init();//初始化OLED
	OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
    
    EEPROM_Write("hello", 0, 5);
    
	Timer1Init();
	EA = 1;
	
	while(1)
	{
		TaskRun();
//        OLED_DrawBMP(0,0,128,64,BMP1);
//		delay_ms(500);
//		OLED_Clear();
//		OLED_ShowChinese(0,0,0,16);//中
//		OLED_ShowChinese(18,0,1,16);//景
//		OLED_ShowChinese(36,0,2,16);//园
//		OLED_ShowChinese(54,0,3,16);//电
//		OLED_ShowChinese(72,0,4,16);//子
//		OLED_ShowChinese(90,0,5,16);//科
//		OLED_ShowChinese(108,0,6,16);//技
//		OLED_ShowString(8,2,"ZHONGJINGYUAN",16);
//		OLED_ShowString(20,4,"2014/05/01",16);
//		OLED_ShowString(0,6,"ASCII:",16);  
//		OLED_ShowString(63,6,"CODE:",16);
//		delay_ms(500);
//		OLED_Clear();
	}
}

/*-----------------------------------------中断服务函数--------------------------------------------------*/
void Timer1_Rountine(void)	interrupt 3
{
	uint8_t i;
    
    // 手动重装
    TL1 = 0x66;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
    
	//任务定时器递减
	for(i = 0; i < TASKNUM_MAX; i++)
		if(TaskTimer[i])
			TaskTimer[i]--;
}

/*-----------------------------------------具体任务--------------------------------------------------*/
void Led_Task(void)
{
    //P0 ^= 0x01;
}	

void key_Task(void)
{
	//三行代码
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

/*-----------------------------------------任务调度--------------------------------------------------*/
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















