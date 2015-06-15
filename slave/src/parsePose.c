#include <Arduino.h>

//�����ȡ�������Ĺܽ�
#define LEFT_FRONT_B   5 // General pin
#define LEFT_REAR_B    6
#define RIGHT_FRONT_B  7
#define RIGHT_REAR_B   8
#define ENCODING_SLOT  2  //����������ʱ���Ӳ�


#define CARMELA_L   18   //С������Cm
#define CARMELA_B   28  //С�����Cm
#define CARMELA_C   40    //С�����ӵ��ܳ�Cm
#define CARMELA_P   700   // = 43.7*16  (��m*n*w)

//�ĸ����ӱ������ļ���
long rightfrontEncoderCnt = 0;
long rightrearEncoderCnt = 0;
long leftrearEncoderCnt = 0;
long leftfrontEncoderCnt = 0;

//λ�˽����е�����ת��ǰ����־
bool flagMotionForward = false;
bool flagMotionRight = false;
bool flagMotionLeft = false;


/***motionModel(long leftCnt,long rightCnt)***
�������ܣ��ɱ��������ݼ����ߵľ����ת�ĽǶ�
�������������������������
�����������
************************************************/
void motionModel(long leftCnt,long rightCnt)
{
    float instAngle; //�������ʱ�����ߵĽǶ�
    float instDistance;//�������ʱ�����ߵľ���

    instAngle = CARMELA_C * (rightCnt - leftCnt)*1.0/(CARMELA_P*CARMELA_B);
    instAngle = instAngle * 180.0 / PI;

    instDistance = CARMELA_C*(rightCnt + leftCnt)*1.0/(CARMELA_P*2.0);

    if(instAngle > 0.2) //��Ϊ��ת ��������
    {
        flagMotionLeft = 1;
        flagMotionRight = 0;
        flagMotionForward = 0;
    }
    else if((instAngle <= 0.2)&&(instAngle >= -0.2))
        //��Ϊǰ�� ��������
        {
            flagMotionLeft = 0;
            flagMotionRight = 0;
            flagMotionForward = 1;
        }
        else    //��Ϊ��ת ��������
        {
            flagMotionLeft = 0;
            flagMotionRight = 1;
            flagMotionForward = 0;
        }
}



/********leftfrontEncoder()******************
�������ܣ����ú����жϣ���ȡ��ǰ�ֱ���������
�����������
�����������
********************************************/
void leftfrontEncoder()
{
  static unsigned long leftfrontTime = 0;

  if(micros()- leftfrontTime>ENCODING_SLOT ) //�˲�
  {
    if(0 == digitalRead(LEFT_FRONT_B))
    //����������ο�֪��A�½��أ�B�͵�ƽ����ת
    {
        leftfrontEncoderCnt ++;
    }
    else
    {
        leftfrontEncoderCnt --;
    }
    leftfrontTime = micros();
  }
}
/********leftrearEncoder()******************
�������ܣ����ú����жϣ���ȡ����ֱ���������
�����������
�����������
********************************************/
void leftrearEncoder()
{
  static unsigned long leftrearTime = 0;

  if(micros()- leftrearTime>ENCODING_SLOT )
  {
    if(0 == digitalRead(LEFT_REAR_B))
    //����������ο�֪��A�½��أ�B�͵�ƽ����ת
    {
        leftrearEncoderCnt ++;
    }
    else
    {
        leftrearEncoderCnt --;
    }
    leftrearTime = micros();
  }
}
/********rightfrontEncoder()******************
�������ܣ����ú����жϣ���ȡ��ǰ�ֱ���������
�����������
�����������
********************************************/
void rightfrontEncoder()
{
  static unsigned long rightfrontTime = 0;

  if(micros()- rightfrontTime>ENCODING_SLOT )
  {
    if(0 == digitalRead(RIGHT_FRONT_B))
    //����������ο�֪��A�½��أ�B�͵�ƽ����ת
    //ʵ�⣺�������������෴��
    {
        rightfrontEncoderCnt --;
    }
    else
    {
        rightfrontEncoderCnt ++;
    }
    rightfrontTime = micros();
  }
}
/********rightrearEncoder()******************
�������ܣ����ú����жϣ���ȡ�Һ��ֱ���������
�����������
�����������
********************************************/
void rightrearEncoder()
{
  static unsigned long rightrearTime = 0;

  if(micros()- rightrearTime>ENCODING_SLOT )
  //����������ο�֪��A�½��أ�B�͵�ƽ����ת
  //ʵ�⣺�������������෴��
  {
    if(0 == digitalRead(RIGHT_REAR_B))
    {
        rightrearEncoderCnt --;
    }
    else
    {
        rightrearEncoderCnt ++;
    }
    rightrearTime = micros();
  }
}








