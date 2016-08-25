//������
//1.�����ͽṹ�������뿴include.h
//2.�����Ǹ������ܳ����̸��Ĺ�ȥ�ģ��кö�û�õ��ļ���������Ķ�����Ҫ�Ĵ���ֻ���������Ͷ�ʱ���жϺ���
//	��ʾ��ˢ�����������ӣ���Ϊ�Ƚ��ң���ʹ���˿��Ļ�������Ҳ��������=��=�����԰��ص���ڽǶ��ںϣ�
//	PID���ƣ��͹��̴������֯
//3.main.c:�����˱�����main()��ʼ��������ģ�飬Ȼ��һֱˢ��
//4.include.h:�����˸���h�ļ������и��ֱ���������
//5.isr.c:
//	PORTB_IRQHandler():ÿ����������������жϣ����ڸı�ģʽ�͸��ı���
//	VSYNC_IRQ(void)������ͷ���жϣ�ֻ�з��Ӳ��ֵ��������ˣ�Ҳ���ǳ����Ӧ��ģʽ7
//	DMA0_IRQHandler():����ͷ��DMA�洢�����ж�
//	PIT0_IRQHandler(void)������Ҫ�ĽǶ��ںϣ�PID���㣬���е�����
//6.ISR_fun.c:������isr.c���õĺ���
//7.IIC.c:������MPU6050�ĵײ㣬��ʼ�����ɼ����ݡ��˲�������
//8.Car_init.c:����ģ���ʼ��
//9.char.h����LCD.c����������ʾ����ˢ��0~9�Ĵ���
//10��Motor_Out()�мǵÿ�ƫ�ǹ���ı��������˷��ӵ����ʣ�ʣ��ʱ�򶼿�����ֹ��

/*�ҵ�����ϵ�͵�����
					
					1�ŵ��(Motor[0])
					+�з���Ƕ�
					-gyro.y
					-acc.x								
2�ŵ��(Motor[1])						4�ŵ����(Motor[3])
+�з���Ƕ�								-�з���Ƕ�
-gyro.y									+gyro.y
+acc.x									-acc.x					
					3�ŵ��(Motor[2])
					-�з���Ƕ�
					+gyro.y
					+acc.x
*/

#include "include.h"



/*************************************************************************
*  ģ�����ƣ�û������
*  ����˵��������ȫ�ֱ����Ķ����Լ���ʼ�������������뿴include.h
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14
*************************************************************************/
struct CarThreeNum      Acc_ADC_Data={0,0,0},   Acc_ADC={0,0,0},        Acc_Offset={0,0,0};
//Acc_ADC_Data�������˲��ļ��ٶȼƵ�ֵ���뿴Filter_2nd_LPF2ndFilter()
//Acc_ADC���ɼ��ļ��ٶȼƵ�ԭʼֵ���ڲ�ú���ƫ���ȥ����ƫ���뿴MPU6050_Read_Acc_Data()����
//Acc_Offset�����ٶȼ��ϵ�Ҫ�ɼ���3������ƫֵ���뿴MPU6050_CalOffset_Acc()����

struct CarThreeNum      Gyro_dps={0,0,0},       Gyro_ADC={0,0,0},       Gyro_Offset={0,0,0};
//Acc_ADC_Data�������˲��ļ��ٶȼƵ�ֵ���뿴Filter_2nd_LPF2ndFilter()
//Acc_ADC���ɼ��ļ��ٶȼƵ�ԭʼֵ���ڲ�ú���ƫ���ȥ����ƫ���뿴MPU6050_Read_Acc_Data()����
//Acc_Offset�����ٶȼ��ϵ�Ҫ�ɼ���3������ƫֵ���뿴MPU6050_CalOffset_Acc()����

struct CarAngle         Angle={0,0,0,0,0,0,0,0,0,0,0};//�����뿴include.c�е�����
struct CarFourNum       Q={1,0,0,0};//û�õ�
struct CarThreeNum      Gravity={0,0,0};//û�õ�

struct CarMotor Motor[4]= {{0},{0},{0},{0}};//4�������pwm�������뿴include.c�е�����
struct CarLcd   Lcd     = {0,0,0,{7,3}};//�����뿴include.c�е�����
struct CarMode  Mode    = {0,0,0,0,0,0,{{0}},{0}};//�����뿴include.c�е�����
long int        Time_1ms = 0;//ms���������ж�����++��




/*************************************************************************
*  �������ƣ�main
*  ����˵����������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14    һֱ�ڲ���
*************************************************************************/
void main()
{
    char str[40];//����ˢ��
    DisableInterrupts;
    delayms(100);
    System_Init();//�������ݺ�ģ���ʼ��
    delayms(100);
    EnableInterrupts;
    
	
    //while���ʱ���ǵȴ����ټƺ������ǲɼ�����ƫֵ��
	//�뿴MPU6050_updateSensor(),MPU6050_CalOffset_Gyro(),MPU6050_CalOffset_Acc(),MPU6050_Read_Acc_Data(),MPU6050_Read_Gyro_Data()
	//�ɼ���ɺ�Acc_CALIBRATED,Gyro_CALIBRATED������0
    while(Acc_CALIBRATED==1 || Gyro_CALIBRATED==1);
    Buzzer_Ring_ms(100);
	
	
	//��ѭ����һֱ��ˢ��
    while(1)
    {
		//��������ģʽ7����������ͷ�õ���b�ڵ�DMA��ò�ƺͰ������ⲿ�жϳ�ͻ�ˣ���������ͷһ����ʼ��
		//�Ͳ��ܰ����ˣ�ֻ�ܵ��磬��������ͷ�������⣬��ͻ�����⻹û�����Ӧ���ǿ�����⣬����Ժ�ע��
        if(Mode.mode==7 && Mode.startFg[Mode.mode]==1)
        {
            //ӥ������ͷ��ʼ��,�������жϺ�DMA
            static int first_run=0;
            if(first_run++==0)
            {
                Ov7725_Init();
                buzzer=0;
                Disp_single_colour(Yellow);
                Angle.aveL = 130;
                Angle.aveH = 71;
            }
            //ͼ��ɼ�����4�������ǲ�Ӭ�Ļ�ľ�������
            static int H1=10,H2=140,L1=70,L2=200;
            if(Time_1ms%300)//300ms����һ��
            {
                ov7725_get_img();//�ɼ�ͼ��
                Process_Image();//����ͼ�񣬵õ���Ӭ��λ��Angle.aveL��Angle.aveH
				//��Ҫ���Process_Image���������Process.c�еĺ�������Ϊ������û�а������.c��.h��������Ϊ̫����û������ļ�ɾ��
				//���Process_Image��LCD.c�еĺ���
             }
            if(sw8==1)//����ڰ�λ
            {
                Send_Image_to_LCD(Image_fire);
                Draw_single_line('H',H1,Black);
                Draw_single_line('H',H2,Black);
                Draw_single_line('L',L1,Black);
                Draw_single_line('L',L2,Black);
                
                Draw_single_line('H',Angle.aveH,Black);
                Draw_single_line('L',Angle.aveL,Black);
            }
            continue;
        }
            
        
		//���´�������ģʽ1~6�����»����ϲ�Ҫ�������漰�㷨��pidʲô�ģ�ֻ�ǵ����ĸ��ֻ�ʽˢ�����Ƚ��ҡ�����
		//˼·���ǣ��и����˵����и��μ��˵������˵���8��ģʽ����ѡ�񣬽���ĳ��ģʽ�����ʾ��Ӧģʽ�Ĳ�����
		//���¡���ʼ�����������ģʽ�������ˣ����¡�ȡ�����������˳�ִ��ģʽ�����¡�ȡ�����ͻص����˵�
        if(Lcd.flushFg == 1)//LCDˢ��һ��
        {
            Lcd.flushFg = 0;
            Disp_single_colour(Yellow);
        }
        
        //LCDģʽ��ʾ
        if(sw1 == 1)
        {
            //�����˵�
            if(Lcd.level==0)
            {
                char strMode[8][40]={"set PWM","L=60 , a Line","set Line Length","set Line Dir","stop!!!","set circle Length","circle vs fan","find fly pai"};
                for(int loop=0;loop <= Lcd.AddMax[ Lcd.level ];loop++)
                {
                    sprintf(str,"select Mode = %d : %s",loop,strMode[loop]);
                    if(loop == Lcd.Add) 
                        LCD_PutString(20,loop*20,str,Red,Green);
                    else 
                        LCD_PutString(20,loop*20,str,Red,Yellow);
                }
            }
            //�μ��˵�
            else if(Lcd.level==1)
            {
                //mode = 0 �μ��ı���PWM
                if(Mode.mode==0)
                {
                    for(int loop=0;loop <= Lcd.AddMax[1];loop++)
                    {
                        sprintf(str,"Mode : 0 , Motor[%d].PWM = %5d",loop,(int)(Motor[loop].PWM));
                        if(loop == Lcd.Add)LCD_PutString(20,loop*20,str,Red,Green);else LCD_PutString(20,loop*20,str,Red,Yellow);
                    }
                }
                //mode ��= 0 �μ��ı����
                else            
                {
                    char strPID[4][7]={"P   ","I   ","D   ","Length"};
                    if(Mode.mode == 3) sprintf( strPID[3],"Dir   ");
                    for(int loop=0;loop <= Lcd.AddMax[1];loop++)
                    {
                        //������ʾ
                        sprintf(str,"Mode = %d , %s = %5d", Mode.mode , strPID[loop] , Mode.para[Mode.mode][loop]);
                        if(loop == Lcd.Add)
                            LCD_PutString(20,loop*20,str,Red,Green);
                        else 
                            LCD_PutString(20,loop*20,str,Red,Yellow);
                    }
                    //��ʾģʽ�������
                    if(Mode.startFg[Mode.mode]==1)
                    {
                        sprintf(str,"start = %3ld",(Time_1ms - Mode.TimeStart)/1000);
                        printBigNum((Time_1ms - Mode.TimeStart)/1000%100,Lcd.AddMax[1]*20+40);
                        if(Time_1ms - Mode.TimeStart>0 && Time_1ms - Mode.TimeStart<1000)
                            buzzer = 1;
                        else
                            buzzer = 0;
                    }
                    else 
                        sprintf(str,"end  ");
                    LCD_PutString(20,Lcd.AddMax[1]*20+20,str,Red,Yellow);
                }
            }
        }
        else if(sw2 == 1)//��ʾ����
            ;//LcdPrintAnglePara();
        else if(sw3 == 1)//send infomation 
            SCI_Send_Datas(UART0);
        else
            Lcd.flushFg = 1;
        
        
        //����ʱ��,�������ԣ�ƽʱ��������ֻ������ʱ��Ĳ���
        #if 0
        static int Test_num=0;
        if(Test_num==1)
        {
            Test_IO = 1;         
            Test_num = 0;
            //��������ʱ��ĺ�����������
            Attitude_Updata_Quaternion(0.002);
            
        }
        else
        {
            Test_IO = 0;
            Test_num = 1;
            //��������ʱ��ĺ�����������
            Attitude_Updata_Quaternion(0.002);
            
        }
        #endif
    }
}
