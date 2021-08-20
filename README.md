#=================================
#7-sigment-Led-Control-for-Arduino
#=================================

这是一个驱动 SPI LED module （7段数码管组成的显示屏）的Arduino类。可以控制多种显示效果，比如静态显示、滚动显示、静态、滚动混合显示。 <br>

重要说明：本类库主要针对RobotBase 公司和DFRobot公司生产的 SPI LED module （7段数码管组成的显示屏）进行设计。
SPI LED module （7段数码管组成的显示屏）的具体介绍如下：<br>
RobotBase公司网址：http://item.taobao.com/item.htm?spm=a230r.1.14.16.umb7YC&id=15848381217&ns=1#detail<br>
DFRobot公司网址：  http://item.taobao.com/item.htm?spm=a230r.1.0.0.umb7YC&id=7457884485&ns=1#detail<br>

》》静态显示效果介绍：<br>
//创建LedControl 类的对象<br>
LedControl led(dataPin, clockPin, csPin);<br>
led.print("L0UE ALL");  //显示字符串<br>
led.print('s');         //显示字符<br>
led.print(1234);        //显示整型<br>
led.print(3.1415926,4); //显示浮点型，并指定4位精度<br><br>

》》滚动显示效果介绍：<br>
//创建LedControl 和 Window 类的对象<br>
    LedControl led;<br>
    Window win; <br>
    win.setWindow(0,8);<br>
    /*在窗口内打印这些字符，如果字符长度超过窗口设置长度就进行滚动显示 */<br>
    led.windowPrint(win,String("      ") + "HELL0 BUddy--HELL0 PoPi"); //显示字符串<br>
    led.windowPrint(win,'s');                                          //显示字符<br>
    led.windowPrint(win,1234);                                         //显示整型<br>
    led.windowPrint(win,3.1415926);                                    //显示浮点型，并指定4位精度<br>
    <br>
》》静态、滚动显示效果介绍：<br>
//创建LedControl 和 Window 类的对象<br>
LedControl led;<br>
Window win; <br>
win.setWindow(0,3);<br>
win.setWindow(3,5);<br>
led.windowPrint(win,"qq=",String("     ") + "373615934");  //两窗口同时显示字符串<br>
led.windowPrint(win,123,373615934);                        //两窗口同时显示整型<br>
led.windowPrint(win,3.1,3.141592654);                      //两窗口同时显示浮点型<br>
led.windowPrint(win,'a','b');                              //两窗口同时显示字符<br>
