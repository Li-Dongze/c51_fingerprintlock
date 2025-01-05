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

//其他函数
void oledshow_MainPage(void);
void oledshow_MasterPage(void);
void door_motor(void);
void password_clear(void);
bit str_judge(int *str1, int *str2);
void str_copy(int *str1, int *str2);

/*****************************************外设全局变量***********************************************/

//led专用变量
uint8_t ucled;

//按键专用变量
uint8_t Key_Value;
uint8_t Key_Down;
uint8_t Key_Old;

//任务调度专用变量
uint16_t data TaskTimer[TASKNUM_MAX];
TaskStruct data Task[] = {
    {Led_Task, 1},
	{key_Task, 10},
	{Oled_Task, 1000}
};
/*****************************************用户全局变量***********************************************/
char str[20];
//uint8_t eepromBuf[10];
int count = 0;
volatile char password[4] = "____";
volatile char user_password[4] = "1234";
volatile char master_password[4] = "4321";

uint8_t index = 0;
bit door_flag = 0;
bit Mode = 0;
bit master_enter_flag = 0;
bit change_flag = 0;
bit page_flag = 0;
bit main_enter_flag = 0;

// 舵机
uint8_t count0 = 0;
int locktime = 0;
uint8_t jiaodu = 1;

//uint8_t menuKey_flag;

/****************************************************************************************************/


/*------------------------------------------主函数--------------------------------------------------*/
int main(void)
{
	Task_Init();
    
    //delay_ms(1000);
    
	OLED_Init();//初始化OLED
	OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
    
    oledshow_MainPage();
    
    //menu_init();
    
    //EEPROM_Write("hello", 0, 5);
//    EEPROM_Write((uint8_t *)user_password, 0, 4);
//    delay_ms(20);
//    EEPROM_Write((uint8_t *)master_password, 8, 4);
    
    EEPROM_Read((uint8_t *)user_password, 0, 4);
    delay_ms(20);
    EEPROM_Read((uint8_t *)master_password, 8, 4);
    
	Timer0_Init();
    Uart1_Init();
	EA = 1;
    
    //sprintf(str, "hello\n");
    //UART_SendString("hello");
	
	while(1)
	{
		TaskRun();
	}
}

void oledshow_MainPage(void)
{
    sprintf(str, "user-ID");
    OLED_ShowString(0,0,str,8);
    
    sprintf(str, "Enter Password:");
    OLED_ShowString(0,2,str,8);
    
    // 清空输入密码
    password_clear();    
    sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
    OLED_ShowString(0,4,str,8);
    
    // 门状态
    sprintf(str, "door close");
    OLED_ShowString(0,6,str,8);
    
}

void oledshow_MasterPage(void)
{
    sprintf(str, "change: user");
    OLED_ShowString(0,0,str,8);
    
    sprintf(str, "Change Password:");
    OLED_ShowString(0,2,str,8);
    
    // 清空输入密码
    password_clear();    
    sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
    OLED_ShowString(0,4,str,8);
    
}

void door_motor(void)
{
    count0++;											//中断次数，即：多少个0.5ms
	if(door_flag)									//锁状态-开时
	{
		locktime++;										//锁开延时开始计时
        jiaodu = 2;
		if(locktime == 1000)							//到达1秒时
		{
			jiaodu = 1;									//舵机回到中间位置
			door_flag = 0;								//锁状态-关闭
			locktime = 0;								//清零计时
		}
	}
		
	if(count0 <= jiaodu) P07 = 1;		//高电平占空比由变量jiaodu控制，jiaodu=1时高电平0.5ms,jiaodu=5时高电平2.5ms
	if(count0 > jiaodu && count0 <= 20) P07 = 0;//低电平占空比，jiaodu=1时低电平19.5ms，jiaodu=5时低电平17.5ms
	if(count0 > 20) count0 = 0;			//脉宽20ms
}

void password_clear(void)
{
    password[0] = '_';
    password[1] = '_';
    password[2] = '_';
    password[3] = '_';
}

bit str_judge(char *str1, char *str2)
{
    uint8_t i;
    
    for(i = 0; i < 4; i++)
    {
        if(str1[i] != str2[i])
            return 0;
    }
    return 1;
}

void str_copy(char *str1, char *str2)
{
    uint8_t i;
    
    for(i = 0; i < 4; i++)
    {
        str1[i] = str2[i];
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
    
    door_motor();        
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
    
    
}	

void key_Task(void)
{
	// 三行代码数字滤波
	Key_Value = Key_Read();
	Key_Down = Key_Value&(Key_Value^Key_Old);
	Key_Old = Key_Value;
	
    // 切换用户/管理员模式
    if(Key_Down == 13)
    {
        Mode ^= 1;
    }
    
    // 选择修改用户/管理员密码
    if(Key_Down == 14)
    {
        change_flag ^= 1;
    }
        
    
	switch(Key_Down)
	{
		case 10:
            password[index++] = '0';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
        break;
        
        case 1:
            password[index++] = '1';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
        break;
        
        case 2:
            password[index++] = '2';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
        break;
        
        case 3:
            password[index++] = '3';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
		break;
        
        case 4:
            password[index++] = '4';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
		break;
        
        case 5:
            password[index++] = '5';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
		break;
        
        case 6:
            password[index++] = '6';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
		break;
        
        case 7:
            password[index++] = '7';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
		break;
        
        case 8:
            password[index++] = '8';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
		break;
        
        case 9:
            password[index++] = '9';
            sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
            OLED_ShowString(0,4,str,8);
		break;
        
        default:
        break;
	}
    
    // 输入完毕，判断密码是否正确
    if(index > 3)
    {
        // 主界面
        if(page_flag == 0)
        {
            // 用户模式下
            if(Mode == 0)
            {
                door_flag = str_judge(password, user_password);
                // 清空输入密码
                password_clear();
                sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
                OLED_ShowString(0,4,str,8);
                
            }
            // 管理员模式下
            else
            {
                master_enter_flag = str_judge(password, master_password);
                // 清空输入密码
                password_clear();
                sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
                OLED_ShowString(0,4,str,8);
                
            }
        }
        // 管理员界面
        else
        {
            // 更改用户密码
            if(change_flag == 0)
            {
                str_copy(user_password, password);
                
                EEPROM_Write((uint8_t *)user_password, 0, 4);
                
                // 清空输入密码
                password_clear();
                sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
                OLED_ShowString(0,4,str,8);
            }
            // 更改管理员密码
            else
            {
                str_copy(master_password, password);
                
                EEPROM_Write((uint8_t *)master_password, 8, 4);
                
                // 清空输入密码
                password_clear();
                sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
                OLED_ShowString(0,4,str,8);
            }
            main_enter_flag = 1;
        }
        
        
        
        index = 0;
    }
       
}

void Oled_Task(void)
{
    // 调试代码
    sprintf(str, "%c%c%c%c-%c%c%c%c", user_password[0], user_password[1], user_password[2], user_password[3],
                                      master_password[0], master_password[1], master_password[2], master_password[3]);
    OLED_ShowString(70,7,str,8);
   
    // 主界面
    if(page_flag == 0)
    {   
        // 是否开门
        if(door_flag)
        {
            sprintf(str, "door open ");
            OLED_ShowString(0,6,str,8);
            
            P06 = 0;
//            
//            delay_ms(1000);
//            door_flag = 0;
        }
        else
        {
            sprintf(str, "door close");
            OLED_ShowString(0,6,str,8);
            
            P06 = 1;
        }
        
        // 用户模式
        if(Mode == 0)
        {
            sprintf(str, "user-ID  ");
            OLED_ShowString(0,0,str,8);
        }
        // 管理员模式
        else
        {
            sprintf(str, "master-ID");
            OLED_ShowString(0,0,str,8);
            
            // 成功进入管理员界面
            if(master_enter_flag)
            {
                OLED_Clear();
                oledshow_MasterPage();
                
                door_flag = 0;
                Mode = 0;
                master_enter_flag = 0;
                change_flag = 0;
                page_flag = 1;
                main_enter_flag = 0;
            }
        }
    }
    // 管理员界面
    else
    {
        // 更改用户密码
        if(change_flag == 0)
        {
            sprintf(str, "change: user  ");
            OLED_ShowString(0,0,str,8);
        }
        // 更改管理员密码
        else
        {
            sprintf(str, "change: master");
            OLED_ShowString(0,0,str,8);
        }
        
        // 成功返回主界面
        if(main_enter_flag)
        {
            OLED_Clear();
            oledshow_MainPage();
            
            // 退回主界面用户模式
            door_flag = 0;
            Mode = 0;
            master_enter_flag = 0;
            change_flag = 0;
            page_flag = 0;
            main_enter_flag = 0;
        }
    }
    
    
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















