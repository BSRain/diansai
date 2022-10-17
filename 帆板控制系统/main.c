#include "MSP430G2553.h"
#include "stdint.h"
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

#define rw_out P2DIR|=BIT0
#define rw_h P2OUT|=BIT0
#define rw_l P2OUT&=~BIT0
#define en_out P2DIR|=BIT2
#define en_h P2OUT|=BIT2
#define en_l P2OUT&=~BIT2

/*函数声明*/
void delay_1ms();
void delay_nms();
void PortConfig(void);
uint Strlen(uchar *p);
void WriteBytes(uchar *p);
void SerialWriteData(uchar send);
void SerialWriteCmd(uchar send);
void init_lcd();
void display_12864(uchar hang, uchar lie, uchar *p);
void Write_CGRAM(uchar *p,uchar cg,uchar y,uchar x);
 
void PortConfig(void)
{
/*引脚初始化*/
 P1DIR = 0xFF;
 P1SEL = 0x00;
}


uint Strlen(uchar *p)
{
/*返回字符串长度*/
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
   delay_nms(1);
   en_l;
   temp<<=1;
  }
}
rw_h;
}
void SerialWriteData(uchar send)
{
uchar data[3];
data[0]=0xfa ;     //1111 1010向液晶写数据
data[1]=send & 0xf0;      //取高4位
data[2]=(send<<4)&0xf0;       //取低4位
WriteBytes(data);
}
void SerialWriteCmd(uchar send)
{
uchar data[3];
data[0]=0xf8 ;                    //1111 1000向液晶写命令
data[1]=send & 0xf0;      //取高4位
data[2]=(send<<4) & 0xf0;       //取低4位
WriteBytes(data); 
}

void init_lcd()
{
 SerialWriteCmd(0x30);
 SerialWriteCmd(0x30);
 SerialWriteCmd(0x02);
 SerialWriteCmd(0x0C); //显示开，关闭游标和游标位置
 SerialWriteCmd(0x06);
 SerialWriteCmd(0x01); //清除显示，等待命令
}

void display_12864(uchar hang, uchar lie, uchar *p)
{
/*输入行,列,字符串并显示*/
// uchar i;
 switch (hang)
 {
 case 1:
 SerialWriteCmd(0x80 + lie);
 break;
 case 2:
 SerialWriteCmd(0x90 + lie);
 break;
 case 3:
 SerialWriteCmd(0x88 + lie);
 break;
 case 4:
 SerialWriteCmd(0x98 + lie);
 break;
 }
           while(*p!='\0')
          {
            SerialWriteData(*p);
                p++;
            }
}
 
void Write_CGRAM(uchar *p,uchar cg,uchar y,uchar x)
{
 /*这个是自定义字符显示*/
 /**p为自定义字符数组,大小为32*/
 /*cg为放入的CGRAM地址,共4个可选*/
 /*y为显示的行*/
 /*x为显示的列*/
 switch (cg)
 {
 case 1:
 SerialWriteCmd(0x40);
 break;
 case 2:
 SerialWriteCmd(0x50);
 break;
 case 3:
 SerialWriteCmd(0x60);
 break;
 case 4:
 SerialWriteCmd(0x70);
 break;
 }
 for (unsigned char i = 0; i <32; i++)
 {
 SerialWriteData(p[i]);
 } 
 switch (y)
 {
 case 1:
 SerialWriteCmd(0x80 + x);
 break;
 case 2:
 SerialWriteCmd(0x90 + x);
 break;
 case 3:
 SerialWriteCmd(0x88 + x);
 break;
 case 4:
 SerialWriteCmd(0x98 + x);
 break;
 }
 SerialWriteData(0x00); 
 switch (cg)
 {
 case 1:
 SerialWriteData(0x00);
 break;
 case 2:
 SerialWriteData(0x02);
 break;
 case 3:
 SerialWriteData(0x04);
 break;
 case 4:
 SerialWriteData(0x06);
 break;
 }
}
void delay_1ms(void)
{
        unsigned int i;
        for (i=0;i<1140;i++);
}

void delay_nms(unsigned int n)
{
        unsigned int i=0;
        for (i=0;i<n;i++)
                delay_1ms();
}
double increment_angle;
/*
 * @fn:		void InitSystemClock(void)
 * @brief:	初始化系统时钟
 * @para:	none
 * @return:	none
 * @comment:初始化系统时钟
 */
void InitSystemClock(void)
{
	/*配置DCO为1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*配置SMCLK的时钟源为DCO*/
    BCSCTL2 &= ~SELS;
    /*SMCLK的分频系数置为1*/
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}
/*
 * @fn:		void InitUART(void)
 * @brief:	初始化串口，包括设置波特率，数据位，校验位等
 * @para:	none
 * @return:	none
 * @comment:初始化串口
 */
void InitUART(void)
{
    /*复位USCI_Ax*/
    UCA0CTL1 |= UCSWRST;

    /*选择USCI_Ax为UART模式*/
    UCA0CTL0 &= ~UCSYNC;

    /*配置UART时钟源为SMCLK*/
    UCA0CTL1 |= UCSSEL1;

    /*配置波特率为9600@1MHz*/
    UCA0BR0 = 0x68;
    UCA0BR1 = 0x00;
    UCA0MCTL = 1 << 1;
    /*使能端口复用*/
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    /*清除复位位，使能UART*/
    UCA0CTL1 &= ~UCSWRST;
}
/*
 * @fn:		void UARTSendString(uint8_t *pbuff,uint8_t num)
 * @brief:	通过串口发送字符串
 * @para:	pbuff:指向要发送字符串的指针
 * 			num:要发送的字符个数
 * @return:	none
 * @comment:通过串口发送字符串
 */
void UARTSendString(uint8_t *pbuff,uint8_t num)
{
	uint8_t cnt = 0;
	for(cnt = 0;cnt < num;cnt ++)
	{
		while(UCA0STAT & UCBUSY);
		UCA0TXBUF = *(pbuff + cnt);
	}
}
/*
 * @fn:		void PrintNumber(uint16_t num)
 * @brief:	通过串口发送数字
 * @para:	num:变量
 * @return:	none
 * @comment:通过串口发送数字
 */
void PrintNumber(uint16_t num)
{
	uint8_t buff[6] = {0,0,0,0,0,'\n'};
	uint8_t cnt = 0;
	for(cnt = 0;cnt < 5;cnt ++)
	{
		buff[4 - cnt] = (uint8_t)(num % 10 + '0');
		num /= 10;
	}
	UARTSendString(buff,6);
}
/*
 * @fn:		void PrintFloat(float num)
 * @brief:	通过串口发送浮点数
 * @para:	num:变量
 * @return:	none
 * @comment:通过串口发送浮点数，可发送1位整数位+3位小数位
 */
void PrintFloat(float num)
{
	uint8_t charbuff[] = {0,'.',0,0,0};
	uint16_t temp = (uint16_t)(num * 1000);
	charbuff[0] = (uint8_t)(temp / 1000) + '0';
	charbuff[2] = (uint8_t)((temp % 1000) / 100) + '0';
	charbuff[3] = (uint8_t)((temp % 100) / 10) + '0';
	charbuff[4] = (uint8_t)(temp % 10) + '0';
	UARTSendString(charbuff,5);
}
/*
 * @fn:		void InitADC(void)
 * @brief:	初始化ADC
 * @para:	none
 * @return:	none
 * @comment:初始化ADC
 */
void InitADC(void)
{
	  /*设置ADC时钟MCLK*/
	  ADC10CTL1 |= ADC10SSEL_2;
	  /*ADC 2分频*/
	  ADC10CTL1 |= ADC10DIV_0;
	  /*设置ADC基准源*/
	  ADC10CTL0 |= SREF_1;
	  /*设置ADC采样保持时间64CLK*/
	  ADC10CTL0 |= ADC10SHT_3;
	  /*设置ADC采样率200k*/
	  ADC10CTL0 &= ~ADC10SR;
	  /*ADC基准选择2.5V*/
	  ADC10CTL0 |= REF2_5V;
	  /*开启基准*/
	  ADC10CTL0 |= REFON;
	  /*选择ADC输入通道A0*/
	  ADC10CTL1 |= INCH_0;
	  /*允许A0模拟输入*/
	  ADC10AE0 |= 0x0001;
	  /*开启ADC*/
	  ADC10CTL0 |= ADC10ON;
}
/*
 * @fn:		uint16_t GetADCValue(void)
 * @brief:	进行一次ADC转换并返回ADC转换结果
 * @para:	none
 * @return:	ADC转换结果
 * @comment:ADC转换结果为10bit，以uint16_t类型返回，低10位为有效数据
 */
uint16_t GetADCValue(void)
{
	  /*开始转换*/
	  ADC10CTL0 |= ADC10SC|ENC;
	  /*等待转换完成*/
	  while(ADC10CTL1&ADC10BUSY);
	  /*返回结果*/
	  return ADC10MEM;
}

/*void PWM
*/

struct 
{
    float set_angle;//设定速度 
    float actual_angle;//实际速度
    float error;//偏差  
    float error_next;//上一个偏差  
    float error_last;//上上一个偏差 
	float error_add; 
    float kp,ki,kd;//定义比例，积分，微分参数  
    float umax,umin; 
}pid;
void pid_init()
{
    pid.set_angle = 0.0;
    pid.actual_angle = 0.0;
    pid.error = 0.0;
    pid.error_next = 0.0;
    pid.error_last = 0.0;
	pid.error_add=0.0;
	pid.umax=70;
    pid.kp = 0.15;
    pid.ki = 1.37;
    pid.kd = 1.97;
}
void pid_realise(float angle)//实现pid  
{
	int gex;
    pid.set_angle = angle;//设置目标速度  
    pid.error = pid.set_angle - pid.actual_angle;	  //设定-实际 
	if(pid.actual_angle>pid.umax)
	{
		if(pid.error>30)
		{
			gex=0;	
		}
		else
		{
			gex=1;
			if(pid.error<0)
			{
				pid.error_add+=pid.error;
			}
		}
	} 
    increment_angle = pid.kp*pid.error+gex*pid.ki*pid.error_add+pid.kd*(pid.error-pid.error_next);//增量计算公式  
 
    pid.error_next = pid.error;
  
}
//PWM波使用
/*void Set(int S){

TA1CTL |= TASSEL1;
TA1CTL |= MC_1;
TA1CCR0 = 20000;
TA1CCR1 = S;
TA1CCTL0 &= ~CAP;
TA1CCTL1 &= ~CAP;

TA1CCTL1 |= OUTMOD_6;
//端口复用
P2SEL |= BIT1;
P2DIR |= BIT1;

P1DIR |= BIT6+BIT7;
P1OUT |= BIT6;
P1OUT &= ~BIT7;
}*/     
从v不过你还没呢,./int main( void )
{
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  InitSystemClock();
  InitUART();
  InitADC();
  pid_init();
  GetADCValue();

	/*配置DCO频率为1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*初始化P1.6为输入*/
    P1DIR &= ~BIT6;

    /*设置时钟源为SMCLK*/
    TA1CTL |= TASSEL1;
    /*设置工作模式为Up&Down*/
    TA1CTL |= MC0|MC1;
    /*设置TA1CCR0为0x00FF*/
    TA1CCR0 = 0x00FF;
    /*设置TA1CCR2为0x00FF*/
    TA1CCR2 = 0x00FF;//占空比(TACCR0 - TACCR2) / TACCR0,频率=SMCLK/(TACCR0+1)/2
    /*设置为比较模式*/
    TA1CCTL0 &= ~CAP;
    TA1CCTL2 &= ~CAP;
    /*设置比较输出模式*/
    TA1CCTL2 |= OUTMOD_6;
    /*设置IO复用*/
    P2SEL |= BIT5;
    P2DIR |= BIT5;
  
  while(1)
  {
        float voltage = 0;
	uint16_t adcvalue = 0;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    InitSystemClock();
    InitUART();
    InitADC();
    while(1)
    {
    	adcvalue = GetADCValue();
    	voltage = adcvalue * 2.5 / 1023;
    	UARTSendString("ADC10转换结果为：",17);
    	PrintNumber(adcvalue);
    	UARTSendString("相应电压值为：",14);
    	PrintFloat(voltage);
    	__delay_cycles(300000);
        init_lcd();
        display_12864(1,0,"角度:(adcvalue-870)*20/10  ");

    }

  }
}
