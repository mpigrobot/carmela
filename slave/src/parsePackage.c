#include <Arduino.h>
#include "src/CarmelaAction.h"


#define BUF_LEN           64
#define RES_LEN           30

#define MAX_PACKAGE_LEN   35
#define MIN_PACKAGE_LEN   8
#define BAUD              9600
#define PACKAGE_SLOT      5 //�������ݰ����ʱ��������λms
#define PACKAGE_HEADER    0x7E
#define PROTOCOL_VERSION  0x31
#define PACKAGE_TAIL      0x0D
#define ARDUINO_ADDRESS   0x01
#define PC_ADDRESS        0x00

#define PACKAGE_UP_REPORT    0x00
#define PACKAGE_DOWN_CMD     0xFF

//���ݴ���Э�鶨�������
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

unsigned long previousMillis = 0; //��ǰʱ��
bool maxCommandReceived = false;//�������������־λ
char buff[BUF_LEN] = {};  //���մ������������
char buffIndex = 0;     //���������ݵĸ���
unsigned int sampleInterval;//�ϱ����ݵ�ʱ����

void startParsePackage();//�Ƿ���Ҫ������
char findPackageHead(char bufflen); //�ҵ����ݰ���ͷ
bool parsePackage(char buffLen);//�������ݰ�������

void flagToStop();      //ֹͣ��־��1��������0
void flagToForward();   //ǰ����־��1��������0
void flagToBackward();  //���˱�־��1��������0
void flagToRight();     //��ת��־��1��������0
void flagToLeft();      //��ת��־��1��������0
void flagToPIDXY();     //ʹ��PID��ָ��������־��1��������0
void flagToPIDNXY();    //ʹ��PID��N��ָ��������־��1��������0
void flagToPIDPolar();  //ʹ��PID����������־��1��������0
void flagToPIDNPolar(); //ʹ��PID��N����������־��1��������0
void flagToLeftAngle(); //��ת�̶��Ƕȱ�־��1��������0
void flagToRightAngle();//��ת�̶��Ƕȱ�־��1��������0
void flagToStopReply(); //ֹͣ�ϴ���Ϣ��־��1��������0
void flagToReplyPose(); //���ϴ�λ����Ϣ��־��1��������0
void flagToReplyRAWDATA();//���ϴ����������ݱ�־��1��������0
void flagToReplyBoth();   //λ�˺ͱ���������һ���ϴ���־��1��������0

/********flagToStop()**************
�������ܣ���λ��ֹͣ��־λ��λ
�����������
�����������
***********************************/
void flagToStop()
{
  flagCarmelaStop = true;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToForward()***********
�������ܣ���λ��ǰ����־λ��λ
�����������
�����������
***********************************/
void flagToForward()
{
  flagCarmelaStop = false;
  flagCarmelaForward = true;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToBackward()**********
�������ܣ���λ�����˱�־λ��λ
�����������
�����������
***********************************/
void flagToBackward()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = true;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToRight()**************
�������ܣ���λ����ת��־λ��λ
�����������
�����������
***********************************/
void flagToRight()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = true;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToLeft()**************
�������ܣ���λ����ת��־λ��λ
�����������
�����������
***********************************/
void flagToLeft()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = true;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToPIDXY()****************
�������ܣ���λ��PID��ʽ��ָ������λ��
            ��־λ��λ
�����������
�����������
*************************************/
void flagToPIDXY()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = true;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToPIDNXY()**************
�������ܣ���λ��PID��ʽ������N��
            ָ������λ�ñ�־λ��λ
�����������
�����������
***********************************/
void flagToPIDNXY()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = true;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToPIDPolar()**************
�������ܣ���λ��PID��ʽ��ָ��������λ��
            ��־λ��λ
�����������
�����������
***************************************/
void flagToPIDPolar()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = true;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToPIDNPolar()**************
�������ܣ���λ��PID��ʽ��N��ָ��������λ��
            ��־λ��λ
�����������
�����������
***************************************/
void flagToPIDNPolar()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = true;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToLeftAngle()**************
�������ܣ���λ����תָ���Ƕȱ�־λ��λ
�����������
�����������
***************************************/
void flagToLeftAngle()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = true;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
 // flagCarmelaReplyBoth = false;
}
/********flagToRightAngle()**************
�������ܣ���λ����תָ���Ƕȱ�־λ��λ
�����������
�����������
***************************************/
void flagToRightAngle()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = true;
  flagCarmelaStopReply = false;
  //flagCarmelaReplyPose = false;
  //flagCarmelaReplyRAWDATA = false;
  //flagCarmelaReplyBoth = false;
}
/********flagToStopReply()**************
�������ܣ���λ��ֹͣ�ϱ���־λ��λ
�����������
�����������
***************************************/
void flagToStopReply()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = true;
  flagCarmelaReplyPose = false;
  flagCarmelaReplyRAWDATA = false;
  flagCarmelaReplyBoth = false;
}
/********flagToReplyPose()**************
�������ܣ���λ���ϱ�λ�˱�־λ��λ
�����������
�����������
***************************************/
void flagToReplyPose()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  flagCarmelaReplyPose = true;
  flagCarmelaReplyRAWDATA = false;
  flagCarmelaReplyBoth = false;
}
/********flagToReplyRAWDATA()**************
�������ܣ���λ���ϱ����������ݱ�־λ��λ
�����������
�����������
***************************************/
void flagToReplyRAWDATA()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  flagCarmelaReplyPose = false;
  flagCarmelaReplyRAWDATA = true;
  flagCarmelaReplyBoth = false;
}
/********flagToLeftAngle()**************
�������ܣ���λ���ϱ�λ�úͱ���������
            ��־λ��λ
�����������
�����������
***************************************/
void flagToReplyBoth()
{
  flagCarmelaStop = false;
  flagCarmelaForward = false;
  flagCarmelaBackward = false;
  flagCarmelaRight = false;
  flagCarmelaLeft = false;
  flagCarmelaPIDXY = false;
  flagCarmelaPIDNXY = false;
  flagCarmelaPIDPolar = false;
  flagCarmelaPIDNPolar = false;
  flagCarmelaLeftAngle = false;
  flagCarmelaRightAngle = false;
  flagCarmelaStopReply = false;
  flagCarmelaReplyPose = false;
  flagCarmelaReplyRAWDATA = false;
  flagCarmelaReplyBoth = true;
}

/*****char findPackageHead��char bufflen��******
�������ܣ��ҵ���ͷ��λ��
����������յ�����ĳ���
���ز���: ��ͷ��λ��
***********************************************/
char findPackageHead(char bufflen)
{
  char offset = 0; //��ͷ��λ��

  do
  {
    if(PACKAGE_HEADER == buff[offset])
    {
      if(PROTOCOL_VERSION == buff[offset+1])
      {
        if(ARDUINO_ADDRESS == buff[offset+2])
        {
          if(PACKAGE_HEADER !=buff[offset+3]) // avoid 7E 31 01 7E 31 01 00 FF 00 31 0D
          {
            return offset;
          }
          else
          {
            offset += 2; //should +3 ,but offset++ below
          }
        }
      }
    }
    offset++;
  }while(offset <= bufflen - MIN_PACKAGE_LEN);

  return offset;
}

/*****bool parsePackage(char brffLen)******
�������ܣ�������������
����������յ�����ĳ���
�������������������ȷ����1������ȷ����0
*********************************************/
bool parsePackage(char buffLen)
{
  char packageHead = 0;//��ͷλ��
  char dataLen = 0;  //parameter length
  char checkSum = 0;

  packageHead = findPackageHead(buffLen);    //packageHead is the position
  if(packageHead > buffLen - MIN_PACKAGE_LEN ) // not found the head
  {
    return false;
  }
   //found the head
    dataLen = buff[packageHead+5];
    if(PACKAGE_TAIL != buff[packageHead+dataLen+7]) //the tail is not right, return false
    {
      return false;
    }
    //the tail is right
    for(char i = 1;i<=dataLen+5;i++)
    {
      checkSum += buff[packageHead+i];
    }
    if(checkSum != buff[packageHead + dataLen +6])
    {
      return false;
    }
    switch (buff[packageHead+3])     //analysis the CTF/RSF
    {
      case CARMELA_STOP:           flagToStop();         break;
      case CARMELA_FORWARD:        flagToForward();      break;
      case CARMELA_BACKWARD:       flagToBackward();     break;
      case CARMELA_RIGHT:          flagToRight();        break;
      case CARMELA_LEFT:           flagToLeft();         break;
      case CARMELA_PID_XY:         flagToPIDXY();        break;
      case CARMELA_PID_NXY:        flagToPIDNXY();       break;
      case CARMELA_PID_POLAR:      flagToPIDPolar();     break;
      case CARMELA_PID_NPOLAR:     flagToPIDNPolar();    break;
      case CARMELA_PID_LEFT_ANGLE: flagToLeftAngle();    break;
      case CARMELA_PID_RIGHT_ANGLE:flagToRightAngle();   break;
      case CARMELA_STOP_REPLY:     flagToStopReply();    break;//�ر��ϱ�
      case CARMELA_REPLY_POSE:     flagToReplyPose();
                                   if(0x01 == buff[5]){
                                      sampleInterval = buff[6]*100; //0.1��Ϊ��λ
                                   }
                                   else{
                                       return false;
                                   }
                                    break;

      case CARMELA_REPLY_RAWDATA:  flagToReplyRAWDATA();
                                   if(0x01 == buff[5]){
                                      sampleInterval = buff[6]*100; //0.1��Ϊ��λ
                                   }
                                   else{
                                       return false;
                                   }
                                    break;

      case CARMELA_REPLY_BOTH:     flagToReplyBoth();
                                   if(0x01 == buff[5]){
                                      sampleInterval = buff[6]*100; //0.1��Ϊ��λ
                                   }
                                   else{
                                       return false;
                                   }
                                    break;
      default: return false;
    }
    return true;
}
//----------------------------------------------------------------//

/*****void startParsePackage()******
�������ܣ����������󣬿�ʼ�����������
        ������ȷ������λ��ִ����Ӧ�����
�����������
�����������
************************************/
void startParsePackage()
{
  if(buffIndex&&(millis()-previousMillis>PACKAGE_SLOT))
    //�����������ݲ���һ֡���ݷ�������
 {
    maxCommandReceived = false;//this
    if(parsePackage(buffIndex) == true) //��������ȷ
    {
      CarmelaAction();
    }
    else
    {
      //��������
      Serial3.write(4);
    }
    buffIndex = 0; //������ɺ������е�������0
  }
}

/********Serial3 Interrupt*********************
�������ܣ�����λ�����������ݴ�ŵ�����������
�����������
�����������
***********************************************/
void serialEvent3() //����3ʹ������ͨ�ţ�����0Ϊ����ͨ��
{
  while(Serial3.available())
  {
    buff[buffIndex++] = Serial3.read();
    if(buffIndex >= MAX_PACKAGE_LEN) //this ?
    {
      maxCommandReceived = true;
      Serial3.flush();
    }
  }
  previousMillis = millis(); //��ȡ��ǰʱ�䣬ȥ�ж�һ��
                            //���ݰ��Ƿ����ꡣ
}
