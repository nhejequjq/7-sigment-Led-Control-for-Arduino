#ifndef LedControl_h
#define LedControl_h

#include <Arduino.h>


const int TABLE_SIZE = 128; 
const static byte codeTable[TABLE_SIZE] = {
						0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
						0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
						0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
						0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
            0b00000000,0b01000001,0b11000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
						0b10001100,0b01110000,0b00000000,0b00000000,0b00000000,0b00000010,0b00000001,0b00000000,
            0b11111100,0b01100000,0b11011010,0b11110010,0b01100110,0b10110110,0b10111110,0b11100000,
            0b11111110,0b11110110,0b00000000,0b00000000,0b00000000,0b00010010,0b00000000,0b00000000,
						0b00000000,0b11101110,0b11111110,0b10011100,0b11111100,0b10011110,0b10001110,0b10111100,
						0b01101110,0b00001100,0b01110000,0b00000000,0b00011100,0b00000000,0b11101100,0b11111100,
						0b11001110,0b00000000,0b00000000,0b10110110,0b00000000,0b01111100,0b00000000,0b00000000,
						0b00000000,0b01110110,0b00000000,0b10001100,0b00000000,0b01110000,0b00000000,0b00010000,
						0b00000100,0b00000000,0b00111110,0b00011010,0b01111010,0b11011110,0b00000000,0b11110110,
						0b00101110,0b00001000,0b00110000,0b00000000,0b00011000,0b00000000,0b00101010,0b00111010,
						0b11001110,0b11100110,0b00001010,0b10110110,0b00011110,0b00111000,0b00111000,0b00000000,
						0b00000000,0b01110110,0b00000000,0b10001100,0b01100000,0b01110000,0b10000000,0b00000000
                        };
class Window
{
private:
	/*窗口地址数组，用来存储窗口的地址*/
	byte windowAddress[4];
	/*窗口尺寸数组，用来存储对应窗口地址的尺寸*/
	int  windowSize[4];
	/*窗口数目*/
	int  windowCnt;
	//byte winPosFlag;
	
	/*
	以窗口地址windowAddress为主序，窗口大小windowSize为次序排列窗口
	*/
	void sortWindow(); 

	/*
	 检测窗口碰撞函数
	 返回值：
	 bool: 窗口碰撞，true; 窗口不碰撞，false;
	*/
	bool detecCollision(void);  

public:
	/*
	Window构造函数，建立一个空Window 对象
	*/
	Window();

	/*
	Window析构函数，释放Window对象
	*/
	virtual ~Window();

	/*
	setWindow()函数用来对SPI LED 模块 添加窗口，窗口可用作动态显示；
	返回值：
	bool: 窗口添加成功，true; 添加失败，false;
	参数：
	const byte adr: SPI LED 模块显示数码管的地址；(0<=adr<8)
	const int sz:  新添加窗口的尺寸；(0<sz<=8)
	*/
	bool setWindow(const byte adr, const int sz);

	/*
	isInWindow()函数用来检测该地址的窗口是否存在；
	返回值：
	bool: 窗口存在，true; 窗口不存在，false;
	参数：
	const byte adr: 窗口的地址；(0<=adr<8)
	*/
	bool isInWindow(const byte adr) const;

	/*
	getWindowCount()函数用来获取SPI LED 模块 当前窗口数目；
	返回值：
	int: 窗口数目大小；
	*/
	int  getWindowCount() const{  return windowCnt;  }

	/*
	clearWindow()函数用来清除指定窗口或者清楚所有窗口；
	返回值：
	bool: 清除成功,true; 清除失败,false;
	参数：
	const byte adr: 清除指定窗口地址的窗口；（无参数时清除所有窗口）
	*/
	bool clearWindow();
	bool clearWindow(const byte adr);

	/*
	getWindowAdr()函数用来获取窗口的地址；
	返回值：
	byte: 窗口地址值;
	参数：
	int i: 在地址数组里的序号
	*/
	byte getWindowAdr(int i) const 
	{
		if(i < 0 || i >= 4)		return 0;
		return windowAddress[i]; 
	}

	/*
	getWindowSize()函数用来获取窗口的尺寸；
	返回值：
	int: 窗口尺寸大小;
	参数：
	int i: 在尺寸数组里的序号
	*/
	int  getWindowSize(int i) const 
	{
		if(i < 0 || i >= 4)		return 0;
		return windowSize[i]; 
	}

};


class LedControl {
    private :
    /* 代表SPI LED Module 模块8个数码管状态的byte数组 */
    byte status[8];
    /* 传送给SPI LED Module 模块的8个数码管状态值的byte数组 */
    byte spidata[8];
    /* Arduino 传递SPI数据的Pin脚值*/
    int SPI_MOSI;
    /* Arduino 传递SPI时钟信号的Pin脚值*/
    int SPI_CLK;
    /* Arduino 传递SPI写入、锁存时钟信号的Pin脚值*/
    int SPI_CS;

	/*
	spiTransfer()函数用来向SPI LED Module 传递SPI 串行数据；
	返回值：
	void;
	参数：
	const byte data[]: spidata[8]数组;
	*/
    void spiTransfer(const byte data[]);

	/*
	encoding()函数用来将字符ch 译码为7段LED 数码管的byte 型值；
	返回值：
	byte: 将字符ch 译码为7段数码管中的byte 型表示信号；
	参数：
	char ch: 需要译码的ASCII码表中字符;
	*/
	byte encoding(char ch);

	/*
	Float2Str()函数用来将double 型数值转化为字符串；
	返回值：
	String: 转换后的字符串；
	参数：
	double num: 需要转换的double 型值;
	unsigned int digits: 小数精确位数;
	*/
	String Float2Str(double num, unsigned int digits = 2);  //定义从浮点数向String 字符串转换的函数,默认保留两位小数

    public:
    /*
	LedControl()函数用来对LedControl 型对象进行基本参数设置；
	参数：
	const int dataPin: SPI数据传送PIN;
	const int clkPin: 时钟信号PIN;
	const int csPin: SPI写入、锁存时钟信号PIN;
	*/
    LedControl(const int dataPin, const int clkPin, const int csPin); 

	/*
	~LedControl()函数用来析构LedControl类的对象；
	*/
	virtual ~LedControl(){};

    /* 
     clearDisplay()函数用来清除SPI LED Module 屏幕；
	返回值:
	void;
     */
    void clearDisplay();
	
	/* 
     print()函数实现SPI LED Module 屏幕上静态显示字符；
	返回值:
	void;
	参数:
	可以接收const String & str， (unsigned) int ， (unsigned) long ， (unsigned) char ， double 参数
	void print(double str = 0.0, unsigned int digits = 2) 中， unsigned int digits表示精确小数位数；
     */
	void print(const String & str = "");
	void print(int str = 0);
	void print(unsigned int str = 0);
	void print(long str = 0);
	void print(unsigned long str = 0);
	void print(char str = 0);
	void print(unsigned char str = 0);
	void print(double str = 0.0, unsigned int digits = 2);

	/* 
     windowPrint()函数实现SPI LED Module 屏幕上指定的多窗口位置同时显示与窗口对应的字符；
	 当字符长度大于对应的窗口尺寸则在对应窗口动态滚动显示，当字符长度小于或等于窗口尺寸则在对应窗口静态显示；
	返回值:
	void;
	参数:
	可以接收const String & str， (unsigned) int ， (unsigned) long ， (unsigned) char ， double 参数；
	void print(double str = 0.0, unsigned int digits = 2) 中， unsigned int digits表示精确小数位数；
     */
	void windowPrint(const Window & win,  const String &  str1 = "", 
										  const String &  str2 = "",
										  const String &  str3 = "",
										  const String &  str4 = ""  );
	void windowPrint(const Window & win,  int  str1 = 0, 
										  int  str2 = 0,
										  int  str3 = 0,
										  int  str4 = 0  );
	void windowPrint(const Window & win,  unsigned int  str1 = 0, 
										  unsigned int  str2 = 0,
										  unsigned int  str3 = 0,
										  unsigned int  str4 = 0  );
	void windowPrint(const Window & win,  long  str1 = 0, 
										  long  str2 = 0,
										  long  str3 = 0,
										  long  str4 = 0  );
	void windowPrint(const Window & win,  unsigned long  str1 = 0, 
										  unsigned long  str2 = 0,
										  unsigned long  str3 = 0,
										  unsigned long  str4 = 0  );
	void windowPrint(const Window & win,  char  str1 = ' ', 
										  char  str2 = ' ',
										  char  str3 = ' ',
										  char  str4 = ' '  );
	void windowPrint(const Window & win,  unsigned char  str1 = 0, 
										  unsigned char  str2 = 0,
										  unsigned char  str3 = 0,
										  unsigned char  str4 = 0  );
	void windowPrint(const Window & win,  double  str1 = 0.0, 
										  double  str2 = 0.0,
										  double  str3 = 0.0,
										  double  str4 = 0.0  );
	
};








#endif
