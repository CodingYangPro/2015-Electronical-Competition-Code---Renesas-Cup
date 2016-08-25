#ifndef __INCLUDE_H__
#define __INCLUDE_H__


#include  "common.h"
#include  "define.h"

/*************************************************************************
*  ģ�����ƣ�û������
*  ����˵�����û��Զ����ͷ�ļ�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*************************************************************************/
#include  "gpio.h"      //IO�ڲ���
#include  "uart.h"      //����
#include  "adc.h"       //ADCģ��
#include  "FTM.h"       //FTMģ�飨FTM0��������� / ͨ�� /PWM     FTM1��2���������� / ͨ�� /PWM ��
#include  "PIT.h"       //�����жϼ�ʱ��
#include  "lptmr.h"     //�͹��Ķ�ʱ��(��ʱ)
#include  "exti.h"      //EXTI�ⲿGPIO�ж�
#include  "arm_math.h"  //DSP��
#include  "delay.h"
#include  "OV7725.h"
#include  "dma.h"
#include  "ff.h"
#include  "flash.h"
#include  "stdio.h"

#include  "Car_init.h"
#include  "LCD.h"
#include  "IIC.h"
#include  "ISR_fun.h"
#include  "math.h"
#include  "ccd.h"

/*************************************************************************
*  ģ�����ƣ��ṹ��ͱ���ģ��
*  ����˵����Include �û��Զ���Ľṹ��ͱ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*************************************************************************/
struct CarThreeNum//��Ԫ�飬�ö����ݶ�����Ԫ��
{
    float x;
    float y;
    float z;    
};
struct CarAngle//�Ƕȵ�һЩ����
{
    float roll;	//�����㷨�����ݣ����û�õ�������ݣ����ÿ�
    float pitch;//�����㷨�����ݣ����û�õ�������ݣ����ÿ�
    float yaw;	//�����㷨�����ݣ����û�õ�������ݣ����ÿ�
    
    float rotation;//�����㷨�����ݣ����û�õ�������ݣ����ÿ�
    float vertical;//�����㷨�����ݣ����û�õ�������ݣ����ÿ�
    
    float GoalH,GoalL;//H�����У�L�����У�GoalH���з����Ŀ��Ƕ�,��λ��
    float NowH,NowL;	//NowH���з���ĵ�ǰʵ�ʽǶȣ�ֻ����pid�õ��ˣ���ʵ��m_angleH��һ�������������о������NowH=G_angleH,��λ��
    float ErrorI_H,ErrorI_L;//ErrorI_H���з���i�����Ļ���,���޷���
    
    float a_sinH,a_sinL;//a_sinH�ǽǶȵ�sinֵ��ȡֵ[-1,1]֮��
    float m_angleH,m_angleL;//a_sinHȡ�����ҵõ��ĽǶ�ֵ��ע���÷����ң����÷����һ��߷����У��ǶȺ�Сʱ�����ұȽ�׼,��λ��
    float G_angleH,G_angleL;//G_angleH���з���ĵ�ǰʵ�ʽǶȣ���pidʱ��ֵ������NowH����ʵ������һ��������Ҳ�ǱȽϴ�æû��,��λ��
    float speedH,speedL;//speedH��H����Ľ��ٶȣ���λ��/s
    
    int Length;//�ڶ����ȣ����ڻ���������˵���ǳ��ȣ����ڻ�Բ��˵����ֱ��
    float ForceH[100],ForceL[100];//ForceH���з���İڶ����ȶ�Ӧ�������С
	
    int Period;//��Ȼ�ڶ������ڣ�T=1500ms��ֱ��������Ӧ�þͿ��ԡ�����ԭ���뿴�ĵ����ܽᣨ��Ȼд��Ҳ���Ǻ������
	//��Ϊ��ģ����Ȼ�ڶ�����������ʵĻ�Ҫʵ�⣬�ڶ�n�γ���n����
	
    int DirAngle;//�������ֵ����ʣ�Ҫ���õ�ƫ�ǣ�ȡֵ0~35����Ӧ0��~350��
    float DirAngleRotation;//û�õ�
    float DirErr[40];//�ǶȽ����б�ԭ���뿴�ĵ��ܽ�
    
    float aveH,aveL;//���ڷ��ӵ����ʣ�����ͷ��λ��aveH��aveL��Ӧ��Ӭ�ĺ�ɫͷ����λ��
    int startImage;//Motor_Out()�����ˣ��ڷ��ӵ����ʵ�ʱ�򣬲�����ƫ�ǹ��󱣻�����Ϊ��Ӭ�Ŀ������λ�ñȽ�Զ�����԰ѱ����ص�
};
struct CarFourNum//��̬��Ԫ�飬���û�õ�
{
    float q1;
    float q2;
    float q3;
    float q4;    
};
struct CarMotor
{
    int speed;				//û�õ�
    float PWM;        	  //�������PWM
};
struct CarLcd	//��ʾ��ר��
{
    int level;//��ǰ���ڵڼ���
    int Add;//Address����ǰ���ڵڼ���
    int flushFg;//�Ƿ�ˢ����Flag
    int AddMax[2];//Add�����ֵ��ÿ����Ӧһ�����ֵ
};
struct CarMode
{
    int mode;//����ģʽ����0��λ���ڸı�pwm����4����������ڲ��ԣ���1~4��Ӧ�������֣�5~7��Ӧ���Ӳ���
    long int TimeStart;//��¼��ʼ�������µ�ʱ�䣬��ģʽ�µ�startFg��1
    int I_max;//PID����Ļ����޷�
    float P;//PID�����P
    float I;//PID�����I
    float D;//PID�����D
    int para[10][4];//���Ա������ı����ʱ���飬�������ͨ���洢pid���������Ȼ��ֵ������
    int startFg[10];//ÿ��ģʽ���и���־λ��0������ʼ��1����ʼ�����ģʽ
};


/*************************************************************************
*  ģ�����ƣ�û������
*  ����˵����Include �û��Զ����ȫ�ֱ�������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*************************************************************************/
extern struct CarThreeNum  Acc_ADC_Data,Acc_ADC,Acc_Offset;
extern struct CarThreeNum  Gyro_dps,Gyro_ADC,Gyro_Offset;
extern struct CarAngle Angle;
extern struct CarFourNum  Q;
extern struct CarThreeNum Gravity;

extern struct CarMotor Motor[4];
extern struct CarLcd Lcd;
extern struct CarMode Mode;
extern long int    Time_1ms;


/***************** ucos ר�� *****************/
#define USOC_EN     0u      //0Ϊ��ֹuC/OS������0������uC/OS
#if USOC_EN > 0u
#include  "ucos_ii.h"  		//uC/OS-IIϵͳ����ͷ�ļ�
#include  "BSP.h"			//�뿪������صĺ���
#include  "app.h"			//�û�������


#endif  //if  USOC_EN > 0


#endif  //__INCLUDE_H__
