#ifndef	__CONFIG_H__
#define __CONFIG_H__

typedef unsigned char 			uint8_t;
typedef unsigned short int 	    uint16_t;
typedef unsigned int 		    uint32_t;

/*************************�궨��*******************************/
#define TASKNUM_MAX	4

/*************************�ṹ��*******************************/
typedef struct{
	void (*pTask)(void);
	uint16_t TaskPeriod;
}TaskStruct;

/*************************����ͷ�ļ�*******************************/
#include <STC89C5xRC.H>
#include <stdio.h>
#include <string.h>
#include <intrins.h>

/*************************����ͷ�ļ�*******************************/
#include "bsp_Init.h"
#include "led.h"
#include "key.h"
#include "seg.h"
#include "ds1302.h"
#include "ds18b20.h"
#include "i2c.h"
#include "timer1.h"

#include "oled.h"
#include "dht11.h"




#endif
















