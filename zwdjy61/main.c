#include <msp430.h>
#include "uart.h"
unsigned char DY[1],EY[1];
unsigned int A1,A2,A3;
unsigned char Re_buf[11],counter;
unsigned char sign;
float a[3],w[3],T;
int angle[3];

void data()
{
    if(sign)
    {
        sign=0;
        if(Re_buf[0]==0x55) //?????
        {
            switch(Re_buf [1])
            {
                case 0x51: //加速度
                a[0] = (((short)Re_buf[3]<<8)|Re_buf[2])/32768.0*16.0;
                a[1] = (((short)Re_buf[5]<<8)|Re_buf[4])/32768.0*16.0;
                a[2] = (((short)Re_buf[7]<<8)|Re_buf[6])/32768.0*16.0;
                T = (((short)Re_buf[9]<<8)|Re_buf[8])/340.0+36.53;
                break;
                case 0x52: //角加速度
                w[0] = (((short)Re_buf[3]<<8)|Re_buf[2])/32768.0*2000;
                w[1] = (((short)Re_buf[5]<<8)|Re_buf[4])/32768.0*2000;
                w[2] = (((short)Re_buf[7]<<8)|Re_buf[6])/32768.0*2000;
                T = (((short)Re_buf[9]<<8)|Re_buf[8])/340.0+36.53;
                break;
                case 0x53: //角度
                angle[0] = (((short)Re_buf[3]<<8)|Re_buf[2])/32768.0*180;
                angle[1] = (((short)Re_buf[5]<<8)|Re_buf[4])/32768.0*180;
                angle[2] = (((short)Re_buf[7]<<8)|Re_buf[6])/32768.0*180;
                T = (((short)Re_buf[9]<<8)|Re_buf[8])/340.0+36.53;
                break;
            }
        }
    }

}
 

int main(void)
{  
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Uart_Init();
	    _EINT();                            //使能中断
	while(1)
	{
         data();
	}
}






