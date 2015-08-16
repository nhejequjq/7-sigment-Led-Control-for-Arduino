#include <LedControl.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//定义温度传感器(DS18B20)的引脚
const int DS18B20_PIN = 6;

//创建LedControl 和 Window 类的对象
LedControl led;
Window win; 

//定义初始时间
unsigned int clock = 20;
unsigned int minute = 03;
unsigned int second = 55;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  for(int i = 0; i < 5; i++) ledClock();
  ledThermometer();
}

void ledClock()
{
    led.print("  -  -  ");  //显示时间分隔符 -
    win.clearWindow();
    win.setWindow(0,2);   //设置“时”位的显示
    win.setWindow(3,2);   //设置“分”位的显示
    win.setWindow(6,2);   //设置“秒”位的显示

    static unsigned long newTime = 0;
    static unsigned long oldTime = 0;
    static unsigned int accumTimeErr = 0;
    unsigned int increSecond = 0;
    unsigned int timeStep = 0;
    
    newTime = millis();
    timeStep = newTime - oldTime;
    increSecond = (timeStep + accumTimeErr) / 1000;
    accumTimeErr = timeStep + accumTimeErr - increSecond * 1000;
    second += increSecond;
	
    if(second >= 60){
      minute += 1;
      second %= 60;
      if(minute >= 60){
        clock += 1;
        minute %= 60;
        if(clock >= 24){
          clock %= 24;
        }
      }
    }
    led.windowPrint(win, clock, minute, second);   //显示当前时间
    oldTime = newTime;
    delay(215);
}

void ledThermometer()
{
  double temp = 0;
  //创建一个单总线实例与单总线协议设备通信 
  OneWire oneWire(DS18B20_PIN);

  DallasTemperature sensors(&oneWire);
  sensors.begin();
  sensors.requestTemperatures(); //发送命令获取温度
  temp = sensors.getTempCByIndex(0);
  
  led.print("        ");  //清除屏幕
  win.clearWindow();
  win.setWindow(0,2);
  win.setWindow(2,2);
  win.setWindow(4,2);
  win.setWindow(6,2);
  led.windowPrint(win,"~-_-~~-_-~~-_-~", "t=", String(int(temp)), "~-_-~~-_-~~-_-~");
}
