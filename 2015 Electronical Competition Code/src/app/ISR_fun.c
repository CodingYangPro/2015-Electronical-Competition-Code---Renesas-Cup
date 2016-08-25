#include "include.h"
#include "math.h"

/*************************************************************************
*  �������ƣ�Water_LEDs
*  ����˵�����Լ���4��led��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14    �Ѳ���
*************************************************************************/
void Water_LEDs()
{
    unsigned char Led_num = (Time_1ms/500)%3;
    if(Led_num==0)
    {_LED1 = 0;_LED2 = 1;_LED3 = 1;}
    else if(Led_num==1)
    {_LED1 = 1;_LED2 = 0;_LED3 = 1;}
    else if(Led_num==2)
    {_LED1 = 1;_LED2 = 1;_LED3 = 0;}
}




/*************************************************************************
*  �������ƣ�Angle_control
*  ����˵�����Ƕ��ںϣ�������Ƕȣ�����Ƕ�pwm�����T=600us
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14    �Ѳ���
*************************************************************************/
#define dt 0.002
#define tg 2         //**����ʱ��:1~4
void Angle_Calculate()
{
    float mG_delta=0;      //��angle_speedһ������
    
    /*********************** ���� ************************/
    Angle.a_sinH = Acc_ADC_Data.y/300.0;	//Acc_ADC_Dataȡֵ���Լ300��
    if(Angle.a_sinH > 1)         Angle.a_sinH = 1;//�޷�
    else if(Angle.a_sinH < -1)   Angle.a_sinH = -1;//�޷�
    
    Angle.m_angleH = asin(Angle.a_sinH)*57.295779513;//�����ң�����m_angleH�Ƚ�С�����Բ���acos����atan
    if(Angle.m_angleH > 90)      Angle.m_angleH = 90;//�޷�
    else if(Angle.m_angleH<-90)  Angle.m_angleH = -90;//�޷�
    
	//ֱ�����Ƕ��ںϴ���
    mG_delta = (Angle.m_angleH - Angle.G_angleH) / tg;
    Angle.speedH = 0 - Gyro_ADC.x * MPU6050G_s2000dps ;//���ٶȣ���λ��/s
    Angle.G_angleH += (mG_delta + Angle.speedH) * dt;//�ںϺ�ĽǶ�G_angleH
    
    
    /*********************** ���� ************************/
    Angle.a_sinL = 0 - Acc_ADC_Data.x/300.0;//Acc_ADC_Dataȡֵ���Լ300��
    if(Angle.a_sinL > 1)         Angle.a_sinL = 1;//�޷�
    else if(Angle.a_sinL < -1)   Angle.a_sinL = -1;//�޷�
    
    Angle.m_angleL = asin(Angle.a_sinL)*57.295779513;//�����ң�����m_angleH�Ƚ�С�����Բ���acos����atan
    if(Angle.m_angleL > 90)      Angle.m_angleL = 90;//�޷�
    else if(Angle.m_angleL<-90)  Angle.m_angleL = -90;//�޷�
    
	//ֱ�����Ƕ��ںϴ���
    mG_delta = (Angle.m_angleL - Angle.G_angleL) / tg;
    Angle.speedL = 0 - Gyro_ADC.y * MPU6050G_s2000dps;//���ٶȣ���λ��/s
    Angle.G_angleL += (mG_delta + Angle.speedL) * dt;//�ںϺ�ĽǶ�G_angleL
}



/*************************************************************************
*  �������ƣ�Mode_PID
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14    �Ѳ���
*************************************************************************/
void Mode_PID()
{
#define Pi 3.1415926535f
    int mode = Mode.mode;
    
    //û����start״̬�Ͳ������ǿ����¡�start��������Mode.startFg[mode]��1
    if(Mode.startFg[mode]==0)
    {
        if(mode != 0 )//0��λ�ǵ���4������ĵ�λ
            Motor[0].PWM = Motor[1].PWM = Motor[2].PWM = Motor[3].PWM = 0;
        return ;
    }
    
    //������ֵMode.P��I��D����ʱPID������ÿһ�ʶ����⼸������
	// Mode.para�ǿ������ı�ļ�������������ο�PORTB_IRQHandler()
    Mode.P = Mode.para[mode][0];
    Mode.I = Mode.para[mode][1];
    Mode.D = Mode.para[mode][2];
    
    if(mode == 1)       //��һ��ָ��60cm
        Angle.Length = 60;
    else if(mode == 2) //�ڶ���ָ�����룬��ͨ����������
        Angle.Length = Mode.para[mode][3];
    else if(mode == 3) //������ָ��60cm
    {
        Angle.Length = 60;
        Angle.DirAngle = Mode.para[mode][3];//��ͨ����������
    }
    else if(mode == 4) //������ר�����ã�P��D��������Ҫ����i���0������ֵҲ����
    {
        Mode.P = Mode.para[mode][0] = 100;
        Mode.I = Mode.para[mode][1] = 0;
        Mode.D = Mode.para[mode][2] = 200;
        Angle.ErrorI_H = Angle.ErrorI_L = 0;
    }
    else if(mode == 5) //�����ʣ���Բ��Բ��ֱ�� Angle.Length ��ͨ����������
    {
        Angle.Length = Mode.para[mode][3];
    }
    
    
    //PID control������ԭ���뿴�ܽ��ĵ�����Щģʽ�Ĵ��붼�ǳ����Ҹо��������Angle.Goal��һ��
	//PID control������ԭ���뿴�ܽ��ĵ�����Щģʽ�Ĵ��붼�ǳ����Ҹо��������Angle.Goal��һ��
	//PID control������ԭ���뿴�ܽ��ĵ�����Щģʽ�Ĵ��붼�ǳ����Ҹо��������Angle.Goal��һ��
	//PID control������ԭ���뿴�ܽ��ĵ�����Щģʽ�Ĵ��붼�ǳ����Ҹо��������Angle.Goal��һ��
	//PID control������ԭ���뿴�ܽ��ĵ�����Щģʽ�Ĵ��붼�ǳ����Ҹо��������Angle.Goal��һ��
    if(mode==1 || mode==2 || mode==4)//1,2,4��ͬһ�״��룬mode=4ʱ��Angle.NowL��Angle.NowH����0����
    {
        /*********************** ���� ************************/
        //����Angle.Goal��Angle.Now
        if(mode==4)
            Angle.GoalL = 0;
        else
            Angle.GoalL = Angle.ForceL[Angle.Length] * sin((Time_1ms - Mode.TimeStart)*2*Pi/((float)(Angle.Period)));
        Angle.NowL  = Angle.G_angleL;
        //����ErrorI�������޷�
        Angle.ErrorI_L += Mode.I * (Angle.NowL - Angle.GoalL)/10.0;
        if(Angle.ErrorI_L >= Mode.I_max) Angle.ErrorI_L = Mode.I_max;
        //���������Խ��ߵĵ��һ���ơ�һ����
        Motor[0].PWM = Mode.P * (Angle.NowL - Angle.GoalL) + Angle.ErrorI_L + Mode.D * Angle.speedL;
        Motor[2].PWM = 0 - Motor[0].PWM;
        
        /*********************** ���� ************************/
        //����Angle.Goal��Angle.Now
        Angle.GoalH = 0;
        Angle.NowH  = Angle.G_angleH;
        //����ErrorI�������޷�
        Angle.ErrorI_H += Mode.I * (Angle.NowH - Angle.GoalH)/10.0;
        if(Angle.ErrorI_H >= Mode.I_max) Angle.ErrorI_H = Mode.I_max;
        else if(Angle.ErrorI_H <= 0 - Mode.I_max) Angle.ErrorI_H = 0 - Mode.I_max;
        //���������Խ��ߵĵ��һ���ơ�һ����
        Motor[1].PWM = Mode.P * (Angle.NowH - Angle.GoalH) + Angle.ErrorI_H + Mode.D * Angle.speedH;
        Motor[3].PWM = 0 - Motor[1].PWM;
    }
    else if(mode == 3)//б�ߣ�����ԭ���뿴�ܽ��ĵ�����1��2��4���������Angle.Goal��һ��
    {        
        static float cosAngle,sinAngle;
        cosAngle = cos( 0 - (Angle.DirAngle*10.0+Angle.DirErr[Angle.DirAngle]) * 2* Pi / 360.0);
        sinAngle = sin( 0 - (Angle.DirAngle*10.0+Angle.DirErr[Angle.DirAngle]) * 2* Pi / 360.0);
        
        /*********************** ���� ************************/
        //����Angle.Goal��Angle.Now
        Angle.GoalL = cosAngle * Angle.ForceL[Angle.Length] * sin((Time_1ms - Mode.TimeStart)*2*Pi/((float)(Angle.Period)));
        Angle.NowL  = Angle.G_angleL;
        //����ErrorI�������޷�
        Angle.ErrorI_L += Mode.I * (Angle.NowL - Angle.GoalL)/10.0;
        if(Angle.ErrorI_L >= Mode.I_max) Angle.ErrorI_L = Mode.I_max;
        //���������Խ��ߵĵ��һ���ơ�һ����
        Motor[0].PWM = Mode.P * (Angle.NowL - Angle.GoalL) + Angle.ErrorI_L + Mode.D * Angle.speedL;
        Motor[2].PWM = 0 - Motor[0].PWM;
        
        /*********************** ���� ************************/
        //����Angle.Goal��Angle.Now
        Angle.GoalH = sinAngle * Angle.ForceH[Angle.Length] * sin((Time_1ms - Mode.TimeStart)*2*Pi/((float)(Angle.Period)));
        Angle.NowH  = Angle.G_angleH;
        //����ErrorI�������޷�
        Angle.ErrorI_H += Mode.I * (Angle.NowH - Angle.GoalH)/10.0;
        if(Angle.ErrorI_H >= Mode.I_max) Angle.ErrorI_H = Mode.I_max;
        else if(Angle.ErrorI_H <= 0 - Mode.I_max) Angle.ErrorI_H = 0 - Mode.I_max;
        //���������Խ��ߵĵ��һ���ơ�һ����
        Motor[1].PWM = Mode.P * (Angle.NowH - Angle.GoalH) + Angle.ErrorI_H + Mode.D * Angle.speedH;
        Motor[3].PWM = 0 - Motor[1].PWM;
    }
    else if(mode==5)//��Բ��û��mode=6����Ϊ������û�����������ܷ��ȸ��š�����ԭ���뿴�ܽ��ĵ�����1��2��4���������Angle.Goal��һ��
    {        
        /*********************** ���� ************************/
        //����Angle.Goal��Angle.Now
        Angle.GoalL = Angle.ForceL[Angle.Length] * cos((Time_1ms - Mode.TimeStart)*2*Pi/((float)(Angle.Period)));
        Angle.NowL  = Angle.G_angleL;
        //����ErrorI�������޷�
        Angle.ErrorI_L += Mode.I * (Angle.NowL - Angle.GoalL)/10.0;
        if(Angle.ErrorI_L >= Mode.I_max) Angle.ErrorI_L = Mode.I_max;
        //���������Խ��ߵĵ��һ���ơ�һ����
        Motor[0].PWM = Mode.P * (Angle.NowL - Angle.GoalL) + Angle.ErrorI_L + Mode.D * Angle.speedL;
        Motor[2].PWM = 0 - Motor[0].PWM;
        
        /*********************** ���� ************************/
        //����Angle.Goal��Angle.Now
        Angle.GoalH = Angle.ForceH[Angle.Length] * sin((Time_1ms - Mode.TimeStart)*2*Pi/((float)(Angle.Period)));
        Angle.NowH  = Angle.G_angleH;
        //����ErrorI�������޷�
        Angle.ErrorI_H += Mode.I * (Angle.NowH - Angle.GoalH)/10.0;
        if(Angle.ErrorI_H >= Mode.I_max) Angle.ErrorI_H = Mode.I_max;
        else if(Angle.ErrorI_H <= 0 - Mode.I_max) Angle.ErrorI_H = 0 - Mode.I_max;
        //���������Խ��ߵĵ��һ���ơ�һ����
        Motor[1].PWM = Mode.P * (Angle.NowH - Angle.GoalH) + Angle.ErrorI_H + Mode.D * Angle.speedH;
        Motor[3].PWM = 0 - Motor[1].PWM;
    }
    else if(mode==7)//����ͷ��λ����1��2��4���������Angle.Goal��һ��
    {   
        /*********************** ���� ************************/
        //����Angle.Goal��Angle.Now 
        //static int tmp=65;
        Angle.GoalL = 360 * atan((Angle.aveL-130)/2.0/72.5)/2.0/Pi;//H = 72.5cm��λ��ת��Ϊ�ǶȵĹ�ʽ
        Angle.NowL  = Angle.G_angleL;
        //����ErrorI�������޷�
        Angle.ErrorI_L += Mode.I * (Angle.NowL - Angle.GoalL)/10.0;
        if(Angle.ErrorI_L >= Mode.I_max) Angle.ErrorI_L = Mode.I_max;
        //���������Խ��ߵĵ��һ���ơ�һ����
        Motor[0].PWM = Mode.P * (Angle.NowL - Angle.GoalL) + Angle.ErrorI_L + Mode.D * Angle.speedL;
        Motor[2].PWM = 0 - Motor[0].PWM;
        
        /*********************** ���� ************************/
        //����Angle.Goal��Angle.Now
        Angle.GoalH = 360 * atan(0 - (Angle.aveH-71)/2.0/72.5)/2.0/Pi;//H = 72.5cm��λ��ת��Ϊ�ǶȵĹ�ʽ
        Angle.NowH  = Angle.G_angleH;
        //����ErrorI�������޷�
        Angle.ErrorI_H += Mode.I * (Angle.NowH - Angle.GoalH)/10.0;
        if(Angle.ErrorI_H >= Mode.I_max) Angle.ErrorI_H = Mode.I_max;
        else if(Angle.ErrorI_H <= 0 - Mode.I_max) Angle.ErrorI_H = 0 - Mode.I_max;
        //���������Խ��ߵĵ��һ���ơ�һ����
        Motor[1].PWM = Mode.P * (Angle.NowH - Angle.GoalH) + Angle.ErrorI_H + Mode.D * Angle.speedH;
        Motor[3].PWM = 0 - Motor[1].PWM; 
    }
}



/*************************************************************************
*  �������ƣ�Motor_Control
*  ����˵����pwm���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14    �Ѳ���
*************************************************************************/
void Motor_Out()
{
    static int pwm_Dead[4]={0,0,0,0};//���������0
    static float pwm_Limit[4]={2400.0,2400.0,2400.0,2400.0};//4���������޷�

    //ƫ�ǹ��󱣻�������������ʱ���豣��
    if(Angle.G_angleL > 35.0f || Angle.G_angleL<-35.0f  || Angle.G_angleH > 35.0f || Angle.G_angleH<-35.0f || (Mode.mode==7 && Angle.startImage == 0))
        Motor[0].PWM = Motor[1].PWM = Motor[2].PWM = Motor[3].PWM = 0;
    
	//������4�������pwm��ֵ���Խ��ߵĵ��һ���ơ�һ����
	//������4�������pwm��ֵ���Խ��ߵĵ��һ���ơ�һ����
	//������4�������pwm��ֵ���Խ��ߵĵ��һ���ơ�һ����
	
    //PWM1
    if(Motor[0].PWM >= 0)
    {
        Motor[0].PWM += pwm_Dead[0]; //������
        if(Motor[0].PWM >= pwm_Limit[0])   //�޷�
            Motor[0].PWM = pwm_Limit[0];
        FTM_PWM_Duty(FTM0, CH2 ,  (unsigned int)(Motor[0].PWM));//���
        DIR1 = 0;
    }
    else
    {
        Motor[0].PWM -= pwm_Dead[2]; //������
        if(Motor[0].PWM <= -pwm_Limit[0])  //�޷�
            Motor[0].PWM = -pwm_Limit[0];
        FTM_PWM_Duty(FTM0, CH2 , (unsigned int)(3990.0 +  Motor[0].PWM)  );//���
        DIR1 = 1;
    }
    
    //PWM2				
    if(Motor[1].PWM >= 0)
    {
        Motor[1].PWM += pwm_Dead[1]; //������
        if(Motor[1].PWM >= pwm_Limit[1])   //�޷�
            Motor[1].PWM = pwm_Limit[1];
        FTM_PWM_Duty(FTM0, CH3 ,  ((unsigned int)(Motor[1].PWM)));//���
        DIR2 = 0;
    }
    else
    {
        Motor[1].PWM -= pwm_Dead[1]; //������
        if(Motor[1].PWM <= -pwm_Limit[1])  //�޷�
            Motor[1].PWM = -pwm_Limit[1];
        FTM_PWM_Duty(FTM0, CH3 , (unsigned int)(4000u + Motor[1].PWM)  );//���
        DIR2 = 1;
    }
    
    //PWM3				
    if(Motor[2].PWM >= 0)
    {
        Motor[2].PWM += pwm_Dead[2]; //������
        if(Motor[2].PWM >= pwm_Limit[2])   //�޷�
            Motor[2].PWM = pwm_Limit[2];
        FTM_PWM_Duty(FTM0, CH4 ,  (unsigned int)(Motor[2].PWM));//���
        DIR3 = 0;
    }
    else
    {
        Motor[2].PWM -= pwm_Dead[2]; //������
        if(Motor[2].PWM <= -pwm_Limit[2])  //�޷�
            Motor[2].PWM = -pwm_Limit[2];
        FTM_PWM_Duty(FTM0, CH4 , (unsigned int)(4000u +  Motor[2].PWM)  );//���
        DIR3 = 1;
    }

    //PWM4				
    if(Motor[3].PWM >= 0)
    {
        Motor[3].PWM += pwm_Dead[3]; //������
        if(Motor[3].PWM >= pwm_Limit[3])   //�޷�
            Motor[3].PWM = pwm_Limit[3];
        FTM_PWM_Duty(FTM2, CH0 ,  (unsigned int)(Motor[3].PWM));//���
        DIR4 = 0;
    }
    else
    {
        Motor[3].PWM -= pwm_Dead[3]; //������
        if(Motor[3].PWM <= -pwm_Limit[3])  //�޷�
            Motor[3].PWM = -pwm_Limit[3];
        FTM_PWM_Duty(FTM2, CH0 , (unsigned int)(4000u +  Motor[3].PWM)  );//���
        DIR4 = 1;
    }
}


