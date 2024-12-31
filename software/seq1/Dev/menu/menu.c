#include "menu.h"
/*
�����Ӧ��bվ��Ƶ
https://www.bilibili.com/video/BV1DT411V78v/
*/

/*==================================1.�˵������������ʾ����ʵ��=========================================*/ 

char pdata str[20];
int selectItem_current,selectItem_hidden,selectItem;

struct MenuItem* MenuPoint=MainMenu;//��������ָ��ǰ�˵�

char Setmenu0_str[3][10]=
{
	{"1.TestA"},
	{"2.TestB"},
	{"3.TestC"},
};
//��һ��
#define MainMenunum 3
struct MenuItem idata MainMenu[MainMenunum] = { 
/*	��ǰ��������˵�������Ŀ | ��ǰ�˵����� | ѡ��ò˵���ִ�еĹ��ܺ��� | ��ǰ��Ŀ���Ӳ˵�	| ��ǰ��Ŀ�ĸ��˵�*/
{ 	 		MainMenunum,     				 "Menu1" ,							NULL ,								Setmenu0, 					NULL },	
{ 	 		MainMenunum,		  			 "Menu2" ,							NULL ,								NULL,						NULL },
{ 	 		MainMenunum,		  			 "Menu3" ,							NULL ,								NULL,						NULL },
};

//�ڶ���
#define Setmenu0num 4
struct MenuItem idata Setmenu0[Setmenu0num]={
{ Setmenu0num,Setmenu0_str[0],NULL,NULL,MainMenu },
{ Setmenu0num,Setmenu0_str[1],NULL,NULL,MainMenu },
{ Setmenu0num,Setmenu0_str[2],NULL,NULL,MainMenu },
{ Setmenu0num,"BACK"   ,NULL,NULL,MainMenu },
};

//��ʾ����
//xΪ���������ص���
//lineΪ�������� 0 1 2 3
void menu_show(int x,int line,char *strdata)
{
    OLED_ShowString((u8)x, (u8)2*line, strdata, 16);
}
//��������
void menu_clear(void)
{
    OLED_Clear();
}
//�˵���ر�����ʼ����������ʾ��Ŀ¼����
void menu_init(void)
{
	selectItem_current =1;
	selectItem_hidden  =0;
	selectItem=selectItem_current+selectItem_hidden;
	menu_clear();
	display(MenuPoint,selectItem_current,selectItem_hidden);
}
//�˵���ʾ����
//
void display(struct MenuItem * MenuPoint,int selectItem_current,int selectItem_hidden)
{
	int j;//forѭ���õ�
	int x=24;//�˵���ʾ����ʼX����
	int arrows_x=0;//��ͷ����ʼX����
	int line=0;//��ʾ����ʼ����
	menu_show(arrows_x,selectItem_current-1,"->");
	for ( j= selectItem_hidden; j < my_min(MenuPoint->MenuCount,linenumber+selectItem_hidden);j++)
	{
		menu_show(x,line,MenuPoint[j].DisplayString);
		line+=1;
	}
    
}
//������ĺ���һ���������Ϊȫ�ֱ���,�ʲ�����ʡȥ
void display1(void)
{
	display(MenuPoint,selectItem_current,selectItem_hidden);
}
//10��N�η�����
u32 my_pow10(int a)
{
	u8 i;
	u32 mydata=1;
	for(i=0;i<a;i++)
	mydata*=10;
	return mydata;
}
//�Ƚ�������������С����
int my_min(int a,int b)
{
	if (a<b)return a;
	return b;
}

/*==================================2.�˵���������=========================================*/ 
void menu(u8 key_num)
{
	switch(key_num)
	{
		case WKUP_PRES:
		{//��
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
		{//��
			menu_clear();
			selectItem_current++;
			if (selectItem_current+selectItem_hidden == MenuPoint->MenuCount+1)//���Ͽ�ʵ�������ƶ��ܵ�������һ��
			{
					selectItem_current =1;
					selectItem_hidden  =0;
					selectItem=selectItem_current+selectItem_hidden;
					display(MenuPoint,selectItem_current,selectItem_hidden);
			}
			if(selectItem_current>linenumber)//��ǰ����������������
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
		{//�� ȷ�Ͻ������Ŀ
			//���Զ�����ַ�Ϊ"������һ��"��ִ����Ӧ�Ĺ���
			if((strcmp((char*)MenuPoint[selectItem-1].DisplayString,"BACK") == 0)&&MenuPoint!=MainMenu)//�����˻���һ������
			//if(selectItem== 1&&MenuPoint!=MainMenu)//�����˻���һ������
			{
					menu_clear();
					MenuPoint = MenuPoint[selectItem - 1].ParentMenus;
					selectItem_current =1;
					selectItem_hidden  =0;
					selectItem=selectItem_current+selectItem_hidden;
					display(MenuPoint,selectItem_current,selectItem_hidden);
			}
			else if (MenuPoint[selectItem - 1].ChildrenMenus != NULL)//�ж��Ƿ�����һ��
			{				
				menu_clear();
				MenuPoint = MenuPoint[selectItem - 1].ChildrenMenus;
				selectItem_current =1;
				selectItem_hidden  =0;
				selectItem=selectItem_current+selectItem_hidden;
				display(MenuPoint,selectItem_current,selectItem_hidden);
			}
			else if(MenuPoint[selectItem - 1].Subs != NULL)MenuPoint[selectItem - 1].Subs();//���ܺ���ִ��	
		};break;
		case KEY2_PRES:
		{//�� ������һ��
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

/*==================================3.���ܺ���ʵ��=========================================*/ 











