#include "menu.h"
/*
代码对应的b站视频
https://www.bilibili.com/video/BV1DT411V78v/
*/

/*==================================1.菜单界面绘制与显示函数实现=========================================*/ 

char pdata str[20];
int selectItem_current,selectItem_hidden,selectItem;

struct MenuItem* MenuPoint=MainMenu;//链表引索指向当前菜单

char Setmenu0_str[3][10]=
{
	{"1.TestA"},
	{"2.TestB"},
	{"3.TestC"},
};
//第一级
#define MainMenunum 3
struct MenuItem idata MainMenu[MainMenunum] = { 
/*	当前界面包含菜单的总数目 | 当前菜单名称 | 选择该菜单后执行的功能函数 | 当前项目的子菜单	| 当前项目的父菜单*/
{ 	 		MainMenunum,     				 "Menu1" ,							NULL ,								Setmenu0, 					NULL },	
{ 	 		MainMenunum,		  			 "Menu2" ,							NULL ,								NULL,						NULL },
{ 	 		MainMenunum,		  			 "Menu3" ,							NULL ,								NULL,						NULL },
};

//第二级
#define Setmenu0num 4
struct MenuItem idata Setmenu0[Setmenu0num]={
{ Setmenu0num,Setmenu0_str[0],NULL,NULL,MainMenu },
{ Setmenu0num,Setmenu0_str[1],NULL,NULL,MainMenu },
{ Setmenu0num,Setmenu0_str[2],NULL,NULL,MainMenu },
{ Setmenu0num,"BACK"   ,NULL,NULL,MainMenu },
};

//显示函数
//x为横坐标像素点数
//line为行数，如 0 1 2 3
void menu_show(int x,int line,char *strdata)
{
    OLED_ShowString((u8)x, (u8)2*line, strdata, 16);
}
//清屏函数
void menu_clear(void)
{
    OLED_Clear();
}
//菜单相关变量初始化，并且显示根目录内容
void menu_init(void)
{
	selectItem_current =1;
	selectItem_hidden  =0;
	selectItem=selectItem_current+selectItem_hidden;
	menu_clear();
	display(MenuPoint,selectItem_current,selectItem_hidden);
}
//菜单显示函数
//
void display(struct MenuItem * MenuPoint,int selectItem_current,int selectItem_hidden)
{
	int j;//for循环用的
	int x=24;//菜单显示的起始X坐标
	int arrows_x=0;//箭头的起始X坐标
	int line=0;//显示的起始行数
	menu_show(arrows_x,selectItem_current-1,"->");
	for ( j= selectItem_hidden; j < my_min(MenuPoint->MenuCount,linenumber+selectItem_hidden);j++)
	{
		menu_show(x,line,MenuPoint[j].DisplayString);
		line+=1;
	}
    
}
//和上面的函数一样，因变量为全局变量,故参数可省去
void display1(void)
{
	display(MenuPoint,selectItem_current,selectItem_hidden);
}
//10的N次方函数
u32 my_pow10(int a)
{
	u8 i;
	u32 mydata=1;
	for(i=0;i<a;i++)
	mydata*=10;
	return mydata;
}
//比较两个数，返回小的数
int my_min(int a,int b)
{
	if (a<b)return a;
	return b;
}

/*==================================2.菜单交互功能=========================================*/ 
void menu(u8 key_num)
{
	switch(key_num)
	{
		case WKUP_PRES:
		{//上
			menu_clear();
			selectItem_current--;
			if (selectItem_current==0)
			{
				if(selectItem_hidden>0)selectItem_hidden--;
				selectItem_current++;
			}
			selectItem=selectItem_current+selectItem_hidden;
			display(MenuPoint,selectItem_current,selectItem_hidden);
		};break;
		case KEY1_PRES:
		{//下
			menu_clear();
			selectItem_current++;
			if (selectItem_current+selectItem_hidden == MenuPoint->MenuCount+1)//加上可实现往下移动跑到最上面一行
			{
					selectItem_current =1;
					selectItem_hidden  =0;
					selectItem=selectItem_current+selectItem_hidden;
					display(MenuPoint,selectItem_current,selectItem_hidden);
			}
			if(selectItem_current>linenumber)//当前的行数超过总行数
			{
				if (selectItem_current+selectItem_hidden <= MenuPoint->MenuCount)
					selectItem_hidden++;
				selectItem_current--;
			}
			else if(selectItem_current>MenuPoint->MenuCount)selectItem_current--;
			selectItem=selectItem_current+selectItem_hidden;
			display(MenuPoint,selectItem_current,selectItem_hidden);
		};break;
		case KEY0_PRES:
		{//右 确认进入此项目
			//可自动监测字符为"返回上一级"就执行相应的功能
			if((strcmp((char*)MenuPoint[selectItem-1].DisplayString,"BACK") == 0)&&MenuPoint!=MainMenu)//满足退回上一级条件
			//if(selectItem== 1&&MenuPoint!=MainMenu)//满足退回上一级条件
			{
					menu_clear();
					MenuPoint = MenuPoint[selectItem - 1].ParentMenus;
					selectItem_current =1;
					selectItem_hidden  =0;
					selectItem=selectItem_current+selectItem_hidden;
					display(MenuPoint,selectItem_current,selectItem_hidden);
			}
			else if (MenuPoint[selectItem - 1].ChildrenMenus != NULL)//判断是否有下一级
			{				
				menu_clear();
				MenuPoint = MenuPoint[selectItem - 1].ChildrenMenus;
				selectItem_current =1;
				selectItem_hidden  =0;
				selectItem=selectItem_current+selectItem_hidden;
				display(MenuPoint,selectItem_current,selectItem_hidden);
			}
			else if(MenuPoint[selectItem - 1].Subs != NULL)MenuPoint[selectItem - 1].Subs();//功能函数执行	
		};break;
		case KEY2_PRES:
		{//左 返回上一级
			if(MenuPoint!=MainMenu)
			{
				menu_clear();
				MenuPoint = MenuPoint[selectItem - 1].ParentMenus;
				selectItem_current =1;
				selectItem_hidden  =0;
				selectItem=selectItem_current+selectItem_hidden;
				display(MenuPoint,selectItem_current,selectItem_hidden);
			}		
		}
	}
}

/*==================================3.功能函数实现=========================================*/ 











