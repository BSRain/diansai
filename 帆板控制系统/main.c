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

/*��������*/
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
/*���ų�ʼ��*/
 P1DIR = 0xFF;
 P1SEL = 0x00;
}


uint Strlen(uchar *p)
{
/*�����ַ�������*/
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
data[0]=0xfa ;     //1111 1010��Һ��д����
data[1]=send & 0xf0;      //ȡ��4λ
data[2]=(send<<4)&0xf0;       //ȡ��4λ
WriteBytes(data);
}
void SerialWriteCmd(uchar send)
{
uchar data[3];
data[0]=0xf8 ;                    //1111 1000��Һ��д����
data[1]=send & 0xf0;      //ȡ��4λ
data[2]=(send<<4) & 0xf0;       //ȡ��4λ
WriteBytes(data); 
}

void init_lcd()
{
 SerialWriteCmd(0x30);
 SerialWriteCmd(0x30);
 SerialWriteCmd(0x02);
 SerialWriteCmd(0x0C); //��ʾ�����ر��α���α�λ��
 SerialWriteCmd(0x06);
 SerialWriteCmd(0x01); //�����ʾ���ȴ�����
}

void display_12864(uchar hang, uchar lie, uchar *p)
{
/*������,��,�ַ�������ʾ*/
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
 /*������Զ����ַ���ʾ*/
 /**pΪ�Զ����ַ�����,��СΪ32*/
 /*cgΪ�����CGRAM��ַ,��4����ѡ*/
 /*yΪ��ʾ����*/
 /*xΪ��ʾ����*/
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
 * @brief:	��ʼ��ϵͳʱ��
 * @para:	none
 * @return:	none
 * @comment:��ʼ��ϵͳʱ��
 */
void InitSystemClock(void)
{
	/*����DCOΪ1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*����SMCLK��ʱ��ԴΪDCO*/
    BCSCTL2 &= ~SELS;
    /*SMCLK�ķ�Ƶϵ����Ϊ1*/
    BCSCTL2 &= ~(DIVS0 | DIVS1);
}
/*
 * @fn:		void InitUART(void)
 * @brief:	��ʼ�����ڣ��������ò����ʣ�����λ��У��λ��
 * @para:	none
 * @return:	none
 * @comment:��ʼ������
 */
void InitUART(void)
{
    /*��λUSCI_Ax*/
    UCA0CTL1 |= UCSWRST;

    /*ѡ��USCI_AxΪUARTģʽ*/
    UCA0CTL0 &= ~UCSYNC;

    /*����UARTʱ��ԴΪSMCLK*/
    UCA0CTL1 |= UCSSEL1;

    /*���ò�����Ϊ9600@1MHz*/
    UCA0BR0 = 0x68;
    UCA0BR1 = 0x00;
    UCA0MCTL = 1 << 1;
    /*ʹ�ܶ˿ڸ���*/
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    /*�����λλ��ʹ��UART*/
    UCA0CTL1 &= ~UCSWRST;
}
/*
 * @fn:		void UARTSendString(uint8_t *pbuff,uint8_t num)
 * @brief:	ͨ�����ڷ����ַ���
 * @para:	pbuff:ָ��Ҫ�����ַ�����ָ��
 * 			num:Ҫ���͵��ַ�����
 * @return:	none
 * @comment:ͨ�����ڷ����ַ���
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
 * @brief:	ͨ�����ڷ�������
 * @para:	num:����
 * @return:	none
 * @comment:ͨ�����ڷ�������
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
 * @brief:	ͨ�����ڷ��͸�����
 * @para:	num:����
 * @return:	none
 * @comment:ͨ�����ڷ��͸��������ɷ���1λ����λ+3λС��λ
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
 * @brief:	��ʼ��ADC
 * @para:	none
 * @return:	none
 * @comment:��ʼ��ADC
 */
void InitADC(void)
{
	  /*����ADCʱ��MCLK*/
	  ADC10CTL1 |= ADC10SSEL_2;
	  /*ADC 2��Ƶ*/
	  ADC10CTL1 |= ADC10DIV_0;
	  /*����ADC��׼Դ*/
	  ADC10CTL0 |= SREF_1;
	  /*����ADC��������ʱ��64CLK*/
	  ADC10CTL0 |= ADC10SHT_3;
	  /*����ADC������200k*/
	  ADC10CTL0 &= ~ADC10SR;
	  /*ADC��׼ѡ��2.5V*/
	  ADC10CTL0 |= REF2_5V;
	  /*������׼*/
	  ADC10CTL0 |= REFON;
	  /*ѡ��ADC����ͨ��A0*/
	  ADC10CTL1 |= INCH_0;
	  /*����A0ģ������*/
	  ADC10AE0 |= 0x0001;
	  /*����ADC*/
	  ADC10CTL0 |= ADC10ON;
}
/*
 * @fn:		uint16_t GetADCValue(void)
 * @brief:	����һ��ADCת��������ADCת�����
 * @para:	none
 * @return:	ADCת�����
 * @comment:ADCת�����Ϊ10bit����uint16_t���ͷ��أ���10λΪ��Ч����
 */
uint16_t GetADCValue(void)
{
	  /*��ʼת��*/
	  ADC10CTL0 |= ADC10SC|ENC;
	  /*�ȴ�ת�����*/
	  while(ADC10CTL1&ADC10BUSY);
	  /*���ؽ��*/
	  return ADC10MEM;
}

/*void PWM
*/

struct 
{
    float set_angle;//�趨�ٶ� 
    float actual_angle;//ʵ���ٶ�
    float error;//ƫ��  
    float error_next;//��һ��ƫ��  
    float error_last;//����һ��ƫ�� 
	float error_add; 
    float kp,ki,kd;//������������֣�΢�ֲ���  
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
void pid_realise(float angle)//ʵ��pid  
{
	int gex;
    pid.set_angle = angle;//����Ŀ���ٶ�  
    pid.error = pid.set_angle - pid.actual_angle;	  //�趨-ʵ�� 
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
    increment_angle = pid.kp*pid.error+gex*pid.ki*pid.error_add+pid.kd*(pid.error-pid.error_next);//�������㹫ʽ  
 
    pid.error_next = pid.error;
  
}
//PWM��ʹ��
/*void Set(int S){

TA1CTL |= TASSEL1;
TA1CTL |= MC_1;
TA1CCR0 = 20000;
TA1CCR1 = S;
TA1CCTL0 &= ~CAP;
TA1CCTL1 &= ~CAP;

TA1CCTL1 |= OUTMOD_6;
//�˿ڸ���
P2SEL |= BIT1;
P2DIR |= BIT1;

P1DIR |= BIT6+BIT7;
P1OUT |= BIT6;
P1OUT &= ~BIT7;
}*/     
��v�����㻹û��,./int main( void )
{
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  InitSystemClock();
  InitUART();
  InitADC();
  pid_init();
  GetADCValue();

	/*����DCOƵ��Ϊ1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    /*��ʼ��P1.6Ϊ����*/
    P1DIR &= ~BIT6;

    /*����ʱ��ԴΪSMCLK*/
    TA1CTL |= TASSEL1;
    /*���ù���ģʽΪUp&Down*/
    TA1CTL |= MC0|MC1;
    /*����TA1CCR0Ϊ0x00FF*/
    TA1CCR0 = 0x00FF;
    /*����TA1CCR2Ϊ0x00FF*/
    TA1CCR2 = 0x00FF;//ռ�ձ�(TACCR0 - TACCR2) / TACCR0,Ƶ��=SMCLK/(TACCR0+1)/2
    /*����Ϊ�Ƚ�ģʽ*/
    TA1CCTL0 &= ~CAP;
    TA1CCTL2 &= ~CAP;
    /*���ñȽ����ģʽ*/
    TA1CCTL2 |= OUTMOD_6;
    /*����IO����*/
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
    	UARTSendString("ADC10ת�����Ϊ��",17);
    	PrintNumber(adcvalue);
    	UARTSendString("��Ӧ��ѹֵΪ��",14);
    	PrintFloat(voltage);
    	__delay_cycles(300000);
        init_lcd();
        display_12864(1,0,"�Ƕ�:(adcvalue-870)*20/10  ");

    }

  }
}
