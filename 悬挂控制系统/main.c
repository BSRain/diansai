#include <msp430f5529.h>
#include <math.h>
#include <intrinsics.h>
#include <stdint.h>
#include "control.h"
#include "keys.h"
#include "main.h"
#include "oled.h"
#define PI acos(-1)
#define CPU_F ((double)1035000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define _delay_cycles __delay_cycles

char datax[2];
char datay[2];
double zerox=120.9339;
double zeroy=153.0523;
double angle=0;
int main(void)
{
  volatile unsigned int i;                  //局部变量
  //设置timerA相关参数
  WDTCTL = WDTPW+WDTHOLD;                   // 关闭看门狗
  
  
  /*P1DIR |= BIT2;                            // 将P1.2设置为输出端口，验证状态
  P2DIR |= BIT0;                            //P2.0输出PWM波
  TA0CCTL1 = OUTMOD_7;                         //将输出模式选择为模式7
  TA0CTL = TASSEL_1 + MC_1 + TACLR ;         //选择ACLK时钟源，timerA计数模式选择up模式，计数到TA0CCR0，重置timerA计数器
  TA1CCTL1 = OUTMOD_7;
  TA1CTL = TASSEL_1 + MC_1 +TACLR ;
  
  P1REN |= BIT1;                            // 使能端口1.1
  P1OUT |= BIT1;                            // 选择下拉模式
  P1IES |= BIT1;                            // 1.1端口中断触发沿选择下降沿，高到低触发
  P1IFG &= ~BIT1;                           // 1.1端口中断寄存器清零
  P1IE |= BIT1;                       // 1.1端口中断使能
  */
  keyboard_init();
  init_one();
  init_two();
  OLED_Init();
  OLED_Clear();
  __bis_SR_register(GIE);                     //将GIE置1，打开全局中断使能
  for(;;)                                    
  {
    char c;
    OLED_ShowString(0,0,"Mode:",16);
    OLED_ShowString(0,2,"X:",16);
    OLED_ShowString(0,4,"Y:",16);
    OLED_ShowNum(42,0,flag,1,16);
    c=scanKey();
    if(c=='M')
    { 
      flag+=1;
      OLED_ShowNum(42,0,flag,1,16);
    }
    while(flag==1)
    {
      c=scanKey();
      if(c=='B')//启动
      {
        double a1=sqrt((x+15)*(x+15)+(120-y)*(120-y));
        double b1=sqrt((95-x)*(95-x)+(120-y)*(120-y));
        double n1=a1-zerox,n2=b1-zeroy;
        int n1flag=1;
        int n2flag=1;
        if(n1<0) {n1=-n1;n1flag=0;}
        if(n2<0) {n2=-n2;n2flag=0;}
        double maxx;
        if(n1>n2) 
        {
          if(x>=40)
            n1*=1.12;
          else
            n1*=1.090;
          if(y>=50)
            n2*=1.125;
          else
            n2*=1.12;
          maxx=n1;
        }
        else 
        { 
          if(x>=40)
            n1*=1.12;
          else
            n1*=1.085;
          if(y>=50)
            n2*=1.125;
          else
            n2*=1.12;
          maxx=n2;
        }
        n1*=10;
        n2*=10;
        maxx*=10;
        for(int i=0;i<=maxx;i++)
        { 
          if(i<=n1&&n1flag==1)
            cycle2_min_s(200);
          else if(i<=n1&&n1flag==0)
            cycle2_min_f(200);
          if(i<=n2&&n2flag==1)
            cycle1_min_f(200);
          else if(i<=n2&&n2flag==0)
            cycle1_min_s(200);
        }
      }
      else if(c=='M')
      {
        flag+=1;
        OLED_ShowNum(42,0,flag,1,16);
        break;
      }
    }    
    while(flag==2)//模式2
    {
      c=scanKey();
      if(c=='2')
      {
        up(200,200);
      }
      else if(c=='8')
      {
        down(200,200);
      }
      else if(c=='4')
      {
        left(200,200);
      }
      else if(c=='6')
      {
        right(200,200);
      }
      else if(c=='M')
      {
        flag+=1;
        OLED_ShowNum(42,0,flag,1,16);
        break;
      }
      
    }
    while(flag==3)
    {
      c=scanKey();
      delay_ms(35);
      if(c=='X')
      {
        delay_ms(35);
        datax[0]=scanKey();
        delay_ms(35);
        datax[1]=scanKey();
        x=(datax[0]-'0')*10+(datax[1]-'0');
        OLED_ShowNum(16,2,x,2,16);
      }
      else if(c=='Y')
      {
        delay_ms(35);
        datay[0]=scanKey();
        delay_ms(35);
        datay[1]=scanKey();
        delay_ms(35);
        //datay[2]=scanKey();
        //delay_ms(35);
        y=(datay[0]-'0')*10+(datay[1]-'0');
        OLED_ShowNum(16,4,y,2,16);
      }
      else if(c=='B')//启动
      {
        double a1=sqrt((x+15)*(x+15)+(120-y)*(120-y));
        double b1=sqrt((95-x)*(95-x)+(120-y)*(120-y));
        double n1=a1-zerox,n2=b1-zeroy;
        int n1flag=1;
        int n2flag=1;
        if(n1<0) {n1=-n1;n1flag=0;}
        if(n2<0) {n2=-n2;n2flag=0;}
        double maxx;
        if(n1>n2) 
        {
          if(x>=40)
            n1*=1.1;
          else
            n1*=1.085;
          if(y>=50)
            n2*=1.125;
          else
            n2*=1.12;
          maxx=n1;
        }
        else 
        { 
          if(x>=40)
            n1*=1.1;
          else
            n1*=1.085;
          if(y>=50)
            n2*=1.125;
          else
            n2*=1.12;
          maxx=n2;
        }
        n1*=10;
        n2*=10;
        maxx*=10;
        for(int i=0;i<=maxx;i++)
        { 
          if(i<=n1&&n1flag==1)
            cycle2_min_f(200);
          else if(i<=n1&&n1flag==0)
            cycle2_min_s(200);
          if(i<=n2&&n2flag==1)
            cycle1_min_s(200);
          else if(i<=n2&&n2flag==0)
            cycle1_min_f(200);
        }
        
        
      }
      else if(c=='M')
      {
        flag+=1;
        OLED_ShowNum(42,0,flag,1,16);
        break;
      }
    }
    while(flag==4)//模式4
    {
      c=scanKey();//获取键值
      if(c=='X')
      {
        delay_ms(35);
        datax[0]=scanKey();
        delay_ms(35);
        datax[1]=scanKey();
        x=(datax[0]-'0')*10+(datax[1]-'0');
        OLED_ShowNum(16,2,x,2,16);
      }
      else if(c=='Y')
      {
        delay_ms(35);
        datay[0]=scanKey();
        delay_ms(35);
        datay[1]=scanKey();
        delay_ms(35);
        
        y=(datay[0]-'0')*10+(datay[1]-'0');
        OLED_ShowNum(16,4,y,2,16);
      }
      else if(c=='B')//启动
      {
        double lastx=x+25,lasty=y;//保存上一个点的坐标
        while(angle<=360)
        {
          double xx=sqrt((lastx+15)*(lastx+15)+(120-lasty)*(120-lasty));//上一个点的
          double yy=sqrt((95-lastx)*(95-lastx)+(120-lasty)*(120-lasty));
          
          double ny=y+25*sin(angle*PI/180);//下一个点的
          double nx=x+25*cos(angle*PI/180);
          lastx=nx,lasty=ny;//更新上一个点
          double a1=sqrt((nx+15)*(nx+15)+(120-ny)*(120-ny));
          double b1=sqrt((95-nx)*(95-nx)+(120-ny)*(120-ny));
          double n1=a1-xx,n2=b1-yy;
          int n1flag=1;
          int n2flag=1;
          if(n1<0) {n1=-n1;n1flag=0;}
          if(n2<0) {n2=-n2;n2flag=0;}
          double maxx;
          if(n1>n2) 
          {
            //if(x>=40)
            //n1*=1.1;
            //else
            //n1*=1.085;
            //if(y>=50)
            //n2*=1.125;
            //else
            //n2*=1.12;
            maxx=n1;
          }
          else 
          { 
            //if(x>=40)
            //n1*=1.1;
            //else
            //n1*=1.085;
            //if(y>=50)
            //n2*=1.125;
            //else
            //n2*=1.12;
            maxx=n2;
          }
          n1*=10;
          n2*=10;
          maxx*=10;
          for(double i=0.0;i<=maxx;i+=1)
          { 
            if(i<=n1&&n1flag==1)
              cycle2_min_f(200);
            else if(i<=n1&&n1flag==0)
              cycle2_min_s(200);
            if(i<=n2&&n2flag==1)
              cycle1_min_s(200);
            else if(i<=n2&&n2flag==0)
              cycle1_min_f(200);
          }
          angle+=5;//前往下一个点
        }
        angle=0;
      }
      else if(c=='M')
      {
        flag+=1;
        OLED_ShowNum(42,0,flag,1,16);
        break;
      }
    }
    if(flag>4)
    {
      flag=0;
      OLED_ShowNum(42,0,flag,1,16);
    }
    
  }
}

//采用S1中断按键实现
/*#pragma vector=PORT1_VECTOR                //端口p1的中断函数
__interrupt void Port_1(void)
{
unsigned int push_key =0;
push_key == P1IFG & (P1IN);
delay_ms(15);                  //精确时间延迟，乘以MCLK时钟周期,防止抖动
if( push_key == (P1IN & BIT1))
{
__low_power_mode_off_on_exit();        //SR寄存器置0，退出低功耗
sum=sum+1;                               // 按下p1.1，sum加1，周期变化一次
    }
P1IFG &= ~BIT1;                          // p1.1中断重置

}*/