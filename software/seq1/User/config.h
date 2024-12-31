#ifndef	__CONFIG_H__
#define __CONFIG_H__

typedef unsigned char 			uint8_t;
typedef unsigned short int 	    uint16_t;
typedef unsigned int 		    uint32_t;

#define  u8 unsigned char 
#define  u16 unsigned int
#define  u32 unsigned int

/*************************宏定义*******************************/
#define TASKNUM_MAX	4

/*************************结构体*******************************/
typedef struct{
	void (*pTask)(void);
	uint16_t TaskPeriod;
}TaskStruct;

/*************************常用头文件*******************************/
#include <STC89C5xRC.H>
#include <stdio.h>
#include <string.h>
#include <intrins.h>

/*************************外设头文件*******************************/
#include "led.h"
#include "key.h"
#include "i2c.h"
#include "timer1.h"

#include "oled.h"
#include "menu.h"
#include "uart.h"
#include "MG200.h"

#define WKUP_PRES   13
#define KEY1_PRES   14
#define KEY0_PRES   15
#define KEY2_PRES   16



#endif
















