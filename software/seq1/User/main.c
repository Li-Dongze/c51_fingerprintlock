#include <STC89C5xRC.H>
#include "config.h"
#include "bmp.h" 

/*****************************************����������***********************************************/

//������Ȼ���
void Task_Init(void);
void TaskRun(void);

//����������
void Led_Task(void);
void key_Task(void);
void Oled_Task(void);

//��������
void oledshow_MainPage(void);
void oledshow_MasterPage(void);
void door_motor(void);
void password_clear(void);
bit str_judge(int *str1, int *str2);
void str_copy(int *str1, int *str2);

/*****************************************����ȫ�ֱ���***********************************************/

//ledר�ñ���
uint8_t ucled;

//����ר�ñ���
uint8_t Key_Value;
uint8_t Key_Down;
uint8_t Key_Old;

//�������ר�ñ���
uint16_t data TaskTimer[TASKNUM_MAX];
TaskStruct data Task[] = {
    {Led_Task, 1},
	{key_Task, 10},
	{Oled_Task, 1000}
};
/*****************************************�û�ȫ�ֱ���***********************************************/
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

// ���
uint8_t count0 = 0;
int locktime = 0;
uint8_t jiaodu = 1;

//uint8_t menuKey_flag;

/****************************************************************************************************/


/*------------------------------------------������--------------------------------------------------*/
int main(void)
{
	Task_Init();
    
    //delay_ms(1000);
    
	OLED_Init();//��ʼ��OLED
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
    
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
    
    // �����������
    password_clear();    
    sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
    OLED_ShowString(0,4,str,8);
    
    // ��״̬
    sprintf(str, "door close");
    OLED_ShowString(0,6,str,8);
    
}

void oledshow_MasterPage(void)
{
    sprintf(str, "change: user");
    OLED_ShowString(0,0,str,8);
    
    sprintf(str, "Change Password:");
    OLED_ShowString(0,2,str,8);
    
    // �����������
    password_clear();    
    sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
    OLED_ShowString(0,4,str,8);
    
}

void door_motor(void)
{
    count0++;											//�жϴ������������ٸ�0.5ms
	if(door_flag)									//��״̬-��ʱ
	{
		locktime++;										//������ʱ��ʼ��ʱ
        jiaodu = 2;
		if(locktime == 1000)							//����1��ʱ
		{
			jiaodu = 1;									//����ص��м�λ��
			door_flag = 0;								//��״̬-�ر�
			locktime = 0;								//�����ʱ
		}
	}
		
	if(count0 <= jiaodu) P07 = 1;		//�ߵ�ƽռ�ձ��ɱ���jiaodu���ƣ�jiaodu=1ʱ�ߵ�ƽ0.5ms,jiaodu=5ʱ�ߵ�ƽ2.5ms
	if(count0 > jiaodu && count0 <= 20) P07 = 0;//�͵�ƽռ�ձȣ�jiaodu=1ʱ�͵�ƽ19.5ms��jiaodu=5ʱ�͵�ƽ17.5ms
	if(count0 > 20) count0 = 0;			//����20ms
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

/*-----------------------------------------�жϷ�����--------------------------------------------------*/
void Timer0_Rountine(void)	interrupt 1
{
	uint8_t i;
    
    // �ֶ���װ
    TL0 = 0x66;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
    
	//����ʱ���ݼ�
	for(i = 0; i < TASKNUM_MAX; i++)
		if(TaskTimer[i])
			TaskTimer[i]--;
    
    door_motor();        
}

void Uart1_Isr(void) interrupt 4
{
	if (RI)				//��⴮��1�����ж�
	{
        
		RI = 0;			//�������1�����ж�����λ
	}
    
//    if (TI)				//��⴮��1�����ж�
//    {
//        
//        TI = 0;
//    }
}

/*-----------------------------------------��������--------------------------------------------------*/
void Led_Task(void)
{
    
    
}	

void key_Task(void)
{
	// ���д��������˲�
	Key_Value = Key_Read();
	Key_Down = Key_Value&(Key_Value^Key_Old);
	Key_Old = Key_Value;
	
    // �л��û�/����Աģʽ
    if(Key_Down == 13)
    {
        Mode ^= 1;
    }
    
    // ѡ���޸��û�/����Ա����
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
    
    // ������ϣ��ж������Ƿ���ȷ
    if(index > 3)
    {
        // ������
        if(page_flag == 0)
        {
            // �û�ģʽ��
            if(Mode == 0)
            {
                door_flag = str_judge(password, user_password);
                // �����������
                password_clear();
                sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
                OLED_ShowString(0,4,str,8);
                
            }
            // ����Աģʽ��
            else
            {
                master_enter_flag = str_judge(password, master_password);
                // �����������
                password_clear();
                sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
                OLED_ShowString(0,4,str,8);
                
            }
        }
        // ����Ա����
        else
        {
            // �����û�����
            if(change_flag == 0)
            {
                str_copy(user_password, password);
                
                EEPROM_Write((uint8_t *)user_password, 0, 4);
                
                // �����������
                password_clear();
                sprintf(str, "     %c  %c  %c  %c", password[0], password[1], password[2], password[3]);
                OLED_ShowString(0,4,str,8);
            }
            // ���Ĺ���Ա����
            else
            {
                str_copy(master_password, password);
                
                EEPROM_Write((uint8_t *)master_password, 8, 4);
                
                // �����������
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
    // ���Դ���
    sprintf(str, "%c%c%c%c-%c%c%c%c", user_password[0], user_password[1], user_password[2], user_password[3],
                                      master_password[0], master_password[1], master_password[2], master_password[3]);
    OLED_ShowString(70,7,str,8);
   
    // ������
    if(page_flag == 0)
    {   
        // �Ƿ���
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
        
        // �û�ģʽ
        if(Mode == 0)
        {
            sprintf(str, "user-ID  ");
            OLED_ShowString(0,0,str,8);
        }
        // ����Աģʽ
        else
        {
            sprintf(str, "master-ID");
            OLED_ShowString(0,0,str,8);
            
            // �ɹ��������Ա����
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
    // ����Ա����
    else
    {
        // �����û�����
        if(change_flag == 0)
        {
            sprintf(str, "change: user  ");
            OLED_ShowString(0,0,str,8);
        }
        // ���Ĺ���Ա����
        else
        {
            sprintf(str, "change: master");
            OLED_ShowString(0,0,str,8);
        }
        
        // �ɹ�����������
        if(main_enter_flag)
        {
            OLED_Clear();
            oledshow_MainPage();
            
            // �˻��������û�ģʽ
            door_flag = 0;
            Mode = 0;
            master_enter_flag = 0;
            change_flag = 0;
            page_flag = 0;
            main_enter_flag = 0;
        }
    }
    
    
}

/*-----------------------------------------�������--------------------------------------------------*/
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















