#include "msp430f5529.h"
#include "keys.h"
#include "cryfucns.h"
#include "sound.h"
#include "FLASH.h"

#define CPU_F ((double)1035000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define _delay_cycles __delay_cycles
#define List0   0x80
#define List1   0x90
#define List2   0x88
#define List3   0x98
#define light1  P1OUT |=BIT5;
#define light2  P2OUT |=BIT4;
#define light3  P2OUT |=BIT5;
#define lightoff P1OUT &= ~BIT5,P2OUT &= ~BIT4,  P2OUT &= ~BIT5;
int x=0;
int length=0;
char *kong="                   ";

int yindiao[10]={264,297,330,352,396,440,495,528};
int yindiao2[10]={123,109,98,92,82,73,65,61};
unsigned int k=0;
unsigned int LastCCR1;		//上一次高电平持续的时间，和距离正相关
unsigned int dat[20];		//通过数组备份采集到的数据
char juli[10]="0";
char * Flash_ptr; 
void play();
void play2();
void play3();
static char data[14];//存储播放音调
//static char data2[14];//存储播放音调
char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
    unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
    int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
 
    //获取要转换的整数的绝对值
    if(radix==10&&num<0)//要转换成十进制数并且是负数
    {
        unum=(unsigned)-num;//将num的绝对值赋给unum
        str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
    }
    else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
    //转换部分，注意转换后是逆序的
    do
    {
        str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
        unum/=radix;//unum去掉最后一位
 
    }while(unum);//直至unum为0退出循环
 
    str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
    //将顺序调整过来
    if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
    else k=0;//不是负数，全部都要调整
 
    char temp;//临时变量，交换两个值时用到
    for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
    {
        temp=str[j];//头部赋值给临时变量
        str[j]=str[i-1+k-j];//尾部赋值给头部
        str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
    }
 
    return str;//返回转换后的字符串
 
}

__ramfunc void Flash_Write_Byte(uchar data)//(uint16_t segmentNo,uint16_t offset,uint8_t data)
{
  char * Addr;                         // Initialize Flash pointer
  Addr = (char *) 0x1880;
  FCTL3 = FWKEY;                            //清除锁定
  FCTL1 = FWKEY+WRT;                        //置位编程位
  _DINT();  
  *Addr = data;                             //写入一个字
  while (FCTL3 & BUSY);                     //等待操作完成
  _EINT(); 
  FCTL1 = FWKEY;                            //清除写入位
  FCTL3 = FWKEY+LOCK;                       //对FLASH加锁，可读，不可写，不可擦除，保护数据
}
char FlashReadChar(unsigned int Addr)
{ 
    char Data;
    char *FlashPtr = (char *) Addr; 
    Data = *FlashPtr;
    return(Data);
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
void FlashRead(uchar *pc_byte,uchar *Dataout,uint count)
{
   while(count--)
  {
    *Dataout = *pc_byte;
    Dataout++;
    pc_byte++;
    }
}


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
 
void light()
{
  P1DIR |= BIT5;
  P2DIR |= BIT4;
  P2DIR |= BIT5;
  P1OUT &= ~BIT5;
  P2OUT &= ~BIT4;
  P2OUT &= ~BIT5;
}
char ans[14];
char ans2[14];
int flag=0;
int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  //unsigned char key;
  P1REN |= BIT1;
  P1OUT |= BIT1;
  P4DIR |= BIT7;
  P4OUT &= ~BIT7;
  light();
  Ini_Lcd();
  HC_SR04Init();			//初始化超声模块HC_SR04
  TimeGetInit();			//初始化计数捕获引脚，模式等
  _EINT();
  while(1)
  {
    Disp_HZ(List0,"声调:",0);
    Disp_HZ(List1,"模式:",0);
    Disp_HZ(List1+3,"普通",0);
    HC_SR04Start();
    /*P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;*/
    
     /*UART的初始化*/
    P3SEL |= BIT3+BIT4;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2; 
    UCA0MCTL |= UCBRS_1 + UCBRF_0;
    UCA0BR0 = 9;
    UCA0BR1 = 0;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCRXIE;
    
    read_flash_char1(0x1800,ans,7);
    Disp_SZ(List2,ans[0]);
    Disp_SZ(List2+1,ans[1]);
    Disp_SZ(List2+2,ans[2]);
    Disp_SZ(List2+3,ans[3]);
    Disp_SZ(List2+4,ans[4]);
    Disp_SZ(List2+5,ans[5]);
    Disp_SZ(List2+6,ans[6]);
    read_flash_char1(0x1940,ans2,7);
    Disp_SZ(List3,ans2[0]);
    Disp_SZ(List3+1,ans2[1]);
    Disp_SZ(List3+2,ans2[2]);
    Disp_SZ(List3+3,ans2[3]);
    Disp_SZ(List3+4,ans2[4]);
    Disp_SZ(List3+5,ans2[5]);
    Disp_SZ(List3+6,ans2[6]);
    __bis_SR_register(LPM0_bits + GIE);
    /*if(!(P1IN & BIT1) == 1)
    {
      flag=1;
      P4OUT ^= BIT7;
    }
    if(flag==0)
    {
      _DINT();
      UCA0IFG &= ~UCRXIFG;
    }
    else*/
    //__bis_SR_register(LPM0_bits + GIE);
    /*unsigned long * read_point;
    unsigned char read_content ;
    read_point = (unsigned long *) 0x1880;
    read_content = *read_point;*/
    /*key=scanKey();
    if(key=scanKey())
    {
      __delay_cycles(1000);
      if(key=='1')
      {
        jby_1(400);
        Disp_HZ(List0+3,"do",0);
        light1;
        delay_ms(1000); 
      }
      else if(key=='2')
      {
        jby_2(400);
        Disp_HZ(List0+3,"re",0);
        light2;
        delay_ms(1000); 
      }
      else if(key=='3')
      {
        jby_3(400);
        Disp_HZ(List0+3,"mi",0);
        light3;
        delay_ms(1000); 
      }
      else if(key=='4')
      {
        jby_4(400);
        Disp_HZ(List0+3,"fa",0);
        light1;
        light2;
        delay_ms(1000); 
      }
      else if(key=='5')
      {
        jby_5(400);
        Disp_HZ(List0+3,"sol",0);
        light1;
        light3;
        delay_ms(1000); 
      }
      else if(key=='6')
      {
        jby_6(400);
        Disp_HZ(List0+3,"la",0);
        light2;
        light3;
        delay_ms(1000); 
      }
      else if(key=='7')
      {
        jby_7(400);
        Disp_HZ(List0+3,"si",0);
        light1;
        light2;
        light3;
        delay_ms(1000); 
      }
      else if(key=='8')
      {
        Disp_HZ(List1+3,"播放",0);
        //play();
        delay_ms(1000); 
      }
      else if(key=='9')
      {
        Disp_HZ(List1+3,"音调",0);
      }
      
    }*/
    /*else
      {
        //Send(0, 0x01);
        lightoff;
        delay_ms(1000); 
      }*/
    
  }
}
int cnt=0;
#pragma vector=USCI_A0_VECTOR  // 进入中断向量地址
__interrupt void USCI_A0_ISR(void)
{
  //static char Buff = 0;
  while (!(UCA0IFG&UCRXIFG));
  int tp=0;
  while(tp!=13)
  {
    //Buff = UCA0RXBUF;
    data[tp]=UCA0RXBUF;
    tp++;
    __delay_cycles(75);
    
  }
  int timess=0;
  if(cnt%2==0)
  {
    write_flash_char(0x1800,data,14);
  }
  else if(cnt%2==1)
  {
    write_flash_char(0x1940,data,14);
  }
  cnt++;

  while(data[timess]!='0')
  {
    
      switch(data[timess])
      {
      case 'p':
      {
        lightoff;
        //play();
      } break;
      case 'o':
      {
        //play2();
      } break;
      case 'i':
      {
        //play3();
      } break;
      case '1':
        {
        jby_1(400);
        Disp_HZ(List0+3,"do",0);
        lightoff;
        light1;
         } break;
      case '2':
      {
        jby_2(400);
        Disp_HZ(List0+3,"re",0);
        lightoff;
        light2;
        
      } break;
      case '3':
        {
        jby_3(400);
        Disp_HZ(List0+3,"mi",0);
        lightoff;
        light3;
        
      } break;
      case '4':
       {
        jby_4(400);
        Disp_HZ(List0+3,"fa",0);
        lightoff;
        light1;
        light2;
         
      } break;
      case '5':
       {
        jby_5(400);
        Disp_HZ(List0+3,"sol",0);
        lightoff;
        light1;
        light3;
        
        Send(0, 0x01);
        Disp_HZ(List0,"声调:",0);
        Disp_HZ(List1,"模式:",0);
      } break;
   case '6':{
        jby_6(400);
        Disp_HZ(List0+3,"la",0);
        lightoff;
        light2;
        light3;
         
      } break;
   case '7':
    {
        jby_7(400);
        Disp_HZ(List0+3,"si",0);
        lightoff;
        light1;
        light2;
        light3;
        
      } break;
      case 'e':
    {
        Flash_Erase_Segment();
        write_flash_char(0x1800,kong,14);
        write_flash_char(0x1940,kong,14);
        
      } break;
    default: break;
    } 
      timess++;
    }
  //_DINT();
  UCA0IFG &= ~UCRXIFG;
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
			dat[i]=TA0CCR3-LastCCR1;;
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
			k=t/5;//
			i=0;
		}
		TA0CCTL3&=~CM_2;		//清除下降沿触发
		TA0CCTL3|=CM_1;			//变为上升沿触发
		times++;				//改变times的值
	}
	times&=0x01;				//times>1时清0
	Disp_HZ(List2,"距离:",0);
        k*=1000000;
        length=340*(k/2)/100;//cm
        itoa(length,juli,10);
        Disp_SZ(List2+3,juli[0]);
        Disp_SZ(List2+4,juli[1]);
        Disp_SZ(List2+5,juli[2]);
        LPM0_EXIT;					//退出低功耗模式
	TA0CCTL3&=~CCIFG;			//清除中断响应标志
        _EINT();					//开中断
}

void play()
{
     Disp_HZ(List1+3,"播放",0);
     jby_1(400);
     jby_2(400);
     jby_3(400);
     jby_1(400);
     _delay_cycles(3000); //每个完整的拍后接一个延时
      P1OUT ^=BIT5;
      P2OUT ^=BIT4;
      P2OUT ^=BIT5;
     jby_1(400);
     jby_2(400);
     jby_3(400);
     jby_1(400);
    _delay_cycles(3000);
      P1OUT ^=BIT5;
      P2OUT ^=BIT4;
      P2OUT ^=BIT5;
    
     jby_3(400);
     jby_4(400);
     jby_5(800);
    _delay_cycles(3000);
      P1OUT ^=BIT5;
      P2OUT ^=BIT4;
      P2OUT ^=BIT5;
     jby_3(400);
     jby_4(400);
     jby_5(800);
     _delay_cycles(3000);
     P1OUT ^=BIT5;
      P2OUT ^=BIT4;
      P2OUT ^=BIT5;
     jby_5(200);
     jby_6(200);
     jby_5(200);
     jby_4(200);
     jby_3(400);
     jby_1(400);
     _delay_cycles(3000);
      P1OUT ^=BIT5;
      P2OUT ^=BIT4;
      P2OUT ^=BIT5;
     jby_5(300);
     jby_6(100);
     jby_5(300);
     jby_4(100);
     jby_3(400);
     jby_1(400);
     _delay_cycles(3000);
      P1OUT ^=BIT5;
      P2OUT ^=BIT4;
      P2OUT ^=BIT5;
     jby_2(400);
     jby_5(400);
     jby_1(800);
     _delay_cycles(3000);
      P1OUT ^=BIT5;
      P2OUT ^=BIT4;
      P2OUT ^=BIT5;
     jby_2(400);
     jby_5(400);
     jby_1(800);
     Send(0, 0x01);
     _delay_cycles(10000);
      P1OUT &=~BIT5;
      P2OUT &=~BIT4;
      P2OUT &=~BIT5;
     Disp_HZ(List0,"声调:",0);
     Disp_HZ(List1,"模式:",0);
}
void play2()
{
  jby_3(400);
  jby_3(200);
  jby_4(200);
  jby_3(400);
  jby_2(200);
  jby_1(200);
  _delay_cycles(3000);
  jby_2(200);
  jby_2(200);
  jby_2(200);
  jby_3(200);
  jby_5(700);
  _delay_cycles(3000);
  jby_6(400);
  jby_6(300);
  jby_7(100);
  jby_1(400);
  jby_7(200);
  jby_6(200);
  _delay_cycles(3000);
  jby_5(200);
  jby_5(200);
  jby_5(200);
  jby_3(200);
  jby_3(800);
  _delay_cycles(3000);
  jby_3(400);
  jby_3(200);
  jby_4(200);
  jby_5(400);
  jby_3(200);
  jby_1(200);
  _delay_cycles(3000);
  jby_2(200);
  jby_2(200);
  jby_2(200);
  jby_5(200);
  jby_2(800);
  _delay_cycles(3000);
  jby_1(400);
  jby_5(200);
  jby_1(200);
  jby_3(400);
  jby_5(400);
  _delay_cycles(3000);
  jby_4(200);
  jby_4(200);
  jby_3(200);
  jby_2(200);
  jby_1(800);
  _delay_cycles(3000);
  jby_1(200);
  jby_2(200);
  jby_3(400);
  jby_2(800);
  _delay_cycles(3000);
  jby_1(200);
  jby_1(200);
  jby_2(200);
  jby_2(200);
  jby_5(800);
  _delay_cycles(3000);
  jby_1(200);
  jby_2(200);
  jby_3(400);
  jby_2(800);
  _delay_cycles(3000);
  jby_1(300);
  jby_1(100);
  jby_5(400);
  jby_3(800);
  _delay_cycles(3000);
  jby_1(300);
  jby_2(100);
  jby_3(400);
  jby_2(800);
  _delay_cycles(3000);
  jby_1(300);
  jby_1(100);
  jby_2(200);
  jby_2(200);
  jby_5(800);
  _delay_cycles(3000);
  jby_1(200);
  jby_2(200);
  jby_3(400);
  jby_2(800);
  _delay_cycles(3000);
  jby_1(300);
  jby_2(100);
  jby_1(1000);
}
void play3()
{
  jby_3(400);
  jby_3(400);
  _delay_cycles(3000);
  jby_3(400);
  jby_3(400);
  _delay_cycles(3000);
  jby_4(400);
  jby_4(400);
  _delay_cycles(3000);
  jby_4(400);
  jby_5(400);
  _delay_cycles(3000);
  jby_3(400);
  jby_3(400);
  _delay_cycles(3000);
  jby_2(800);
  _delay_cycles(3000);
  jby_5(800);
  
  jby_5(400);
  jby_6(400);
  jby_3(400);
  _delay_cycles(3000);
  jby_2(400);
  jby_1(400);
  jby_2(400);
  _delay_cycles(3000);
  jby_6(400);
  jby_1(400);
  jby_2(400);
  jby_6(400);
  _delay_cycles(3000);
  jby_5(800);
  _delay_cycles(3000);
  jby_6(400);
  jby_1(400);
  jby_2(400);
  _delay_cycles(3000);
  jby_3(400);
  jby_5(400);
  jby_5(400);
  _delay_cycles(3000);
  jby_6(400);
  jby_5(400);
  jby_3(400);
  jby_1(400);
  _delay_cycles(3000);
  jby_2(800);
  _delay_cycles(3000);
  jby_3(400);
  jby_5(400);
  jby_3(400);
  _delay_cycles(3000);
  jby_5(400);
  jby_5(400);
  jby_6(400);
  jby_5(400);
  _delay_cycles(3000);
  jby_3(400);
  jby_2(400);
  jby_2(400);
  jby_1(400);
  _delay_cycles(3000);
  jby_6(800);
  _delay_cycles(3000);
  jby_3(400);
  jby_2(400);
  jby_2(400);
  _delay_cycles(3000);
  jby_3(400);
  jby_2(400);
  jby_1(400);
  jby_6(400);
  _delay_cycles(3000);
  jby_1(800);
  _delay_cycles(3000);
  jby_1(400);
  jby_2(400);
  jby_3(400);
  jby_4(400);
  _delay_cycles(3000);
  jby_5(400);
  jby_3(400);
  jby_3(400);
  _delay_cycles(3000);
  jby_2(400);
  jby_2(400);
  jby_1(400);
  jby_2(400);
  _delay_cycles(3000);
  jby_1(400);
  jby_6(400);
  jby_1(400);
  jby_2(400);
  _delay_cycles(3000);
  jby_3(800);
  _delay_cycles(3000);
  jby_1(400);
  jby_6(400);
  jby_1(400);
  _delay_cycles(3000);
  jby_5(400);
  jby_5(400);
  jby_2(400);
  jby_1(400);
  _delay_cycles(3000);
  jby_3(400);
  jby_2(400);
  jby_2(400);
  jby_1(400);
  _delay_cycles(3000);
  jby_2(400);
  jby_1(400);
  jby_2(400);
  _delay_cycles(3000);
  jby_5(400);
  jby_3(400);
  jby_3(400);
  _delay_cycles(3000);
  jby_2(400);
  jby_2(400);
  jby_1(400);
  jby_2(400);
  _delay_cycles(3000);
  jby_1(400);
  jby_6(400);
  jby_1(400);
  jby_5(400);
  _delay_cycles(3000);
  jby_3(800);
  _delay_cycles(3000);
  jby_1(400);
  jby_6(400);
  jby_1(400);
  _delay_cycles(3000);
  jby_5(400);
  jby_5(400);
  jby_2(400);
  jby_1(400);
  _delay_cycles(3000);
  jby_3(400);
  jby_2(400);
  jby_1(400);
  jby_6(400);
  _delay_cycles(3000);
  jby_1(800);
}