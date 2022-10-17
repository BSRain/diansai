#include "MSP430F5529.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void senfchar(char s);
                         // Initialize Flash pointer
int abc(int n);
unsigned char u8_count_10ms;
//设置系统时钟
volatile unsigned int i;
#define SCK_L       P4OUT   &= ~BIT0
#define SCK_H       P4OUT   |= BIT0
#define SDA_IN      (P4IN & BIT1)
#define GapValue    119.7    /* 读取出的HX711为一个寄存器数值单位并不是g，除以这个GapValue值就是换算成g单位 */
unsigned long Weight_Maopi; /* 毛皮重，上电后就读取出这个变量 */
void waming ();        
void stopwaming ();
void TestWeight ();
char * Flash_ptr;    
unsigned long HX711_Buffer=0;
unsigned long maopia=0;
unsigned int Weight_Shiwu=0;
int w=0,aaa=0,ii=0;
unsigned int e=0;
//测量重量相关
void testnum();
#define rw_out P2DIR|=BIT0
#define rw_h P2OUT|=BIT0
#define rw_l P2OUT&=~BIT0
#define en_out P2DIR|=BIT2
#define en_h P2OUT|=BIT2
#define en_l P2OUT&=~BIT2
#define uchar unsigned char
#define uint unsigned int
void PrintFloatv(float num);
void transmission();
float m=0;
float n=0,ss=0;
uint yy=0;
int xx=0;
unsigned long get_weight(void); 
char a[100];
uchar juzhen[]={'0','\0'};
uchar zhongliang[]={'0','0','0','0','\0'};
uchar jiage[7]= {'0','0','0','.','0','0','\0'};
uchar he[7]= {'0','0','0','.','0','0','\0'};
char pp='0';
static uchar danjia[7]={'0','0','0','0','0','0','\0'};
unsigned char state=0;
__ramfunc void Flash_Write_Byte(uchar data)//(uint16_t segmentNo,uint16_t offset,uint8_t data)
{
  char * Addr;                         // Initialize Flash pointer
  Addr = (char *) 0x1880;
  FCTL3 = FWKEY;                            //清除锁定
  FCTL1 = FWKEY+WRT;                        //置位编程位
  *Addr = data;                             //写入一个字
  while (FCTL3 & BUSY);                     //等待操作完成
  FCTL1 = FWKEY;                            //清除写入位
  FCTL3 = FWKEY+LOCK;                       //对FLASH加锁，可读，不可写，不可擦除，保护数据
}


__ramfunc void Flash_Erase_Segment()//(uint16_t segmentNo)
{
// ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Erase_Segment","segmentNo参数超出范围！");  //断言检测段范围是否在FLASH区间

//  uint16_t * Addr = (uint16_t *)((uint32_t)segmentNo*512);   //计算地址 C段0X1800 ，D段 0X1880;
  char * Addr;                         // Initialize Flash pointer
  Addr = (char *) 0x1880;

  FCTL3 = FWKEY;                              // 清除锁定
  FCTL1 = FWKEY+ERASE;                        // 擦除一段
  *Addr = 0;                                  //对擦除的地址范围内任意做一次空写入，用以启动擦除操作
  while (FCTL3 & BUSY);                       // 等待允许操作
  FCTL1 = FWKEY;                              //清除擦除位
  FCTL3 = FWKEY+LOCK;                         //对FLASH加锁，可读，不可写，不可擦除
}
//void Flash_Esntdecimal(float num,int  len ,int z_len,int uu)
//{         	int f_len;
//	unsigned char t,temp;
//	unsigned char enshow;
//	unsigned long f_temp;
//       unsigned long z_temp;   
//       f_len=len-z_len;
//	z_temp=(unsigned long)num;
//        static int FAddr = 0;
//        FAddr += 11*uu;
//	//整数部分
//	for(t=0;t<z_len;t++)
//	{
//		temp=(z_temp/abc(z_len-t-1))%10;
//		if(enshow==0 && t<(z_len-1))
//		{
//			if(temp == 0)
//			{
//				Flash_Write_Byte(' ',FAddr);
//                                FAddr++;
//				continue;
//			}
//			else
//			enshow=1;
//		}
//		Flash_Write_Byte(temp + '0',FAddr);
//                FAddr++;
//	}
//	//小数点
//	Flash_Write_Byte('.',FAddr); 
//          FAddr++;
//	
//	f_temp=(int)((num-z_temp)*(abc(f_len)));
//  //小数部分
//	for(t=0;t<f_len;t++)
//	{
//		temp=(f_temp/abc((f_len-t-1)-'0'))%10;
//		Flash_Write_Byte(temp + '0',FAddr);
//                FAddr++;
//	}
//        FAddr = 0;
//}
void senfchar(char s)
{
    UCA0TXBUF=s;
    while(!(UCA0IFG&UCTXIFG));
}

void sendstring(unsigned char *p)
{
    while(*p!='\0')
    {
        while(!(UCA0IFG&UCTXIFG));
        UCA0TXBUF=*p++;

    }
}
void delay()
{
    for(i=50000;i>0;i--);
}
void TestWeight()
{//判断是否超出量程
      if(Weight_Shiwu>500)
           waming ();
else stopwaming ();   
}
void init_hx711port(void)
{//引脚初始化
P4DIR |= BIT0; /* 单片机P4.0引脚输出 */
P4OUT &= ~BIT0; /* 单片机P4.0引脚输出低电平 */

P4DIR &= ~BIT1; /* 单片机P4.1引脚输入 */
/* P4REN |= BIT1;   //单片机P4.1引脚输入上拉使能 F149单片机这里没内部上拉 */
//P4OUT |= BIT1; /* 单片机P4.1引脚输入上拉电阻 */
}
unsigned long read_hx711_24bit(void)
{//读取HX711内部的数据 
unsigned long Count = 0;
unsigned char i;
SCK_L; /* SCL=0;使能AD（PD_SCL 置低） */
    while ( SDA_IN)
        ;
    /* AD转换未结束则等待，否则开始读取 循环读取24次 */
    for (i = 0; i < 24; i++)
    {
        SCK_H; /* SCL = 1; PD_SCL 置高（发送脉冲） */
        Count = Count << 1; /*下降沿来时变量Count左移一位，右侧补零 */
      //  delay_us(2);
        SCK_L; /* SCL=0; PD_SCL 置低 */
        if ( SDA_IN)
            Count++; /* if(SDA) Count++; */
      //  delay_us(2);
    }
SCK_H; /* SCL=1; */
Count = Count ^ 0x800000; //第25个脉冲下降沿来时，转换数据
SCK_L; /* SCL=0; */
return (Count);
}
void get_maopi(void)
{//获取初始重量
Weight_Maopi = read_hx711_24bit();
Weight_Maopi = read_hx711_24bit(); // 多读一次确保稳定
}
unsigned long get_weight(void)
{//获取重量 返回g单位的重量 

HX711_Buffer = read_hx711_24bit();
maopia = (unsigned long) ((float) Weight_Maopi / GapValue); //毛皮重量转换成g 
HX711_Buffer = (unsigned long) ((float) HX711_Buffer / GapValue); //此时称重转换成g 
    if (HX711_Buffer > maopia)
    {
        Weight_Shiwu = HX711_Buffer - maopia; /* 计算实物的实际重量 */
        Weight_Shiwu=Weight_Shiwu/3.4;
        return (Weight_Shiwu); /* 测量正确返回g单位的重量数值 */
    }
    return (0); /* 称重不符合规范(HX711_Buffer > Weight_Maopi) 返回2 */
}
void waming ()
{//报警函数
P6DIR |= BIT5;//蜂鸣器轰鸣报警//灯泡亮起报警
P6OUT &=~ BIT5;
P3DIR |= BIT0;
P3OUT |= BIT0;
}
void stopwaming ()
{//报警函数
  P6OUT |= BIT5;
  P3OUT &=~ BIT0;
/*蜂鸣器停止轰鸣报警*//*灯泡停止亮起报警*/ 
  
}

void testchar();
uint Strlen(uchar *p)
{//
uint i;
for(i=0;*(p+i);i++);
return i;
}
void WriteBytes(uchar *p)
{
uchar i,j,temp;
en_out;
rw_out;
en_l;
for(j=0;j<3;j++)
{
temp=p[j];
for(i=0;i<8;i++)
{
if(temp & 0x80)
rw_h;
else rw_l;
en_h;
en_l;
temp<<=1;
}
}
rw_h;
}
void SerialWriteData(uchar send)
{
uchar data[3];
data[0]=0xFa;//1111?1010向液晶写数据
data[1]=send & 0xF0;//取高4位
data[2]=(send<<4)&0xF0;//取低4位
WriteBytes(data);
}
void SerialWriteCmd(uchar send)
{
uchar data[3];
data[0]=0xf8 ;//1111?1000向液晶写命令
data[1]=send & 0xf0;//取高4位
data[2]=(send<<4) & 0xf0;//取低4位
WriteBytes(data);
}
void init_lcd()
{
en_out;
rw_out;
SerialWriteCmd(0x0C);//显示开，关闭游标和游标位置
SerialWriteCmd(0x06);
SerialWriteCmd(0x01);//清除显示，等待命令
}
void display_12864(uchar hang,uchar lie,uchar *p)
{
uchar i;
switch(hang)
{
case 1:SerialWriteCmd(0x80+lie);break;
case 2:SerialWriteCmd(0x90+lie);break;
case 3:SerialWriteCmd(0x88+lie);break;
case 4:SerialWriteCmd(0x98+lie);break;
default:break;
}
for(i=0;p[i]!='\0';i++)
{
SerialWriteData(p[i]);
}
}

void clear_12864()
{
SerialWriteCmd(0x01);
display_12864(3,0,"价格:\0  ");
display_12864(1,0,"重量:");
display_12864(1,6,"g");
display_12864(2,0,"单价:");
display_12864(2,7,"元");
}
int abc(int n)
{
  int t=1;
if(n>0)
  for(int i=1;i<=n;i++)
 {t=t*10;}
else
t=1;
return t;
}
int dd=0;
void transmission()
{//向LCD显示屏发送数据 
   //int i=0;
   m=0;
for(int i=0;danjia[i]!='.';i++)
{
   if(danjia[i]=='\0')
    break;
    xx=i;
}
for(int i=0;i<=xx+3;i++)
{ 
   if(danjia[i]=='\0')
    break;
  else
    if(i==xx+1)
   danjia[i]=danjia[i] ;
  else
    if(i==xx+2)
    m=m+(float)((danjia[i]-'0')*0.1);
  
  else
    if(i==xx+3)
    m=m+(float)((danjia[i]-'0')*0.01);
  else
  {
    dd=abc(xx-i);
    m=m+((danjia[i]-'0')*dd);
  }
}
zhongliang[0]=(uint)(Weight_Shiwu/1000)+'0';
zhongliang[1]=(uint)((Weight_Shiwu%1000)/100)+'0';
zhongliang[2]=(uint)((Weight_Shiwu%100)/10)+'0';
zhongliang[3]=(uint)(Weight_Shiwu%10)+'0';
n=(Weight_Shiwu*m);
if(n<1000)//如果价格在1000元以下，采用：元。若价格在1000元以上，采用：万。
{
 e=(uint)(n*100);
  jiage[0]=(uint)(e/10000)+'0';
  jiage[1]=(uint)((e/1000)%10)+'0';
  jiage[2]=(uint)((e/100)%10)+'0';
  jiage[4]=(uint)((e/10)%10)+'0';
  jiage[5]=(uint)((e%10))+'0';
 display_12864(3,7,"元");  
 display_12864(3,3,jiage);
}
else
{
  e=(uint)(n/100);
  jiage[0]=(uint)(e/10000)+'0';
  jiage[1]=(uint)((e/1000)%10)+'0';
  jiage[2]=(uint)((e/100)%10)+'0';
  jiage[4]=(uint)((e/10)%10)+'0';
  jiage[5]=(uint)((e%10))+'0';
  display_12864(3,7,"万");
  display_12864(3,3,jiage);
}
display_12864(1,3,zhongliang);//输出到LCD屏幕上。
display_12864(2,3,danjia);
display_12864(3,3,jiage);//输出到LCD屏幕上。
}

void sss()
{
  n=(Weight_Shiwu*m);
  ss=n+ss;
  yy=(uint)(ss/100);
  pp+=1;

  he[0]=(uint)(yy/10000)+'0';
  he[1]=(uint)((yy/1000)%10)+'0';
  he[2]=(uint)((yy/100)%10)+'0';
  he[4]=(uint)((yy/10)%10)+'0';
  he[5]=(uint)((yy%10))+'0';
 

}
void keyboard_init()
{//设置行管脚为输入
    
    P3DIR &= ~BIT5; //P3.5端口设为输入
    P3REN |= BIT5;  //使3.5能上拉/下拉电阻
    P3OUT &=~ BIT5;  //置P3.5为下拉电阻模式
    P3SEL &= ~BIT5; //P3.5为I/O接口

    P3DIR &= ~BIT6; //P3.6端口设为输入
    P3REN |= BIT6;  //使P3.6能上拉/下拉电阻
    P3OUT &=~ BIT6;  //置P3.6为下拉电阻模式
    P3SEL &= ~BIT6; //P3.6为I/O接口

    P7DIR &= ~BIT0; //P7.0端口设为输入
    P7REN |= BIT0;  //使P7.0能上拉/下拉电阻
    P7OUT &=~ BIT0;  //置P7.0为下拉电阻模式
    P7SEL &= ~BIT0; //P7.0为I/O接口

    P6DIR &= ~BIT4; //P6.4端口设为输入
    P6REN |= BIT4;  //使P6.4能上拉/下拉电阻
    P6OUT &=~ BIT4;  //置P6.4为下拉电阻模式
    P6SEL &=~ BIT4; //P6.4为I/O接口



    //设置列管脚为输出
    P6DIR |= BIT3+BIT2+BIT1+BIT0; //P6.3,2,1,0端口设为输出
    P6OUT &=~(BIT3+BIT2+BIT1+BIT0);

}  
uchar c[2]="00",u='0';
uchar g[7]={'0','0','0','0','0','0','0'};
void kkk()
{
  u=u+1;
   Flash_Write_Byte(u);
   sendstring("\n苹果：");
   c[0]=(uchar)(*Flash_ptr);
   sendstring(c);
   sendstring("元\n");
}
void hhh()
{
clear_12864();
transmission();
display_12864(3,0,"价格:");
display_12864(1,0,"重量:");
display_12864(1,6,"g");
display_12864(2,0,"单价:");
display_12864(2,7,"元");


}
#define CPU_CLOCK       8000000
 #define delay_ms(ms)    __delay_cycles(CPU_CLOCK/1000*(ms))
 int ms=0;
 int s=0,b=0;
 uchar p[3]={'0','0','\0'},jh[3]={'0','0','\0'};
void time()
{  
  ms++;
if(ms==60)
{
  ms=0;
  s++;
  if(s==60)
{
  s=0;

}
}
  p[0]=ms/10+'0';
  p[1]=ms%10+'0';
  jh[0]=s/10+'0';
  jh[1]=s%10+'0';
}

void key()
{//扫描列
    
    char key_value=0;
       if(state==0){

            P6OUT |= BIT0;
            P6OUT &=~ (BIT1+BIT2+BIT3);

        }else if(state==1){

            P6OUT |= BIT1;
            P6OUT &=~ (BIT0+BIT2+BIT3);

        }else if(state==2){

            P6OUT |= BIT2;
            P6OUT &=~ (BIT0+BIT1+BIT3);

        }else if(state==3){

            P6OUT |= BIT3;
            P6OUT &=~ (BIT0+BIT1+BIT2);

        }
        if(++state==4)
          state=0;
       
        if(P6OUT & BIT0)
            key_value = BIT4;

        if(P6OUT & BIT1)
            key_value = BIT5;

        if(P6OUT & BIT2)
            key_value = BIT6;

        if(P6OUT & BIT3)
            key_value = BIT7;

        if(P3IN & BIT5) key_value |= BIT0;
        else key_value &= ~BIT0;

        if(P3IN & BIT6) key_value |= BIT1;
        else key_value &= ~BIT1;

        if(P7IN & BIT0) key_value |= BIT2;
        else key_value &= ~BIT2;

        if(P6IN & BIT4) key_value |= BIT3;
        else key_value &= ~BIT3;

     switch(key_value)
     {//返回按下的字符
        case 17:              Flash_Erase_Segment();break;
        case 18:  if((w!=0)&&danjia[0]!='.'&&danjia[1]!='.'&&danjia[2]!='.'&&danjia[3]!='.'&&danjia[4]!='.'&&danjia[5]!='.'&&danjia[6]!='.')
        {  danjia[w]= '.';w++;}else;break;                            
        case 20:   sss();break;
        case 24:   ss=0;break;
     case 33:  if(w<6){danjia[w]=0+'0';w++;}if(w>6){w=0;}break;
     case 34:  if(w<6){danjia[w]=1+'0';w++;}if(w>6){w=0;}break;
     case 36:  if(w<6){danjia[w]=2+'0';w++;}if(w>6){w=0;}break;
     case 40:  if(w<6){danjia[w]=3+'0';w++;}if(w>6){w=0;}break;
     case 65:  if(w<6){danjia[w]=4+'0';w++;}if(w>6){w=0;}break;
     case 66:  if(w<6){danjia[w]=5+'0';w++;}if(w>6){w=0;}break;
     case 68:  if(w<6){danjia[w]=6+'0';w++;}if(w>6){w=0;}break;
     case 72:  if(w<6){danjia[w]=7+'0';w++;}if(w>6){w=0;}break;
     case 129: if(w<6){danjia[w]=8+'0';w++;}if(w>6){w=0;}break;
     case 130: if(w<6){danjia[w]=9+'0';w++;}if(w>6){w=0;}break;
     case 132: for(int i=0;i<6;i++)danjia[i]='0';w=0;break;
        case 136: kkk();break;
        default: juzhen[0]=0;break;

}
}

unsigned int k=0;
unsigned int LastCCR1;		//上一次高电平持续的时间，和距离正相关
unsigned int dat[20];		//通过数组备份采集到的数据
 
unsigned char wen[5]={'0','0','.','0','\0'};

unsigned char hc[5] = {0,0,'.',0,'\0'};
 
/*初始化HC_SR04Init()模块*/
void HC_SR04Init()
{
	P1DIR|=BIT2;
}
/*开始超声发送*/
void HC_SR04Start()
{
	P1OUT|=BIT2;		
	__delay_cycles(12);		//至少持续10us的高电平
	P1OUT&=~BIT2;			
}
 
/*初始化捕获模式，获取时间*/
void TimeGetInit()
{
	P1DIR&=~BIT4;
	P1SEL|=BIT4;
 
	TA0CTL=TASSEL_2+ID_0+MC_2+TACLR;
	TA0CCTL3=CM_1+SCS+CAP+CCIE+CCIS_0;
}

#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)   // Temperature Sensor Calibration-30 C                                                      //See device datasheet for TLV table memory mapping
#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)   // Temperature Sensor Calibration-85 C

unsigned int temp;
volatile float temperatureDegC;
volatile float temperatureDegF;
void star()
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
                                            // ADC12_A ref control registers
  ADC12CTL0 = ADC12SHT0_8 + ADC12REFON + ADC12ON;
                                            // Internal ref = 1.5V
  ADC12CTL1 = ADC12SHP;                     // enable sample timer
  ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;  // ADC i/p ch A10 = temp sense i/p
  ADC12IE = 0x001;                          // ADC_IFG upon conv result-ADCMEMO
  ADC12CTL0 |= ADC12ENC;
}

void main( void )
{//主函数

  WDTCTL = WDTPW + WDTHOLD;
  P3SEL = BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 9;                              // 1MHz 115200 (see User's Guide)
  UCA0BR1 = 0;                              // 1MHz 115200
  UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    _EINT();                                            // over sampling
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;   
  
	HC_SR04Init();			//初始化超声模块HC_SR04
	TimeGetInit();			//初始化计数捕获引脚，模式等
	_EINT();
        
init_hx711port();
init_lcd();
read_hx711_24bit();
keyboard_init();
get_maopi();
get_weight();
_EINT();
P1DIR |= BIT0;       
TA1CCTL0 = CCIE;
TA1CCR0 = 50000;
TA1CTL = TASSEL_2 + MC_1 + TACLR;
u8_count_10ms=0;
P2REN |= BIT1;
P2OUT |= BIT1;
P2IFG &= ~BIT1;
P2IE |= BIT1;
P2IES |= BIT1;
display_12864(1,5,"g");
display_12864(2,0,"单价:");
display_12864(2,7,"元");
display_12864(3,0,"价格:\0  ");
display_12864(1,0,"重量:");
//Flash_Erase_Segment();
  star();
  int yuu=0;
    while(1)
    { 
      yuu++;
      if(yuu>80)
        time();
      get_weight();
      transmission();
      TestWeight();
      HC_SR04Start();
      ADC12CTL0 &= ~ADC12SC; 
      ADC12CTL0 |= ADC12SC;                   // Sampling and conversion start
    __bis_SR_register(LPM4_bits + GIE);     // LPM0 with interrupts enabled
    temperatureDegC = ((float)(((long)temp - CALADC12_15V_30C) * (85 - 30)) /(CALADC12_15V_85C - CALADC12_15V_30C) + 30.0f)*10;
    temperatureDegF = (temperatureDegC * 9.0f / 5.0f + 32.0f);
    wen[0]=(uint)((uint)temperatureDegC/100)+'0';
    wen[1]=(uint)((uint)temperatureDegC%100/10)+'0';
    wen[3]=(uint)((uint)temperatureDegC%10)+'0';
    }

}





#pragma vector =PORT2_VECTOR
__interrupt void P3(void)
{
	if(P2IFG & BIT1)
	{
         __delay_cycles(1000);
         if(P2IFG & BIT1)
	 { 
           get_maopi();
           Flash_Erase_Segment();
         }
         P2IFG &= ~BIT1;
        }
}
#pragma  vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A_ISR(void)
{
unsigned char u8_count_10ms=0;

u8_count_10ms++;
if(u8_count_10ms>3)
   {
    
     u8_count_10ms=0;
   }
    else 
      if(u8_count_10ms==1)
    {                          // SET BREAKPOINT HERE
    key();
    } 




} 
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(UCA0RXBUF)
  {
  case '0':sendstring("\n单价：\r");
      sendstring(danjia);
      sendstring("元\n");      
      sendstring("重量：\r");
      sendstring(zhongliang);
      sendstring("g\n");  
      sendstring("价格：\r");
      sendstring(jiage);
      if(n<1000)
        sendstring("元\n");
      else
      sendstring("万\n");   
        sendstring("求和：");
        sendstring(he);
        sendstring("万元\n");
        senfchar(pp);
        sendstring("\n距离");
       sendstring(hc);
       sendstring("cm\n");
       sendstring("温度");
       sendstring(wen);
       sendstring("摄氏度\n");
       sendstring("时间:");

                          sendstring(p);
                          sendstring("/");
                          sendstring(jh);
                                 sendstring("\n");
 break;                   // Vector 4 - TXIFG
case '1':Flash_Erase_Segment(); break; 
      
  default: break;
  }
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
  P1DIR |= BIT0;
  P1OUT |= BIT0;
	_DINT();						//关中断
	static unsigned char times=1;
	static unsigned char i=0;
	unsigned char j;
	unsigned int t;
 
	if(times==1)
	{
		LastCCR1=TA0CCR3;		//记录下上次CCR3的值
		TA0CCTL3&=~CM_1;		//清上升沿捕获
		TA0CCTL3|=CM_2;			//改为下降沿捕获
		times++;
	}
	if(times==0)
	{
		if(i<20)				//把采集到的20次的值都放到数组中
		{
			dat[i]=TA0CCR3-LastCCR1;
			i++;
		}
		if(i>=20)				//为了防止误差，排序把采集到的两头两尾的值去掉，取中间的平均值
		{
			for(i=0;i<20;i++)
			{
				for(j=0;j<20-i;j++)
				{
					if(dat[j]>dat[j+1])
					{
						t=dat[j];
						dat[j]=dat[j+1];
						dat[j+1]=t;
					}
				}
			}
			
			t=0;
			for(i=7;i<12;i++)
			{
				t=t+dat[i];
			}
			k=t/254*20;
			i=0;
                        hc[0] = (unsigned int)(k / 100) + '0';
                        hc[1] = (unsigned int)((k % 100) / 10) + '0';
                        hc[3] = (unsigned int)(k % 10)  + '0';                    
                        //OLED_ShowString(72,4,hc);
		}
		TA0CCTL3&=~CM_2;		//清除下降沿触发
		TA0CCTL3|=CM_1;			//变为上升沿触发
		times++;				//改变times的值
	}
	times&=0x01;				//times>1时清0
	LPM0_EXIT;					//退出低功耗模式
	TA0CCTL3&=~CCIFG;			//清除中断响应标志
	_EINT();					//开中断
}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6:                                  // Vector  6:  ADC12IFG0
    temp = ADC12MEM0;                       // Move results, IFG is cleared
    __bic_SR_register_on_exit(LPM4_bits);   // Exit active CPU
    break;
  case  8: break;                           // Vector  8:  ADC12IFG1
  case 10: break;                           // Vector 10:  ADC12IFG2
  case 12: break;                           // Vector 12:  ADC12IFG3
  case 14: break;                           // Vector 14:  ADC12IFG4
  case 16: break;                           // Vector 16:  ADC12IFG5
  case 18: break;                           // Vector 18:  ADC12IFG6
  case 20: break;                           // Vector 20:  ADC12IFG7
  case 22: break;                           // Vector 22:  ADC12IFG8
  case 24: break;                           // Vector 24:  ADC12IFG9
  case 26: break;                           // Vector 26:  ADC12IFG10
  case 28: break;                           // Vector 28:  ADC12IFG11
  case 30: break;                           // Vector 30:  ADC12IFG12
  case 32: break;                           // Vector 32:  ADC12IFG13
  case 34: break;                           // Vector 34:  ADC12IFG14
  default: break;
  }
}