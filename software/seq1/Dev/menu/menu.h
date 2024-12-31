#ifndef __MENU_H
#define __MENU_H	 

#define linenumber 4 //菜单显示的行数

#include "stdio.h"
#include "config.h"
#include "string.h"


struct MenuItem
{
	short MenuCount;//当前菜单项目总数
	char *DisplayString;//当前项目要显示的字符
	void(*Subs)();//选择某一菜单后执行的功能函数
	struct MenuItem *ChildrenMenus;//当前项目的子菜单
	struct MenuItem *ParentMenus;//当前项目的父菜单
};

// 菜单函数
u32 my_pow10(int a);
int my_min(int a,int b);
void menu_init(void);
void display1(void);
void display(struct MenuItem * MenuPoint,int selectItem_current,int selectItem_hidden);
void menu(u8 key_num);

// 功能函数


// 用户需修改显示函数
void menu_show(int x,int line,char *strdata);//显示函数
void menu_clear(void);	//清屏函数

/*========================================返回变量==========================================*/

extern char pdata str[20];
extern int selectItem_current,selectItem_hidden,selectItem;
extern struct MenuItem idata MainMenu[];
extern struct MenuItem idata Setmenu0[];


extern struct MenuItem* MenuPoint;//当前菜单
extern int key;
extern char Setmenu0_str[3][14];

#endif

