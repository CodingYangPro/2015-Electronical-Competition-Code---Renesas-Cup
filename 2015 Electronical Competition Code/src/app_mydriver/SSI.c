#include"include.h"


/***************************��������ʼ��*****************************/


/************************************************
*  �������ƣ�SSI_Init
*  ����˵�������Ա��������ų�ʼ�����������ű���Ϊ5V���̣�����������ź�Ϊ5V��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void SSI_Init()
{
    gpio_init(PORTB, 0, GPI, LOW);//ʹ��DO
    gpio_init(PORTB, 1, GPO, LOW);//ʹ��CS
    gpio_init(PORTB, 2, GPO, LOW);//ʹ��CLK 	  
}


/************************************************
*  �������ƣ�SSIRead
*  ����˵�������ؾ��Ա�������ֵ,T=34us
*  ����˵������
*  �������أ����Ա�������ֵ
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
unsigned short int SSIRead()
{  
    int	data=0;
    CS_1;
    SSInops();SSInops();SSInops();
    CS_0;
    SSInops();SSInops();SSInops();
    for(int i=0; i<10; i++)
    {
        CLK_1;
        SSInops();SSInops();SSInops();
        if(DataOut)
            data+=1;
        data<<=1;
        CLK_0;
        SSInops();SSInops();SSInops();
    }
    data>>=1;
    return(data);
}
