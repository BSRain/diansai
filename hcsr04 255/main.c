//Connections
//P2.1 ECHO
//P1.6 TRIGGER yellow
#include <msp430.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
// Timers in MSP430 by drselim
// Plese don't forget to give credits while sharing this code
// for the video description for the code:
// https://youtu.be/LzOVfDQaol8
char printdist[] = "Distance to the nearest object is: ";
char centimeter[] = " cm";
char dot[] = ".";
char zerro[] = "0";
char newline[] = " \r\n";
volatile int temp[2];
volatile float diff;
volatile unsigned int i=0;
int dst_int;
int dst_flt;
float tmp_flt;
char dst_char[5];
char dst_flt_char[5];
volatile float distance;
void ser_output(char *str);

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

void main(void)
// Timers in MSP430 by drselim
// Plese don't forget to give credits while sharing this code
// for the video description for the code:
// https://youtu.be/LzOVfDQaol8
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1= CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    P1DIR = BIT6;  
    P2SEL = BIT1; 
    P1SEL = BIT1|BIT2|BIT6; 
    P1SEL2 = BIT1|BIT2;
    UCA0CTL1 |= UCSWRST+UCSSEL_2; 
    UCA0BR0 = 52;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;  
    TA0CTL = TASSEL_2|MC_1 ;  
    TA0CCR0 = 0xFFFF;  
    TA0CCR1 = 0x000A;  
    TA0CCTL1 = OUTMOD_7; 
    TA1CTL = TASSEL_2|MC_2 ; 
    TA1CCTL1 = CAP | CCIE | CCIS_0 | CM_3 | SCS ;  
     __enable_interrupt();
    while(1){
        distance = diff/58;
        dst_int = floor(distance);
        tmp_flt = distance - dst_int;
        //if you get an error with ltoa, try: 'ltoa(dst_int, dst_char,10)'
        //check article:
        //https://software-dl.ti.com/ccs/esd/documents/sdto_cgt_handling_changes_in_ltoa.html
        itoa(dst_int, dst_char,10);
        if (tmp_flt < 0.01) {
        dst_flt = floor(tmp_flt * 1000);
        //if you get an error with ltoa, try: 'ltoa(dst_flt,dst_flt_char,10)'
        //check article:
        //https://software-dl.ti.com/ccs/esd/documents/sdto_cgt_handling_changes_in_ltoa.html
        itoa(dst_flt,dst_flt_char,10);
        ser_output(printdist); ser_output(dst_char); ser_output(dot); ser_output(zerro); ser_output(zerro); ser_output(dst_flt_char); ser_output(centimeter);
        }
        else if (tmp_flt < 0.1) {
            dst_flt = floor(tmp_flt * 100);
            itoa(dst_flt,dst_flt_char,10);
            ser_output(printdist); ser_output(dst_char); ser_output(dot); ser_output(zerro); ser_output(dst_flt_char); ser_output(centimeter);
        }
        else {
            dst_flt = floor(tmp_flt * 100);
            itoa(dst_flt,dst_flt_char,10);
            ser_output(printdist); ser_output(dst_char); ser_output(dot); ser_output(dst_flt_char); ser_output(centimeter);
        }
        ser_output(newline);
        __delay_cycles(500000);  //0.5 second delay
    }
}
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer_A(void){
        temp[i] = TA1CCR1;
        i += 1;
        TA1CCTL1 &= ~CCIFG ;
        if (i==2) {
            diff=temp[i-1]-temp[i-2];
            i=0;
        }

}
void ser_output(char *str){
    while(*str != 0) {
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
        }
}