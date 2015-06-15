#include <Arduino.h>


boolean flagCarmelaStop;
boolean flagCarmelaForward;
boolean flagCarmelaBackward;
boolean flagCarmelaRight;
boolean flagCarmelaLeft;
boolean flagCarmelaPIDXY;
boolean flagCarmelaPIDNXY;
boolean flagCarmelaPIDPolar;
boolean flagCarmelaPIDNPolar;
boolean flagCarmelaLeftAngle;
boolean flagCarmelaRightAngle;
boolean flagCarmelaStopReply;
boolean flagCarmelaReplyPose;
boolean flagCarmelaReplyRAWDATA;
boolean flagCarmelaReplyBoth;

void CarmelaStop()
{
  int i,j;
  //Packetized serial mode
  //0xAA (��ģʽ��һ���ַ�)
  //0x82  �������õĵ�ַ��130 = 0x82
  //0x00 ���a��00��ǰ01���b��04��ǰ05���
  //0x00 �ٶ�ֵ����Χ��0x00 - 0x80
  //0x02 У��λ = (0x82+0x00+0x00)&0x80
  char a[5]={0xAA,0x82,0x00,0x00,0x02};
  char b[5]={0xAA,0x82,0x04,0x00,0x06};

  for(i=0;i<5;i++)//ͨ������1�����Ƶ��������
  {
    Serial1.print(a[i]);
  }
  for(j=0;j<5;j++)
  {
    Serial1.print(b[j]);
  }
}

void CarmelaForward()
{
  int i,j;
  char a[5]={0xAA,0x82,0x00,0x30,0x32};
  char b[5]={0xAA,0x82,0x04,0x30,0x36};
  for(i=0;i<5;i++) //ͨ������1�����Ƶ��������
  {
    Serial1.print(a[i]);
  }
  for(j=0;j<5;j++)
  {
    Serial1.print(b[j]);
  }
}

void CarmelaBackward()
{
  int i,j;
  char a[5]={0xAA,0x82,0x01,0x30,0x33};
  char b[5]={0xAA,0x82,0x05,0x30,0x37};
  for(i=0;i<5;i++)//ͨ������1�����Ƶ��������
  {
    Serial1.print(a[i]);
  }
  for(j=0;j<5;j++)
  {
    Serial1.print(b[j]);
  }
}

void CarmelaRight()
{
  int i,j;
  char a[5]={0xAA,0x82,0x00,0x30,0x32};
  char b[5]={0xAA,0x82,0x05,0x30,0x37};
  for(i=0;i<5;i++)//ͨ������1�����Ƶ��������
  {
    Serial1.print(a[i]);
  }
  for(j=0;j<5;j++)
  {
    Serial1.print(b[j]);
  }
}

void CarmelaLeft()
{
  int i,j;
  char a[5]={0xAA,0x82,0x01,0x30,0x33};
  char b[5]={0xAA,0x82,0x04,0x30,0x36};
  for(i=0;i<5;i++)//ͨ������1�����Ƶ��������
  {
    Serial1.print(a[i]);
  }
  for(j=0;j<5;j++)
  {
    Serial1.print(b[j]);
  }
}

void CarmelaPIDXY()
{
  Serial3.println("Carmela PIDXY!");
}

void CarmelaPIDNXY()
{
  Serial3.println("Carmela PIDNXY!");
}

void CarmelaPIDPolar()
{
  Serial3.println("Carmela PIDPolar!");
}

void CarmelaPIDNPolar()
{
  Serial3.println("Carmela PIDNPolar!");
}

void CarmelaLeftAngle()
{
  Serial3.println("Carmela LeftAngle!");
}

void CarmelaRightAngle()
{
  Serial3.println("Carmela RightAngle!");
}

void CarmelaStopReply()
{
  flagCarmelaReplyPose = 0;
  flagCarmelaReplyRAWDATA = 0;
  flagCarmelaReplyBoth = 0;

  Serial3.println("Carmela StopReply!");
}

/***********CarmelaAction()***********
�������ܣ���λ�����ݱ�־λ����Ӧ�Ķ���
�����������
�����������
*************************************/
void CarmelaAction()
{
  if(flagCarmelaStop)
  {
    CarmelaStop();
  }

  if(flagCarmelaForward)
  {
    CarmelaForward();
  }

  if(flagCarmelaBackward)
  {
    CarmelaBackward();
  }

  if(flagCarmelaRight)
  {
    CarmelaRight();
  }

  if(flagCarmelaLeft)
  {
    CarmelaLeft();
  }

  if(flagCarmelaPIDXY)
  {
    CarmelaPIDXY();
  }

  if(flagCarmelaPIDNXY)
  {
    CarmelaPIDNXY();
  }

  if(flagCarmelaPIDPolar)
  {
    CarmelaPIDPolar();
  }

  if(flagCarmelaPIDNPolar)
  {
    CarmelaPIDNPolar();
  }

  if(flagCarmelaLeftAngle)
  {
    CarmelaLeftAngle();
  }

  if(flagCarmelaRightAngle)
  {
    CarmelaRightAngle();
  }
    if(flagCarmelaStopReply)
  {
    CarmelaStopReply();
  }

}



