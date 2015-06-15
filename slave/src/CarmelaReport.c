#include <Arduino.h>
#include "src/parsePackage.h"
#include "src/ParsePose.h"
#include "src/CarmelaAction.h"

#define BUF_LEN           64
#define RES_LEN           30

#define MAX_PACKAGE_LEN   35
#define MIN_PACKAGE_LEN   8

#define PACKAGE_SLOT      5
#define PACKAGE_HEADER    0x7E
#define PROTOCOL_VERSION  0x31
#define PACKAGE_TAIL      0x0D
#define ARDUINO_ADDRESS   0x01
#define PC_ADDRESS        0x00

#define PACKAGE_UP_REPORT    0x00
#define PACKAGE_DOWN_CMD     0xFF

#define CARMELA_STOP      0x00
#define CARMELA_FORWARD   0x01
#define CARMELA_BACKWARD  0x02
#define CARMELA_RIGHT     0x03
#define CARMELA_LEFT      0x04
#define CARMELA_PID_XY    0x10
#define CARMELA_PID_NXY   0x11
#define CARMELA_PID_POLAR 0x20
#define CARMELA_PID_NPOLAR 0x21
#define CARMELA_PID_LEFT_ANGLE  0x30
#define CARMELA_PID_RIGHT_ANGLE  0x31
#define CARMELA_STOP_REPLY       0x40
#define CARMELA_REPLY_POSE       0x41
#define CARMELA_REPLY_RAWDATA    0x42
#define CARMELA_REPLY_BOTH       0x43

//�ĸ���������ֵ���ݴ����
int leftfrontCntTmp,rightfrontCntTmp;
int leftrearCntTmp,rightrearCntTmp;

//�ϱ�λ����Ϣ
void CarmelaReplyPose(unsigned char heartBeat, int X, int Y,int course,int difS,int difR);

//�ϱ�������ԭʼ����
void CarmelaReplyRAWDATA(unsigned char heartBeat, int leftfrontCnt, int leftrearCnt,int rightfrontCnt,int rightrearCnt,char leftFlag,char rightFlag);

//�ϱ�λ�˺ͱ�����ԭʼ����
void CarmelaReplyBoth();

/*********CarmelaReplyPose��...��*********
�������ܣ��ϱ���λ����λ����Ϣ
���������������־,λ����Ϣ
�����������
*****************************************/
void CarmelaReplyPose(unsigned char heartBeat, int X, int Y,int course,int difS,int difR)////////////////////////////////////
{
  char resBuf[RES_LEN];
  char cnt;
  char checkSum = 0;

  resBuf[0] = PACKAGE_HEADER;
  resBuf[1] = PROTOCOL_VERSION;
  resBuf[2] = PC_ADDRESS;
  resBuf[3] = CARMELA_REPLY_POSE;
  resBuf[4] = PACKAGE_UP_REPORT;
  resBuf[5] = 0x0B;
  resBuf[6] = heartBeat;
  resBuf[7] = X >> 8;
  resBuf[8] = X ;
  resBuf[9] = Y >> 8;
  resBuf[10] = Y ;
  resBuf[11] = course >>8;
  resBuf[12] = course;
  resBuf[13] = difS >>8;
  resBuf[14] = difS;
  resBuf[15] = difR >>8;
  resBuf[16] = difR;
  for(cnt=1;cnt <= 16;cnt++)
  {
      checkSum += resBuf[cnt];
  }
  resBuf[17] = checkSum;
  resBuf[18] = PACKAGE_TAIL;

  for(cnt = 0;cnt <= 18;cnt++)
  {
    Serial3.write(resBuf[cnt]);
  }
}
/*********CarmelaReplyRAWDATA��...��******
�������ܣ��ϱ���������ԭʼ����
���������������־,4����������Ԫ������
�����������
*****************************************/
void CarmelaReplyRAWDATA(unsigned char heartBeat, int leftfrontCnt, int leftrearCnt,
                         int rightfrontCnt,int rightrearCnt,char leftFlag,char rightFlag)
{
  char resBuf[RES_LEN];
  char cnt;
  char checkSum = 0;

  resBuf[0] = PACKAGE_HEADER;
  resBuf[1] = PROTOCOL_VERSION;
  resBuf[2] = PC_ADDRESS;
  resBuf[3] = CARMELA_REPLY_RAWDATA;
  resBuf[4] = PACKAGE_UP_REPORT;
  resBuf[5] = 0x0B;
  resBuf[6] = heartBeat;
  resBuf[7] = leftfrontCnt >> 8;
  resBuf[8] = leftfrontCnt ;
  resBuf[9] = leftrearCnt >> 8;
  resBuf[10] = leftrearCnt ;
  resBuf[11] = rightfrontCnt >>8;
  resBuf[12] = rightfrontCnt;
  resBuf[13] = rightrearCnt >>8;
  resBuf[14] = rightrearCnt;
  resBuf[15] = leftFlag;
  resBuf[16] = rightFlag;
  for(cnt=1;cnt <= 16;cnt++)
  {
      checkSum += resBuf[cnt];
  }
  resBuf[17] = checkSum;
  resBuf[18] = PACKAGE_TAIL;

  for(cnt = 0;cnt <= 18;cnt++)
  {
    Serial3.write(resBuf[cnt]);
  }
}
/*
void CarmelaReplyBoth()
{
  char resBuf[RES_LEN];
  char cnt;
  char checkSum = 0;

  resBuf[0] = PACKAGE_HEADER;
  resBuf[1] = PROTOCOL_VERSION;
  resBuf[2] = PC_ADDRESS;
  resBuf[3] = CARMELA_REPLY_POSE;
  resBuf[4] = PACKAGE_UP_REPORT;
  resBuf[5] = 0x15; //21
  resBuf[6] = heartBeat;
  resBuf[7] = X >> 8;
  resBuf[8] = X ;
  resBuf[9] = Y >> 8;
  resBuf[10] = Y ;
  resBuf[11] = course >>8;
  resBuf[12] = course;
  resBuf[13] = difS >>8;
  resBuf[14] = difS;
  resBuf[15] = difR >>8;
  resBuf[16] = difR;
  resBuf[17] = leftfrontCnt >> 8;
  resBuf[18] = leftfrontCnt ;
  resBuf[19] = leftrearCnt >> 8;
  resBuf[20] = leftrearCnt ;
  resBuf[21] = rightfrontCnt >>8;
  resBuf[22] = rightfrontCnt;
  resBuf[23] = rightrearCnt >>8;
  resBuf[24] = rightrearCnt;
  resBuf[25] = leftFlag;
  resBuf[26] = rightFlag;
  for(cnt=1;cnt <= 26;cnt++)
  {
      checkSum += resBUf[cnt];
  }
  resBuf[27] = checkSum;
  resBuf[28] = PACKAGE_TAIL;

  for(cnt = 0;cnt <= 28;cnt++)
  {
    Serial3.write(resBuf[cnt]);
  }
  Serial3.println("Carmela ReplyBoth!");
}
*/

/****report(unsigned int SampleInterval)****
�������ܣ�ÿSampleIntervalʱ���ϱ�һ��
���������SamlpeInterval
�����������
********************************************/
void report()
{
    static int heartBoo = 0; //������־
    static unsigned long sampleTime = 0;//��ǰ�Ĳ���

    if(millis() - sampleTime >= sampleInterval) //ÿsampleInterval��
    {                                           //��������������λ�ˡ����ϱ�
        sampleTime = millis();

        leftfrontCntTmp = leftfrontEncoderCnt;
        rightfrontCntTmp = rightfrontEncoderCnt;
        leftrearCntTmp = leftrearEncoderCnt;
        rightrearCntTmp = rightrearEncoderCnt;

        leftfrontEncoderCnt = 0;
        rightfrontEncoderCnt = 0;
        leftrearEncoderCnt = 0;
        rightrearEncoderCnt = 0;

        motionModel(leftfrontCntTmp,rightfrontCntTmp);//�����˶�ģ��

        heartBoo ++;

          if(flagCarmelaReplyPose)
          {
            CarmelaReplyPose(heartBoo, 1, 1,1,1,1);//
          }

          if(flagCarmelaReplyRAWDATA)
          {
            CarmelaReplyRAWDATA(heartBoo, leftfrontCntTmp,leftrearCntTmp,rightfrontCntTmp,rightrearCntTmp,
                            leftfrontCntTmp/abs(leftfrontCntTmp),rightfrontCntTmp/abs(rightfrontCntTmp));
                            //��ΪʲôҪ�ϴ���������������
          }

           if(flagCarmelaReplyBoth)
          {
  //          CarmelaReplyBoth();
          }
    }

}

