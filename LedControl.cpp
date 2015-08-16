
/********************************************
版权：撸主享有所有权
作者：亨特胥
日期：2015年6月30日
开源协议：GPLv3
联系方式:xmj8888@hotmail.com
********************************************/

#include "LedControl.h"


Window::Window()
{
	windowCnt = 0;
	for(int i = 0; i < 4; i++)
		windowAddress[i] = windowSize[i] = 0;
}

Window::~Window(){}

void Window::sortWindow()
{
	byte b_tmp = 0;
	int  n_tmp = 0;
	for(int i = 0; i < 4; i++)
		for(int j = i; j < 4; j++)
		{
			if(windowAddress[j] < windowAddress[i])  //冒泡排序法
			{
				b_tmp = windowAddress[j];
				n_tmp = windowSize[j];
				windowAddress[j] = windowAddress[i];
				windowSize[j] = windowSize[i];
				windowAddress[i] = b_tmp;
				windowSize[i] = n_tmp;
			}
			else if(windowAddress[j] == windowAddress[i])
			{
				if(windowSize[j] < windowSize[i])
				{
					b_tmp = windowAddress[j];
					n_tmp = windowSize[j];
					windowAddress[j] = windowAddress[i];
					windowSize[j] = windowSize[i];
					windowAddress[i] = b_tmp;
					windowSize[i] = n_tmp;
				}
			}
		}
}

bool Window::isInWindow(const byte adr) const
{
	if(adr < 0 || adr >= 8 )
		return false;
	for(int i = 0; i < 4; i++)
		if(adr == windowAddress[i])
			return true;
	return false;
}

bool Window::setWindow(const byte adr, const int sz)
{
	if(adr < 0 || adr >= 8 || sz <= 0 || sz > 8)		
		return false;
	if(windowCnt >= 4)   
		return false;
	/*
	完成插入新窗口值、排序和窗口碰撞检测任务
	*/
	Window win = *this;  //对当前调用对象备份
	windowAddress[0] = adr;
	windowSize[0]    = sz;
	sortWindow();
	if(detecCollision())  //新窗口碰撞
	{
		*this = win;  //用备份对象恢复当前调用对象
		return false;
	}
	windowCnt++;
	return true;
}

bool Window::clearWindow()
{
	windowCnt = 0;
	for(int i = 0; i < 4; i++)
		windowAddress[i] = windowSize[i] = 0;
	return true;
}

bool Window::clearWindow(const byte adr)
{
	if(adr < 0 || adr >= 8)		
		return false;
	for(int i = 0; i < 4; i++)
		if(windowAddress[i] == adr)
			windowAddress[i] = windowSize[i] = 0;
	windowCnt--;
	sortWindow();  //在该位置删除窗口后重新排序
	return true;
}

bool Window::detecCollision(void)
{
	if(windowAddress[3]+windowSize[3] > 8)
		return true;
	for(int i=0; i<4-1; i++)
		if(windowAddress[i]+windowSize[i] > windowAddress[i+1])
			return true;
	return false;
}

LedControl::LedControl(const int dataPin, const int clkPin, const int csPin) 
{
    SPI_MOSI=dataPin;
    SPI_CLK=clkPin;
    SPI_CS=csPin;
    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
	digitalWrite(SPI_CS,HIGH);
	SPI_MOSI=dataPin;
	for(int i=0;i<8;i++) 
		status[i] = spidata[i] = 0x00;
	clearDisplay();
}

void LedControl::clearDisplay()
{
	for(int i=0;i<8;i++) 
		spidata[i] = 0x00;
	spiTransfer(spidata);
}

void LedControl::spiTransfer(const byte data[])
{
	digitalWrite(SPI_CS,LOW);
	for(int i=8; i>0; i--)
		shiftOut(SPI_MOSI,SPI_CLK,LSBFIRST,~data[i-1]);  //针对共阳数码管 ~data[i-1]，Q0~Q7依次连接a,b,...,g,dp
	digitalWrite(SPI_CS,HIGH);
	//delay(500);
}

String LedControl::Float2Str(double number, unsigned int digits)
{
  String str;
 
  if (number > 4294967040.0) return String("0UF");  // constant determined empirically
  if (number <-4294967040.0) return String("OUF");  // constant determined empirically
  
  // Handle negative numbers
  if (number < 0.0)
  {
	str += String("-");
	number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (unsigned int i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  str += String(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    str += String("."); 
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    str += String(toPrint);
    remainder -= toPrint; 
  }
  
  return str;

}


byte LedControl::encoding(char ch)
{
	return codeTable[byte(ch)];
}

void LedControl::print(const String & str)
{
	for(int i = 0; str[i] != '\0' && i < 8; i++)
	{
		spidata[i] = encoding(str[i]);
	}
	spiTransfer(spidata);
}

void LedControl::print(int str)
{
	print(String(str));
}
void LedControl::print(unsigned int str)
{
	print(String(str));
}
void LedControl::print(long str)
{
	print(String(str));
}
void LedControl::print(unsigned long str)
{
	print(String(str));
}
void LedControl::print(char str)
{
	print(String(str));
}
void LedControl::print(unsigned char str)
{
	print(String(str));
}
void LedControl::print(double str, unsigned int digits)
{
	print(Float2Str(str,digits));
}

void LedControl::windowPrint(const Window & win, 
		const String &  str1, 
		const String &  str2, 
		const String &  str3, 
		const String &  str4 )
{
	if(!win.getWindowCount())	return ;
	String str[4] = {str1, str2, str3, str4};
	int cnt = win.getWindowCount();     //获取窗口数目
	int maxLen = 0;    //所有字符串的最大长度
	for(int i = 0; i < cnt; i++)
		if((int)str[i].length() > maxLen)
			maxLen = str[i].length();   //获取最大字符长度

	//初始化字符串长度小于窗口尺寸的情况
	for(int i = 0; i < cnt; i++)
	{
		byte winAddr = win.getWindowAdr(i+4-cnt);
		int  winSize = win.getWindowSize(i+4-cnt);
		if((int)str[i].length() <= winSize)
		{
			for(int j = 0 ; j < winSize ; j++,winAddr++)
			{
				spidata[winAddr] = encoding(str[i][j]);
			}
		}
	}
	//对字符串长度大于窗口尺寸的情况，向左滚动显示一次
	for(int step = 0; step <= maxLen; step++ )
	{
		//每移动一步，依次装载每个窗口，并发送
		for(int i = 0; i < cnt; i++)
		{
			byte winAddr = win.getWindowAdr(i+4-cnt);
			int  winSize = win.getWindowSize(i+4-cnt);
			if((int)str[i].length() <= winSize)	continue;  
			for(int j = 0 + step; j < winSize + step; j++,winAddr++)
			{
				spidata[winAddr] = encoding(str[i][j]);
			}
		}
		spiTransfer(spidata);
		delay(250); //每次发送间隙等待0.25s
	}
}


void LedControl::windowPrint(const Window & win,  int  str1 , 
			int  str2 ,
			int  str3 ,
			int  str4  )
{
	windowPrint(win, String(str1), String(str2), String(str3), String(str4));
}
void LedControl::windowPrint(const Window & win,  unsigned int  str1 , 
			unsigned int  str2 ,
			unsigned int  str3 ,
			unsigned int  str4   )
{
	windowPrint(win, String(str1), String(str2), String(str3), String(str4));
}
void LedControl::windowPrint(const Window & win,  long  str1 , 
			long  str2 ,
			long  str3 ,
			long  str4   )
{
	windowPrint(win, String(str1), String(str2), String(str3), String(str4));
}
void LedControl::windowPrint(const Window & win,  unsigned long  str1 , 
			unsigned long  str2 ,
			unsigned long  str3 ,
			unsigned long  str4   )
{
	windowPrint(win, String(str1), String(str2), String(str3), String(str4));
}
void LedControl::windowPrint(const Window & win,  char  str1 , 
			char  str2 ,
			char  str3 ,
			char  str4   )
{
	windowPrint(win, String(str1), String(str2), String(str3), String(str4));
}
void LedControl::windowPrint(const Window & win,  unsigned char  str1 , 
			unsigned char  str2 ,
			unsigned char  str3 ,
			unsigned char  str4   )
{
	windowPrint(win, String(str1), String(str2), String(str3), String(str4));
}
void LedControl::windowPrint(const Window & win,  double  str1 , 
			double  str2 ,
			double  str3 ,
			double  str4   )
{
	windowPrint(win, Float2Str(str1), Float2Str(str2), Float2Str(str3), Float2Str(str4));
}
