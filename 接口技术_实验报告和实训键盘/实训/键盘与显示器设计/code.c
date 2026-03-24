#include <stdlib.h>
#define MAX_DISPLAY 8
char displayBuff[MAX_DISPLAY] = {0}; // 全局显示缓冲区
int displayIndex = 0; // 全局显示索引

// 第1行显示的汉字字模（示例数据，需要根据实际显示内容调整）
unsigned char Line1_1[] = {  // 示例：第一个字的字模
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00,
	0x22, 0x00, 0x02, 0x00, 0x3E, 0x00, 0x22, 0x00,
	0x22, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char Line1_2[] = {  // 示例：第二个字的字模
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

unsigned char Line1_3[] = {  // 示例：第三个字的字模
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

unsigned char Line1_4[] = {  // 示例：第四个字的字模
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// 第3行显示的汉字字模
unsigned char Line3_1[] = {  // 示例："欢" 字
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

unsigned char Line3_2[] = {  // 示例："迎" 字
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

unsigned char Line3_3[] = {  // 示例："使" 字
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

unsigned char Line3_4[] = {  // 示例："用" 字
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

//8255
#define PA_Addr		0x270
#define PB_Addr		0x271
#define PC_Addr		0x272
#define	CON_Addr	0x273

#define WR_COM_AD_L		0x264				//写左半屏指令地址
#define WR_COM_AD_R		0x260				//写右半屏指令地址
#define WR_DATA_AD_L 	0x266				//写左半屏数据地址	
#define WR_DATA_AD_R	0x262				//写右半屏数据地址
#define RD_BUSY_AD 		0x261				//查忙地址
#define RD_DATA_AD 		0x263				//读数据地址

#define X 			0xB8							//起始显示行基址
#define Y			0x40							//起始显示列基址
#define FirstLine	0xC0							//起始显示行


//16*16LED
extern void outportb( unsigned int, char);
extern char inportb( unsigned int );
extern void outportw( unsigned int, int);

#define u8	unsigned char
#define u16	unsigned int

#define A8255_PA 	0x270
#define A8255_PB 	0x271
#define A8255_CTL 	0x273

#define A273		0x230

#define LINE  A273	//列线
#define ROW1  A8255_PA	//行线1
#define ROW2  A8255_PB	//行线2

//8255按键状态判断
u8 keyall[516]={0};
u8* charTab16 = NULL; // 初始为空指针
size_t charTabSize = 0; // 已分配的数组大小
void addToCharTab(u8* newData, size_t newSize) {
    size_t newSizeTab = charTabSize + newSize;
    u8* newCharTab = realloc(charTab16, newSizeTab);
    if (newCharTab == NULL) {
        // 内存分配失败处理
        return;
    }
    charTab16 = newCharTab;
    memcpy(charTab16 + charTabSize, newData, newSize);
    charTabSize += newSize;
}
void deleteCharTab(size_t newSize) {
    size_t newSizeTab = charTabSize - newSize;
    u8* newCharTab = realloc(charTab16, newSizeTab);
    if (newCharTab == NULL) {
        // 内存分配失败处理
        return;
    }
    charTab16 = newCharTab;

    charTabSize -= newSize;
}
//大写左边，右边补0
u8 Line_16A[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x08,0x00,0x18,0x00,0x14,0x00,0x14,0x00,
0x24,0x00,0x3C,0x00,0x22,0x00,0x42,0x00,0x42,0x00,0xE7,0x00,0x00,0x00,0x00,0x00
};

u8 Line_16B[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x22,0x00,0x22,0x00,0x22,0x00,0x1E,0x00,
0x22,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0x22,0x00,0x1F,0x00,0x00,0x00,0x00,0x00
};
u8 Line_16C[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x00,0x42,0x00,0x42,0x00,0x01,0x00,0x01,0x00,
0x01,0x00,0x01,0x00,0x01,0x00,0x42,0x00,0x22,0x00,0x1C,0x00,0x00,0x00,0x00,0x00
};

u8 Line_16D[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x22,0x00,0x42,0x00,0x42,0x00,0x42,0x00,
0x42,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0x22,0x00,0x1F,0x00,0x00,0x00,0x00,0x00};

u8 Line_16E[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x42,0x00,0x12,0x00,0x12,0x00,0x1E,0x00,
0x12,0x00,0x12,0x00,0x02,0x00,0x42,0x00,0x42,0x00,0x3F,0x00,0x00,0x00,0x00,0x00};

u8 Line_16F[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x42,0x00,0x12,0x00,0x12,0x00,0x1E,0x00,
0x12,0x00,0x12,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x07,0x00,0x00,0x00,0x00,0x00};

u8 Line_16G[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,0x22,0x00,0x22,0x00,0x01,0x00,0x01,0x00,
0x01,0x00,0x71,0x00,0x21,0x00,0x22,0x00,0x22,0x00,0x1C,0x00,0x00,0x00,0x00,0x00};

u8 Line_16H[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0x42,0x00,
0x7E,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0xE7,0x00,0x00,0x00,0x00,0x00};
//小写左边，右边补0
u8 Line_16a1[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,
0x22,0x00,0x30,0x00,0x2C,0x00,0x22,0x00,0x32,0x00,0x6C,0x00,0x00,0x00,0x00,0x00
};

u8 Line_16b1[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x02,0x00,0x02,0x00,0x1A,0x00,
0x26,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0x26,0x00,0x1A,0x00,0x00,0x00,0x00,0x00
};
u8 Line_16c1[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,
0x44,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x44,0x00,0x38,0x00,0x00,0x00,0x00,0x00
};

u8 Line_16d1[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x40,0x00,0x40,0x00,0x7C,0x00,
0x42,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0x62,0x00,0xDC,0x00,0x00,0x00,0x00,0x00};

u8 Line_16e1[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,
0x42,0x00,0x42,0x00,0x7E,0x00,0x02,0x00,0x42,0x00,0x3C,0x00,0x00,0x00,0x00,0x00};

u8 Line_16f1[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x48,0x00,0x08,0x00,0x3E,0x00,
0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x3E,0x00,0x00,0x00,0x00,0x00};

u8 Line_16g1[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x00,
0x22,0x00,0x22,0x00,0x1C,0x00,0x02,0x00,0x3C,0x00,0x42,0x00,0x42,0x00,0x3C,0x00};

u8 Line_16h1[] = {		
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x02,0x00,0x02,0x00,0x3A,0x00,
0x46,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0xE7,0x00,0x00,0x00,0x00,0x00};
//小写左边，右边补0


void AddToDisplay(char c) {
    if (displayIndex < MAX_DISPLAY - 1) {
        displayBuff[displayIndex++] = c; // 添加字符到缓冲区
        displayBuff[displayIndex] = '\0'; // 确保字符串以NULL结尾
    }
}
//延时1ms
void DL1ms()
{
	u16 i;
	i = 100;
	while (--i)
	{;}
}

void DL500ms()
{
	u16 i = 500;
	
	while(i--)
		DL1ms();
}

	
//8255、8155初始化	
void initIO()
{
	outportb(A8255_CTL, 0x80);				//设置8255的PA、PB、PC口为输出口
}

void clear()
{
	outportw(LINE, 0xffff);
	outportb(ROW1, 0x0);
	outportb(ROW2, 0x0);
}

//测试LED子程序,点亮LED并延时1S	
void Test_LED()
{
	outportw(LINE, 0x0);
	outportb(ROW1, 0xff);
	outportb(ROW2, 0xff);
	DL500ms();
	DL500ms();
}

//显示一个16*16点阵字
void Display1(u8* pBuffer)
{
	u8 i;
	u16 j;
	j = 0xfffe;						//16行的哪一行
	while(j != 0xffff)				//16列依次被扫描
	{
		outportw(LINE, j);			//16行输出
		outportb(ROW1, *pBuffer++);	//左边列输出
		outportb(ROW2, *pBuffer++);	//右边列输出
		DL1ms();
		clear();
		j = (j<<1) | 1;				//准备扫描下一行
	}
}

//显示一个16*16点阵字,字型码放在pBuffer指出的存贮器中,显示时间为20*8ms
void DISP_CH(u8* pBuffer)
{
	u8 i = 8;
	while (i--)
		Display1(pBuffer);
}

//"abcdefgh"
unsigned char Line2_a[] = {
		0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x19,0x24,0x24,0x12,0x3F,0x20,0x00};
unsigned char Line2_b[] = {
		0x10,0xF0,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00};
unsigned char Line2_c[] = {
		0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00};
unsigned char Line2_d[] = {
		0x00,0x00,0x80,0x80,0x80,0x90,0xF0,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20};
unsigned char Line2_e[] = {
		0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x24,0x24,0x24,0x24,0x17,0x00};
unsigned char Line2_f[] = {
		0x00,0x80,0x80,0xE0,0x90,0x90,0x20,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00};
unsigned char Line2_g[] = {
		0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00};
unsigned char Line2_h[] = {
		0x10,0xF0,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20};
//"ABCDEFGH"
unsigned char Line2_AA[] = {
		0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20};
unsigned char Line2_BB[] = {
		0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00};
unsigned char Line2_CC[] = {
		0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00};
unsigned char Line2_DD[] = {
		0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00};
unsigned char Line2_EE[] = {
		0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00};
unsigned char Line2_FF[] = {
		0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00};
unsigned char Line2_GG[] = {
		0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00};
unsigned char Line2_HH[] = {
		0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20};
	
//***************************************
//基本控制		
//***************************************					
//写左半屏控制指令
void WRComL(unsigned char _data)
{
	outportb(WR_COM_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//写右半屏控制指令
void WRComR(unsigned char _data)
{
	outportb(WR_COM_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//写左半屏数据
void WRDataL(unsigned char _data)
{
	outportb(WR_DATA_AD_L, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//写右半屏数据
void WRDataR(unsigned char _data)
{
	outportb(WR_DATA_AD_R, _data);
	while (inportb(RD_BUSY_AD) & 0x80)	//检查液晶显示是否处于忙状态
	{;}
}

//显示左半屏数据，count-显示数据个数
void DisplayL(unsigned char *pt, char count)
{
	while (count--)
	{
		WRDataL(*pt++);			//写左半屏数据
	}
}

//显示右半屏数据，count-显示数据个数
void DisplayR(unsigned char * pt, char count)
{
	while (count--)
	{
		WRDataR(*pt++);			//写右半屏数据
	}
}

//设置左半屏起始显示行列地址,x-X起始行序数(0-7)，y-Y起始列序数(0-63)
void SETXYL(unsigned char x, unsigned char y)
{
	WRComL(x+X);				//行地址=行序数+行基址
	WRComL(y+Y);				//列地址=列序数+列基址
}

//设置右半屏起始显示行列地址,x:X起始行序数(0-7)，y:Y起始列序数(0-63)
void SETXYR(unsigned char x, unsigned char y)
{
	WRComR(x+X);				//行地址=行序数+行基址
	WRComR(y+Y);				//列地址=列序数+列基址
}

//***************************************
//显示图形		
//***************************************
//显示左半屏一行图形,A-X起始行序数(0-7)，B-Y起始列地址序数(0-63)
void LineDisL(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYL(x,y);				//设置起始显示行列
	DisplayL(pt, 64);			//显示数据
}

//显示右半屏一行图形,A-X起始行地址序数(0-7)，B-Y起始列地址序数(0-63)
void LineDisR(unsigned char x, unsigned char y, unsigned char * pt)
{
	SETXYR(x,y);				//设置起始显示行列
	DisplayR(pt, 64);			//显示数据
}

//***************************************
//显示字体，显示一个数据要占用X行两行位置
//***************************************
//右半屏显示一个字节/字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void ByteDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//设置起始显示行列地址
	DisplayR(pt, 8);			//显示上半行数据
	SETXYR(x+1,y);			//设置起始显示行列地址
	DisplayR(pt+8, 8);		//显示下半行数据
}

void WordDisR(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYR(x,y);			//设置起始显示行列地址
	DisplayR(pt, 16);		//显示上半行数据
	SETXYR(x+1,y);			//设置起始显示行列地址
	DisplayR(pt+16, 16);	//显示下半行数据
}

//左半屏显示一个字节/字：x-起始显示行序数X(0-7)；y-起始显示列序数Y(0-63)；pt-显示字数据首地址
void ByteDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//设置起始显示行列地址
	DisplayL(pt, 8);		//显示上半行数据
	SETXYL(x+1,y);			//设置起始显示行列地址
	DisplayL(pt+8, 8);		//显示下半行数据
}

void WordDisL(unsigned char x, unsigned char y,unsigned char * pt)
{
	SETXYL(x,y);			//设置起始显示行列地址
	DisplayL(pt, 16);		//显示上半行数据
	SETXYL(x+1,y);			//设置起始显示行列地址
	DisplayL(pt+16, 16);	//显示下半行数据
}
//清屏
void LCDClear()
{
//清左半屏
	unsigned char x,y;
	char j;
	x = 0;						//起始行，第0行
	y = 0;						//起始列，第0列
	for (x = 0; x < 8; x++)		//共8行
	{
		SETXYL(x,y);			//设置起始显示行列地址
		j = 64;
		while (j--)
			WRDataL(0);
	}
//清右半屏
	x = 0;						//起始行，第0行
	y = 0;						//起始列，第0列
	for (x = 0; x < 8; x++)		//共8行
	{
		SETXYR(x,y);			//设置起始显示行列地址
		j = 64;
		while (j--)
			WRDataR(0);
	}
}

//液晶初始化	
void LCD_INIT()
{
	WRComL(0x3e);			//初始化左半屏，关显示
	WRComL(FirstLine);		//设置起始显示行，第0行
	WRComR(0x3e);			//初始化右半屏，关显示
	WRComR(FirstLine);		//设置起始显示行，第0行
	LCDClear();				//清屏
	WRComL(0x3f);			//开显示
	WRComR(0x3f);			//开显示
}

//第3行显示"STAR ES51PRO"
void DisLine2a()
{
			//第4行,第16列，左半屏，显示一个字节子程序
	ByteDisL(4,32,Line2_a);

}
void DisLine2b()
{
			//第4行,第16列，左半屏，显示一个字节子程序

	ByteDisL(4,40,Line2_b);


}
void DisLine2c()
{

	ByteDisL(4,48,Line2_c);


}
void DisLine2d()
{

	ByteDisL(4,56,Line2_d);
	


}
void DisLine2e()
{

	
	ByteDisR(4,0,Line2_e);			//右半屏字节显示数据


}
void DisLine2f()
{
			//右半屏字节显示数据
	ByteDisR(4,8,Line2_f);


}
void DisLine2g()
{

	ByteDisR(4,16,Line2_g);


}
void DisLine2h()
{

	ByteDisR(4,24,Line2_h);

}
void DisLine2()
{
			//第4行,第16列，左半屏，显示一个字节子程序
	ByteDisL(4,32,Line2_a);
	ByteDisL(4,40,Line2_b);
	ByteDisL(4,48,Line2_c);
	ByteDisL(4,56,Line2_d);
	
	ByteDisR(4,0,Line2_e);			//右半屏字节显示数据
	ByteDisR(4,8,Line2_f);
	ByteDisR(4,16,Line2_g);
	ByteDisR(4,24,Line2_h);

}
//第2行显示"星研电子"
void DisLine1()
{
	WordDisL(2,32,Line1_1);		//第2行,第32列，左半屏，显示一个字子程序
	WordDisL(2,48,Line1_2);
	WordDisR(2,0,Line1_3);		//右半屏，显示一个字子程序
	WordDisR(2,16,Line1_4);
}

//第4行显示"欢迎使用"
void DisLine3()
{
	WordDisL(6,32,Line3_1);		//第6行, 第32列，左半屏，显示一个字子程序
	WordDisL(6,48,Line3_2);		//第6行, 第48列
	WordDisR(6,0,Line3_3);		//右半屏，显示一个字子程序
	WordDisR(6,16,Line3_4);
}

//延时程序
void DelayTime()
{
	unsigned char i;
	unsigned int j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 0x0fff; j++)
		{;}
	}
}

u8 ScanKey2()
{
	u8 keyResult;
	u8 i = inportb(PC_Addr);
	switch(i)
	{
	case 0xfe:
		keyResult = 0;
		break;
	case 0xfd:
		keyResult = 1;
		break;
	case 0xfb:
		keyResult = 2;
		break;
	case 0xf7:
		keyResult = 3;
		break;
	case 0xef:
		keyResult = 4;
		break;
	case 0xdf:
		keyResult = 5;
		break;
	case 0xbf:
		keyResult = 6;
		break;
	case 0x7f:
		keyResult = 7;
		break;
	default:
		keyResult = 0xff;//
	}
	return keyResult;
}
void Delay20ms()
{
	u16 i = 1320;
	while (--i)
	{;}
}

u8 ScanKey()
{
	u8 i;
	u8 KeyResult;
	while (1)
	{
		KeyResult = inportb(PC_Addr);			//检测按键输入
		if (KeyResult == 0xff)					// ==0xff, 无按键按下
			break;
		KeyResult = ScanKey2();
		if (KeyResult == 0xff)
			continue;							//按了不只一个键
		Delay20ms();						//有按键，取消抖动处理
		Delay20ms();
		if (KeyResult == ScanKey2())
		{
			do
			{
				i = inportb(PC_Addr);
			}while (i != 0xff);
			break;
		}
	}
	return KeyResult;
}
int count=0;//输入字符数量
int line=24,lineflag=24;//列数
int row[128]={3};//行数
int shift[128]={0};//大小写转换
int flagRL[128]={0};//左右半屏显示标志,0左，1右
void Displaybu(){

	int charth=0;
	

    
	while(charth<count){
	if(charth!=0&&charth==16){
		row[charth]=(row[charth]+2)%7;	

	}
	else{
	row[charth+1]=row[charth];
	}
		switch (keyall[charth])
		{
			case 0:
			
			if(line==64){
				
				line=0;
				
				flagRL[charth+1]=(flagRL[charth]+1)%2;
				
				if(shift[charth]==1){
				
				ByteDisR(row[charth],line, Line2_AA);
				
				line+=8;
				}
				else{
				ByteDisR(row[charth],line, Line2_a);
				line+=8;
				}
				}
			else{
				flagRL[charth+1]=flagRL[charth];
				
				if(flagRL[charth]==0){
				if(shift[charth]==1){
                ByteDisL(row[charth], line, Line2_AA);
                line+=8;
            }
            
            	else{
            	ByteDisL(row[charth], line, Line2_a);
            	line+=8;
        	}
  			
            }
           		else{
            	if(shift[charth]==1){
            	 ByteDisR(row[charth], line, Line2_AA);
            	 line+=8;
        	 	}
        	 	
        	 	else{
        	 	 ByteDisR(row[charth], line, Line2_a);
    	 		}

        	}
            }
                break;
            case 1:
             if(line==64){
				line=0;
				flagRL[charth+1]=(flagRL[charth]+1)%2;
				if(shift[charth]==0){
				ByteDisR(row[charth],line, Line2_b);
				line+=8;
				}
				else{
				ByteDisR(row[charth],line, Line2_BB);
				line+=8;
				}
				}
			else{
				flagRL[charth+1]=flagRL[charth];
				
				if(flagRL[charth]==0){
				if(shift[charth]==0){
                ByteDisL(row[charth], line, Line2_b);
                line+=8;
            }
            else{
            	ByteDisL(row[charth], line, Line2_BB);
            	line+=8;
        	}
    
            }
            else{
            	if(shift[charth]==0){
            	 ByteDisR(row[charth], line, Line2_b);
            	 line+=8;
        	 	}
        	 	else{
        	 	 ByteDisR(row[charth], line, Line2_BB);
        	 	 line+=8;
    	 		}

        	}
            }
                break;
            case 2:
               	if(line==64){
				line=0;
				flagRL[charth+1]=(flagRL[charth]+1)%2;
				if(shift[charth]==0){
				ByteDisR(row[charth],line, Line2_c);
				line+=8;
				}
				else{
				ByteDisR(row[charth],line, Line2_CC);
				line+=8;
				}
				}
			else{
				flagRL[charth+1]=flagRL[charth];
				
				if(flagRL[charth]==0){
				if(shift[charth]==0){
                ByteDisL(row[charth], line, Line2_c);
                line+=8;
            }
            else{
            	ByteDisL(row[charth], line, Line2_CC);
            	line+=8;
        	}

            }
            else{
            	if(shift[charth]==0){
            	 ByteDisR(row[charth], line, Line2_c);
            	 line+=8;
        	 	}
        	 	else{
        	 	 ByteDisR(row[charth], line, Line2_CC);
        	 	 line+=8;
    	 		}

        	}
            }
                break;
            case 3:
			if(line==64){
				line=0;
				flagRL[charth+1]=(flagRL[charth]+1)%2;
				if(shift[charth]==0){
				ByteDisR(row[charth],line, Line2_d);
				line+=8;
				}
				else{
				ByteDisR(row[charth],line, Line2_DD);
				line+=8;
				}
				}
			else{
				flagRL[charth+1]=flagRL[charth];
				
				if(flagRL[charth]==0){
				if(shift[charth]==0){
                ByteDisL(row[charth], line, Line2_d);
                line+=8;
            }
            else{
            	ByteDisL(row[charth], line, Line2_DD);
            	line+=8;
        	}

            }
            else{
            	if(shift[charth]==0){
            	 ByteDisR(row[charth], line, Line2_d);
            	 line+=8;
        	 	}
        	 	else{
        	 	 ByteDisR(row[charth], line, Line2_DD);
        	 	 line+=8;
    	 		}

        	}
            }
                break;
            case 4:
               			if(line==64){
				line=0;
				flagRL[charth+1]=(flagRL[charth]+1)%2;
				if(shift[charth]==0){
				ByteDisR(row[charth],line, Line2_e);
				line+=8;
				}
				else{
				ByteDisR(row[charth],line, Line2_EE);
				line+=8;
				}
				}
			else{
				
				flagRL[charth+1]=flagRL[charth];
				if(flagRL[charth]==0){
				if(shift[charth]==0){
                ByteDisL(row[charth], line, Line2_e);
                line+=8;
            }
            else{
            	ByteDisL(row[charth], line, Line2_EE);
            	line+=8;
        	}

            }
            else{
            	if(shift[charth]==0){
            	 ByteDisR(row[charth], line, Line2_e);
            	 line+=8;
        	 	}
        	 	else{
        	 	 ByteDisR(row[charth], line, Line2_EE);
        	 	 line+=8;
    	 		}

        	}
            }
                break;
            case 5:
             			if(line==64){
				line=0;
				flagRL[charth+1]=(flagRL[charth]+1)%2;
				if(shift[charth]==0){
				ByteDisR(row[charth],line, Line2_f);
				line+=8;
				}
				else{
				ByteDisR(row[charth],line, Line2_FF);
				line+=8;
				}
				}
			else{
				flagRL[charth+1]=flagRL[charth];
				
				if(flagRL[charth]==0){
				if(shift[charth]==0){
                ByteDisL(row[charth], line, Line2_f);
                line+=8;
            }
            else{
            	ByteDisL(row[charth], line, Line2_FF);
            	line+=8;
        	}

            }
            else{
            	if(shift[charth]==0){
            	 ByteDisR(row[charth], line, Line2_f);
            	 line+=8;
        	 	}
        	 	else{
        	 	 ByteDisR(row[charth], line, Line2_FF);
        	 	 line+=8;
    	 		}

        	}
            }
                break;
            case 6:
					if(line==64){
				line=0;
				flagRL[charth+1]=(flagRL[charth]+1)%2;
				if(shift[charth]==0){
				ByteDisR(row[charth],line, Line2_g);
				line+=8;
				}
				else{
				ByteDisR(row[charth],line, Line2_GG);
				line+=8;
				}
				}
			else{
				
				flagRL[charth+1]=flagRL[charth];
				if(flagRL[charth]==0){
				if(shift[charth]==0){
                ByteDisL(row[charth], line, Line2_g);
                line+=8;
            }
            else{
            	ByteDisL(row[charth], line, Line2_GG);
            	line+=8;
        	}

            }
            else{
            	if(shift[charth]==0){
            	 ByteDisR(row[charth], line, Line2_g);
            	 line+=8;
        	 	}
        	 	else{
        	 	 ByteDisR(row[charth], line, Line2_GG);
        	 	 line+=8;
    	 		}

        	}
            }
                break;
            case 7:
				if(line==64){
				line=0;
				
				flagRL[charth+1]=(flagRL[charth]+1)%2;
				if(shift[charth]==0){
				ByteDisR(row[charth],line, Line2_h);
				line+=8;
				}
				else{
				ByteDisR(row[charth],line, Line2_HH);
				line+=8;
				}
				}
			else{
				
				flagRL[charth+1]=flagRL[charth];
				if(flagRL[charth]==0){
				if(shift[charth]==0){
                ByteDisL(row[charth], line, Line2_h);
                line+=8;
            }
            else{
            	ByteDisL(row[charth], line, Line2_HH);
            	line+=8;
        	}

            }
            else{
            	if(shift[charth]==0){
            	 ByteDisR(row[charth], line, Line2_h);
            	 line+=8;
        	 	}
        	 	else{
        	 	 ByteDisR(row[charth], line, Line2_HH);
        	 	 line+=8;
    	 		}

        	}
            }
                break;
            default:

			break;
		}
		charth++;
	}
}

u8* pBuffer;
typedef enum{
	mode_lowercase=0,
	mode_uppercase,	
	mode_function
}LCDMode;
static LCDMode currentMode = mode_lowercase;
static u8 lastKey=0xff;
void display1616(){
	u8 i,j;
	

    		//按键读入设置到PC口

		i = count-1;				//显示9个字符
		pBuffer = charTab16;
		while (i--)
		{
			j = 16;			//移动16次
			while (j--)
			{
				DISP_CH(pBuffer);	//显示一桢
				pBuffer += 2;
			}
		}
	
}
int jj,kk;
int main()
{
	u8 keyResult;

    outportb(CON_Addr, 0x89);
	Test_LED();				//调用测试子程序,测试LED是否全亮
	clear();
    
    

    



	while (1)
	{
		    line=24;
		
			keyResult = ScanKey();			//扫描按键
			
			if(keyResult==0xff){continue;}
			
			
			if(keyResult == 7 && lastKey == 7){
				currentMode = (currentMode+1)%3;
				keyall[--count]=0xff;
				lastKey = 0xff;
				deleteCharTab(sizeof(Line_16A));
				continue;	
			}
			
			//模式选择
			switch(currentMode){
			case mode_lowercase:
			keyall[count++]=keyResult;
			switch(keyResult){
				case 0:addToCharTab(Line_16a1, sizeof(Line_16A));break;
				case 1:addToCharTab(Line_16b1, sizeof(Line_16A));break;
				case 2:addToCharTab(Line_16c1, sizeof(Line_16A));break;
				case 3:addToCharTab(Line_16d1, sizeof(Line_16A));break;
				case 4:addToCharTab(Line_16e1, sizeof(Line_16A));break;
				case 5:addToCharTab(Line_16f1, sizeof(Line_16A));break;
				
				case 6:addToCharTab(Line_16g1, sizeof(Line_16A));break;
				case 7:addToCharTab(Line_16h1, sizeof(Line_16A));break;
				default: break;
			}
			
			shift[count-1]=0;
			break;
			case mode_uppercase:
			switch(keyResult){
				case 0:addToCharTab(Line_16A, sizeof(Line_16A));break;
				case 1:addToCharTab(Line_16B, sizeof(Line_16A));break;
				case 2:addToCharTab(Line_16C, sizeof(Line_16A));break;
				case 3:addToCharTab(Line_16D, sizeof(Line_16A));break;
				case 4:addToCharTab(Line_16E, sizeof(Line_16A));break;
				case 5:addToCharTab(Line_16F, sizeof(Line_16A));break;
				
				case 6:addToCharTab(Line_16G, sizeof(Line_16A));break;
				case 7:addToCharTab(Line_16H, sizeof(Line_16A));break;
				default: break;
			}

			keyall[count++]=keyResult;
			shift[count-1]=1;
			break;
			
			case mode_function:
			switch(keyResult){
				case 0:
					line=lineflag;			//向上移动
				for(jj=0;jj<count;jj++){
						
					if(row[jj]==0)row[jj]=6;
					else {row[jj]=row[jj]-1;}
				}
				break;
				case 1:
			//向下移动
				for(kk=0;kk<count;kk++){
						row[kk]=(row[kk]+1)%7;
				}
				line=lineflag;
				break;
				case 2:
					line=(lineflag-8)%65;			//左移
					lineflag=(lineflag-8)%65;
				break;
				case 3:line=(lineflag+8)%65;			//右移
					lineflag=(lineflag+8)%65;
				break;
				case 4:keyall[--count]=0xff;		//删除
					line=lineflag;
				break;
				case 5:display1616();			
					line=lineflag;				//16*16点阵显示
				break;
				
				case 6:line=lineflag;break;
				case 7:line=lineflag;break;//功能暂定
				default: break;
				
			}
		
			break;
			}
			lastKey=keyResult;
			
			LCD_INIT();					//液晶初始化


			Displaybu();
          	DelayTime();
   


	}
	
	
}


