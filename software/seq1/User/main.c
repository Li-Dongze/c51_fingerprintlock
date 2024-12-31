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
uint16_t pdata TaskTimer[TASKNUM_MAX];
TaskStruct pdata Task[] = {
    {Led_Task, 100},
	{key_Task, 10},
	{Oled_Task, 1000}
};
/*****************************************用户全局变量***********************************************/
//char str[10];
//uint8_t eepromBuf[10];
//int count = 0;

uint8_t menuKey_flag;

/****************************************************************************************************/


/*------------------------------------------主函数--------------------------------------------------*/
int main(void)
{
	Task_Init();
    
    //delay_ms(1000);
    
	OLED_Init();//初始化OLED
	OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
    
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

/*-----------------------------------------中断服务函数--------------------------------------------------*/
void Timer0_Rountine(void)	interrupt 1
{
	uint8_t i;
    
    // 手动重装
    TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
    
	//任务定时器递减
	for(i = 0; i < TASKNUM_MAX; i++)
		if(TaskTimer[i])
			TaskTimer[i]--;
}

void Uart1_Isr(void) interrupt 4
{
	if (RI)				//检测串口1接收中断
	{
        
		RI = 0;			//清除串口1接收中断请求位
	}
    
//    if (TI)				//检测串口1发送中断
//    {
//        
//        TI = 0;
//    }
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
//    // oled测试
//    sprintf(str, "count = %d", count++);
//    OLED_ShowString(0,0,str,16);
//    
//    // eeprom测试
//    EEPROM_Read(eepromBuf, 0, 5);
//    OLED_ShowString(0,2,(char *)eepromBuf,16);
       
        menu(menuKey_flag);
        menuKey_flag = 0;
    
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















