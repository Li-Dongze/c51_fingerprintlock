#ifndef __MENU_H
#define __MENU_H	 

#define linenumber 4 //�˵���ʾ������

#include "stdio.h"
#include "config.h"
#include "string.h"


struct MenuItem
{
	short MenuCount;//��ǰ�˵���Ŀ����
	char *DisplayString;//��ǰ��ĿҪ��ʾ���ַ�
	void(*Subs)();//ѡ��ĳһ�˵���ִ�еĹ��ܺ���
	struct MenuItem *ChildrenMenus;//��ǰ��Ŀ���Ӳ˵�
	struct MenuItem *ParentMenus;//��ǰ��Ŀ�ĸ��˵�
};

// �˵�����
u32 my_pow10(int a);
int my_min(int a,int b);
void menu_init(void);
void display1(void);
void display(struct MenuItem * MenuPoint,int selectItem_current,int selectItem_hidden);
void menu(u8 key_num);

// ���ܺ���


// �û����޸���ʾ����
void menu_show(int x,int line,char *strdata);//��ʾ����
void menu_clear(void);	//��������

/*========================================���ر���==========================================*/

extern char pdata str[20];
extern int selectItem_current,selectItem_hidden,selectItem;
extern struct MenuItem idata MainMenu[];
extern struct MenuItem idata Setmenu0[];


extern struct MenuItem* MenuPoint;//��ǰ�˵�
extern int key;
extern char Setmenu0_str[3][14];

#endif

