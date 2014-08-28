#include <LedControl.h>

//分别定义片选引脚、时钟引脚、数据引脚
int csPin = 8;
int clockPin = 12;
int dataPin = 11;

void setup()
{
 Serial.begin(9600);
}

void loop()
{
    //创建LedControl 和 Window 类的对象
    LedControl led(dataPin, clockPin, csPin);
    Window win; 

    //设置一个起始地址为0，长度为8字符的显示窗口
    win.setWindow(0,8);
    led.windowPrint(win,String("      ") + "HELL0 BUddy--HELL0 PoPi");  /*在窗口内打印这些字符，如果字符长度超过窗口设置长度就进行滚动显示 */
    delay(1000);
    win.clearWindow();  //清除刚才设置的窗口

    //重新设置两个长度为4字符的窗口，起始地址分别为0和4
    win.setWindow(0,4);
    win.setWindow(4,4);
    led.windowPrint(win,String("    ") + "3q For SEEING",String("    ") + "HELL0 BUddy--HELL0 PoPi");/*在两个窗口同时打印两组字符，如果字符长度超过窗口设置长度就进行滚动显示 */
    delay(500);
    win.clearWindow();
    win.setWindow(0,3);
    win.setWindow(3,5);
   for(int i=0; i<2; i++) led.windowPrint(win,"qq=",String("     ") + "373615934");
    delay(500);
    led.print("L0UE ALL");/*在显示屏上静态打印字符串*/
  
  while(true);
}
