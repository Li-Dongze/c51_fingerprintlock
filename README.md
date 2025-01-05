# 单片机课设：基于stc89c52rc的密码锁设计

## 一、项目简介

本项目是一款基于基于stc89c52rc单片机的密码锁设计，使用oled屏显示界面，4*4矩阵键盘实现密码输入，舵机模拟开门，eeprom确保密码掉电不丢失。

功能描述：

1. 按键功能：输入密码，并能根据密码来决定开门还是进入管理员模式
2. 密码功能：能够更换开门密码/管理员密码，且具有掉电不丢失功能
3. 指纹识别功能：识别用户指纹
4. 门锁功能：使用舵机模拟开门动作

![image-20250105184334524](https://picture-note-1328988318.cos.ap-nanjing.myqcloud.com/Typora/202501051843776.png)

## 二、软件具体实现

### 1. 总体架构

使用一种类RTOS时间片调度的架构，但没有RTOS的任务间硬切换。其实也是传统前后台架构的一种，实现任务的时间片轮询，不加delay占用cpu资源

``` c
#define TASKNUM_MAX	4

typedef struct{
	void (*pTask)(void);    //任务函数
	uint16_t TaskPeriod;    //多少毫秒调用一次任务函数
}TaskStruct;

/***************自定义一个定时器中断，我以定时器0为例，每次进入给每个任务减一个时钟节拍，中断进入时间可自定义，本例1ms******************/
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

/******************************任务调度机制*********************************/
void Task_Init(void)
{
	uint8_t NTask;
	for(NTask = 0; NTask < sizeof(Task)/sizeof(Task[0]); NTask++){
		TaskTimer[NTask] = Task[NTask].TaskPeriod;
	}
}	

void Task_Run(void)
{
	uint8_t NTask;
	for(NTask = 0; NTask < sizeof(Task)/sizeof(Task[0]); NTask++){
		if(TaskTimer[NTask] == 0)
		{
			TaskTimer[NTask] = Task[NTask].TaskPeriod;
			(Task[NTask].pTask)();
		}
	}
}

/******************************具体任务函数声明*********************************/
void Key_Task(void);			//按键任务
void Mouse_XY_Updata(void);		//HID协议发送电脑任务
void Mouse_wheel_Updata(void);	//鼠标滚轮更新任务
void LED_Task(void);			//LED任务

/******************************全局变量声明*********************************/
uint16_t TaskTimer[TASKNUM_MAX];
TaskStruct Task[] = {	// 添加任务函数
	{Led_Task, 100},
	{key_Task, 10},
	{Oled_Task, 1000}
};

/******************************主函数中调用*********************************/
int main()
{
    Task_Init();
    
    while(1)
    {
        Task_Run();
    }
}
```

网上有很多类似实现方法，也有原理的讲解，如：

[时间片轮询的任务调度方法（一）](https://blog.csdn.net/mirco_mcu/article/details/114157274)

### 2. 矩阵按键

为保证系统的无阻塞运行，使用数字滤波进行按键采集，每10ms读取一次按键电平，前后两次间进行比较，获取当前10ms按键的电平及上一次10ms的电平，判断此时是按下瞬间/弹起瞬间/一直按下/一直弹起。

由此，定义一个按键任务，10ms触发一次

![image-20250105190142084](https://picture-note-1328988318.cos.ap-nanjing.myqcloud.com/Typora/202501051901135.png)

任务内容：首先进行数字滤波，接下来判断当前按键状态执行相应动作

```c
void key_Task(void)
{
	// 三行代码数字滤波
	Key_Value = Key_Read();
	Key_Down = Key_Value&(Key_Value^Key_Old);
	Key_Old = Key_Value;
	
    // 根据按键状态执行相应动作
    
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
    
    //以下省略
    
}    
```

执行数字滤波时需要用一个函数判断当前所有按键中的哪个键被按下，返回其键值。矩阵按键判断函数如下：

```c
uint8_t Key_Read(void)
{
	uint8_t Key_Value;
	uint16_t Key_New;
	
	P13 = 0; P12 = 1; P11 = 1; P10 = 1;
	Key_New = P0&0x0f;
	P13 = 1; P12 = 0; P11 = 1; P10 = 1;
	Key_New = (Key_New<<4)|(P0&0x0f);
	P13 = 1; P12 = 1; P11 = 0; P10 = 1;
	Key_New = (Key_New<<4)|(P0&0x0f);
	P13 = 1; P12 = 1; P11 = 1; P10 = 0;
	Key_New = (Key_New<<4)|(P0&0x0f);
	
	switch(~Key_New)
	{
		case 0x8000:	Key_Value = 13;	break;
		case 0x4000:	Key_Value = 9;	break;
		case 0x2000:	Key_Value = 5;	break;
		case 0x1000:	Key_Value = 1;	break;
		
		case 0x0800:	Key_Value = 14;	break;
		case 0x0400:	Key_Value = 10;	break;
		case 0x0200:	Key_Value = 6;	break;
		case 0x0100:	Key_Value = 2;	break;
		
		case 0x0080:	Key_Value = 15;	break;
		case 0x0040:	Key_Value = 11;	break;
		case 0x0020:	Key_Value = 7;	break;
		case 0x0010:	Key_Value = 3;	break;
		
		case 0x0008:	Key_Value = 16;	break;
		case 0x0004:	Key_Value = 12;	break;
		case 0x0002:	Key_Value = 8;	break;
		case 0x0001:	Key_Value = 4;	break;
		
		default:			Key_Value = 0;  break;
	}
	
	return Key_Value;
}
```

### 3. oled界面

界面大致样式：

![image-20250105191739747](https://picture-note-1328988318.cos.ap-nanjing.myqcloud.com/Typora/202501051917850.png)

初始时为主界面，user-ID，门关闭

与按键交互：

矩阵键盘上输入s1-s10，直接填入密码，主界面用户密码正确，开门；主界面管理员密码正确，跳转到管理员界面。密码错误全部变为初始态，等待重新输入密码

在主界面按下s13改变ID为管理员或用户ID，切换输入密码

在主界面按下s14改变要修改的密码为管理员或用户

### 4. 密码修改掉电不丢失

使用i2c通信eeprom，存储密码，每次上电时读取，修改时存入

上电时读取eeprom中密码并存入内存

![image-20250105192902673](https://picture-note-1328988318.cos.ap-nanjing.myqcloud.com/Typora/202501051929749.png)

修改时写入eeprom保存

![image-20250105193114343](https://picture-note-1328988318.cos.ap-nanjing.myqcloud.com/Typora/202501051931428.png)

## 三、硬件电路实现

未画pcb，杜邦线接线

| oled引脚 | 开发板端口 |
| -------- | ---------- |
| SCL      | P21        |
| SDA      | P20        |

| 矩阵按键引脚 | 开发板端口 |
| ------------ | ---------- |
| 端口1-4      | P10 - P13  |
| 端口5-8      | P00 - P03  |

| 舵机引脚 | 开发板端口 |
| -------- | ---------- |
| PWM      | P07        |

## 四、遇到的问题及解决方案

### 1. c51内存不够

stc89c52rc的ram有512字节，rom有8192字节，代码多了容易内存不够

![image-20250105194248767](https://picture-note-1328988318.cos.ap-nanjing.myqcloud.com/Typora/202501051942831.png)

#### 解决方案

将一些用不到的库删除，如oled的16x16字库和汉字字库，删除后code少了2k

### 2. 数组越界

![image-20250105194557047](https://picture-note-1328988318.cos.ap-nanjing.myqcloud.com/Typora/202501051945118.png)

由于我的oled界面使用了sprintf函数绘制，用str字符数组存储。最多的字符数有15个，所以一开始就定义了15给字节给str数组。但没考虑字符串会用"\0"结尾，所以会多一个字符。而str数组和管理员密码的内存被编译器分配的相邻，导致多出的“\0”溢出到管理员密码上，使密码被非法修改

#### 解决方案

把str数组改大点即可

























