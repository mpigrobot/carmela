#include <Arduino.h>
#include "src/parsePose.h"
#include "src/CarmelaReport.h"
#include "src/parsePackage.h"
#include "src/CarmelaAction.h"

//定义读取编码器的管脚
#define LEFT_FRONT_A  0 //pin 2  //interrupt pin
#define LEFT_FRONT_B  5 // General pin
#define LEFT_REAR_A  1  // pin 3
#define LEFT_REAR_B  6
#define RIGHT_FRONT_A  2 //pin 21
#define RIGHT_FRONT_B  7
#define RIGHT_REAR_A  3 // pin 20
#define RIGHT_REAR_B  8
#define ENCODING_SLOT   2  //编码器读数时防杂波

#define BAUD    9600 //波特率的设置

//声明中断函数
void leftfrontEncoder();
void leftrearEncoder();
void rightfrontEncoder();
void rightrearEncoder();


const int LED = 13;

void txetLED()//调试用函数：1S闪灯
{
  static unsigned long timeLED = 0;
  static char  LEDState = 0;

  if(millis() - timeLED > 1000)
  {
    timeLED = millis();

    LEDState = !LEDState;
    digitalWrite(LED,LEDState);
  }
}

void testAction()//调试用函数
{
    long testTime;

    testTime = millis();
    if(testTime > 5000 && testTime < 6000)
    {
      CarmelaForward();
    }
    if(testTime > 6000 && testTime < 7000)
    {
        CarmelaStop();
    }
}

void setup()
{
  Serial3.begin(BAUD); //串口3用于蓝牙通信
  Serial1.begin(BAUD); //串口1用于给电机驱动板发送命令

  pinMode(LED,OUTPUT); //LED管脚用于测试

  pinMode(LEFT_FRONT_A,INPUT); //4轮8个编码器接口，定义为输入
  pinMode(LEFT_FRONT_B,INPUT);
  pinMode(LEFT_REAR_A,INPUT);
  pinMode(LEFT_REAR_B,INPUT);
  pinMode(RIGHT_FRONT_A ,INPUT);
  pinMode(RIGHT_FRONT_B ,INPUT);
  pinMode(RIGHT_REAR_A,INPUT);
  pinMode(RIGHT_REAR_B,INPUT);

 //用外部中断的方式读取4个编码器的读数，下降沿触发中断
  attachInterrupt(LEFT_FRONT_A,leftfrontEncoder,FALLING);
  attachInterrupt(LEFT_REAR_A,leftrearEncoder,FALLING);
  attachInterrupt(RIGHT_FRONT_A,rightfrontEncoder,FALLING);
  attachInterrupt(RIGHT_REAR_A,rightrearEncoder,FALLING);

  sampleInterval = 500; //采样、上报时间间隔,默认为0.5s
  flagCarmelaReplyRAWDATA = true;//默认上报编码器原始数据
}

void loop()
{
    startParsePackage();//解析上位机发来的命令
    report();//分析运动状态并上报。

    txetLED();
    testAction();
    delay(20);
}







