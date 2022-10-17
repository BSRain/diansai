#include "MSP430F5529.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void senfchar(char s);
                         // Initialize Flash pointer
int abc(int n);
unsigned char u8_count_10ms;
//����ϵͳʱ��
volatile unsigned int i;
#define SCK_L       P4OUT   &= ~BIT0
#define SCK_H       P4OUT   |= BIT0
#define SDA_IN      (P4IN & BIT1)
#define GapValue    119.7    /* ��ȡ����HX711Ϊһ���Ĵ�����ֵ��λ������g���������GapValueֵ���ǻ����g��λ */
unsigned long Weight_Maopi; /* ëƤ�أ��ϵ��Ͷ�ȡ��������� */
void waming ();        
void stopwaming ();
void TestWeight ();
char * Flash_ptr;    
unsigned long HX711_Buffer=0;
unsigned long maopia=0;
unsigned int Weight_Shiwu=0;
int w=0,aaa=0,ii=0;
unsigned int e=0;
//�����������
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
  FCTL3 = FWKEY;                            //�������
  FCTL1 = FWKEY+WRT;                        //��λ���λ
  *Addr = data;                             //д��һ����
  while (FCTL3 & BUSY);                     //�ȴ��������
  FCTL1 = FWKEY;                            //���д��λ
  FCTL3 = FWKEY+LOCK;                       //��FLASH�������ɶ�������д�����ɲ�������������
}


__ramfunc void Flash_Erase_Segment()//(uint16_t segmentNo)
{
// ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Erase_Segment","segmentNo����������Χ��");  //���Լ��η�Χ�Ƿ���FLASH����

//  uint16_t * Addr = (uint16_t *)((uint32_t)segmentNo*512);   //�����ַ C��0X1800 ��D�� 0X1880;
  char * Addr;                         // Initialize Flash pointer
  Addr = (char *) 0x1880;

  FCTL3 = FWKEY;                              // �������
  FCTL1 = FWKEY+ERASE;                        // ����һ��
  *Addr = 0;                                  //�Բ����ĵ�ַ��Χ��������һ�ο�д�룬����������������
  while (FCTL3 & BUSY);                       // �ȴ��������
  FCTL1 = FWKEY;                              //�������λ
  FCTL3 = FWKEY+LOCK;                         //��FLASH�������ɶ�������д�����ɲ���
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
//	//��������
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
//	//С����
//	Flash_Write_Byte('.',FAddr); 
//          FAddr++;
//	
//	f_temp=(int)((num-z_temp)*(abc(f_len)));
//  //С������
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
{//�ж��Ƿ񳬳�����
      if(Weight_Shiwu>500)
           waming ();
else stopwaming ();   
}
void init_hx711port(void)
{//���ų�ʼ��
P4DIR |= BIT0; /* ��Ƭ��P4.0������� */
P4OUT &= ~BIT0; /* ��Ƭ��P4.0��������͵�ƽ */

P4DIR &= ~BIT1; /* ��Ƭ��P4.1�������� */
/* P4REN |= BIT1;   //��Ƭ��P4.1������������ʹ�� F149��Ƭ������û�ڲ����� */
//P4OUT |= BIT1; /* ��Ƭ��P4.1���������������� */
}
unsigned long read_hx711_24bit(void)
{//��ȡHX711�ڲ������� 
unsigned long Count = 0;
unsigned char i;
SCK_L; /* SCL=0;ʹ��AD��PD_SCL �õͣ� */
    while ( SDA_IN)
        ;
    /* ADת��δ������ȴ�������ʼ��ȡ ѭ����ȡ24�� */
    for (i = 0; i < 24; i++)
    {
        SCK_H; /* SCL = 1; PD_SCL �øߣ��������壩 */
        Count = Count << 1; /*�½�����ʱ����Count����һλ���Ҳಹ�� */
      //  delay_us(2);
        SCK_L; /* SCL=0; PD_SCL �õ� */
        if ( SDA_IN)
            Count++; /* if(SDA) Count++; */
      //  delay_us(2);
    }
SCK_H; /* SCL=1; */
Count = Count ^ 0x800000; //��25�������½�����ʱ��ת������
SCK_L; /* SCL=0; */
return (Count);
}
void get_maopi(void)
{//��ȡ��ʼ����
Weight_Maopi = read_hx711_24bit();
Weight_Maopi = read_hx711_24bit(); // ���һ��ȷ���ȶ�
}
unsigned long get_weight(void)
{//��ȡ���� ����g��λ������ 

HX711_Buffer = read_hx711_24bit();
maopia = (unsigned long) ((float) Weight_Maopi / GapValue); //ëƤ����ת����g 
HX711_Buffer = (unsigned long) ((float) HX711_Buffer / GapValue); //��ʱ����ת����g 
    if (HX711_Buffer > maopia)
    {
        Weight_Shiwu = HX711_Buffer - maopia; /* ����ʵ���ʵ������ */
        Weight_Shiwu=Weight_Shiwu/3.4;
        return (Weight_Shiwu); /* ������ȷ����g��λ��������ֵ */
    }
    return (0); /* ���ز����Ϲ淶(HX711_Buffer > Weight_Maopi) ����2 */
}
void waming ()
{//��������
P6DIR |= BIT5;//��������������//�������𱨾�
P6OUT &=~ BIT5;
P3DIR |= BIT0;
P3OUT |= BIT0;
}
void stopwaming ()
{//��������
  P6OUT |= BIT5;
  P3OUT &=~ BIT0;
/*������ֹͣ��������*//*����ֹͣ���𱨾�*/ 
  
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
data[0]=0xFa;//1111?1010��Һ��д����
data[1]=send & 0xF0;//ȡ��4λ
data[2]=(send<<4)&0xF0;//ȡ��4λ
WriteBytes(data);
}
void SerialWriteCmd(uchar send)
{
uchar data[3];
data[0]=0xf8 ;//1111?1000��Һ��д����
data[1]=send & 0xf0;//ȡ��4λ
data[2]=(send<<4) & 0xf0;//ȡ��4λ
WriteBytes(data);
}
void init_lcd()
{
en_out;
rw_out;
SerialWriteCmd(0x0C);//��ʾ�����ر��α���α�λ��
SerialWriteCmd(0x06);
SerialWriteCmd(0x01);//�����ʾ���ȴ�����
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
display_12864(3,0,"�۸�:\0  ");
display_12864(1,0,"����:");
display_12864(1,6,"g");
display_12864(2,0,"����:");
display_12864(2,7,"Ԫ");
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
{//��LCD��ʾ���������� 
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
if(n<1000)//����۸���1000Ԫ���£����ã�Ԫ�����۸���1000Ԫ���ϣ����ã���
{
 e=(uint)(n*100);
  jiage[0]=(uint)(e/10000)+'0';
  jiage[1]=(uint)((e/1000)%10)+'0';
  jiage[2]=(uint)((e/100)%10)+'0';
  jiage[4]=(uint)((e/10)%10)+'0';
  jiage[5]=(uint)((e%10))+'0';
 display_12864(3,7,"Ԫ");  
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
  display_12864(3,7,"��");
  display_12864(3,3,jiage);
}
display_12864(1,3,zhongliang);//�����LCD��Ļ�ϡ�
display_12864(2,3,danjia);
display_12864(3,3,jiage);//�����LCD��Ļ�ϡ�
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
{//�����йܽ�Ϊ����
    
    P3DIR &= ~BIT5; //P3.5�˿���Ϊ����
    P3REN |= BIT5;  //ʹ3.5������/��������
    P3OUT &=~ BIT5;  //��P3.5Ϊ��������ģʽ
    P3SEL &= ~BIT5; //P3.5ΪI/O�ӿ�

    P3DIR &= ~BIT6; //P3.6�˿���Ϊ����
    P3REN |= BIT6;  //ʹP3.6������/��������
    P3OUT &=~ BIT6;  //��P3.6Ϊ��������ģʽ
    P3SEL &= ~BIT6; //P3.6ΪI/O�ӿ�

    P7DIR &= ~BIT0; //P7.0�˿���Ϊ����
    P7REN |= BIT0;  //ʹP7.0������/��������
    P7OUT &=~ BIT0;  //��P7.0Ϊ��������ģʽ
    P7SEL &= ~BIT0; //P7.0ΪI/O�ӿ�

    P6DIR &= ~BIT4; //P6.4�˿���Ϊ����
    P6REN |= BIT4;  //ʹP6.4������/��������
    P6OUT &=~ BIT4;  //��P6.4Ϊ��������ģʽ
    P6SEL &=~ BIT4; //P6.4ΪI/O�ӿ�



    //�����йܽ�Ϊ���
    P6DIR |= BIT3+BIT2+BIT1+BIT0; //P6.3,2,1,0�˿���Ϊ���
    P6OUT &=~(BIT3+BIT2+BIT1+BIT0);

}  
uchar c[2]="00",u='0';
uchar g[7]={'0','0','0','0','0','0','0'};
void kkk()
{
  u=u+1;
   Flash_Write_Byte(u);
   sendstring("\nƻ����");
   c[0]=(uchar)(*Flash_ptr);
   sendstring(c);
   sendstring("Ԫ\n");
}
void hhh()
{
clear_12864();
transmission();
display_12864(3,0,"�۸�:");
display_12864(1,0,"����:");
display_12864(1,6,"g");
display_12864(2,0,"����:");
display_12864(2,7,"Ԫ");


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
{//ɨ����
    
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
     {//���ذ��µ��ַ�
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
unsigned int LastCCR1;		//��һ�θߵ�ƽ������ʱ�䣬�;��������
unsigned int dat[20];		//ͨ�����鱸�ݲɼ���������
 
unsigned char wen[5]={'0','0','.','0','\0'};

unsigned char hc[5] = {0,0,'.',0,'\0'};
 
/*��ʼ��HC_SR04Init()ģ��*/
void HC_SR04Init()
{
	P1DIR|=BIT2;
}
/*��ʼ��������*/
void HC_SR04Start()
{
	P1OUT|=BIT2;		
	__delay_cycles(12);		//���ٳ���10us�ĸߵ�ƽ
	P1OUT&=~BIT2;			
}
 
/*��ʼ������ģʽ����ȡʱ��*/
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
{//������

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
  
	HC_SR04Init();			//��ʼ������ģ��HC_SR04
	TimeGetInit();			//��ʼ�������������ţ�ģʽ��
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
display_12864(2,0,"����:");
display_12864(2,7,"Ԫ");
display_12864(3,0,"�۸�:\0  ");
display_12864(1,0,"����:");
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
  case '0':sendstring("\n���ۣ�\r");
      sendstring(danjia);
      sendstring("Ԫ\n");      
      sendstring("������\r");
      sendstring(zhongliang);
      sendstring("g\n");  
      sendstring("�۸�\r");
      sendstring(jiage);
      if(n<1000)
        sendstring("Ԫ\n");
      else
      sendstring("��\n");   
        sendstring("��ͣ�");
        sendstring(he);
        sendstring("��Ԫ\n");
        senfchar(pp);
        sendstring("\n����");
       sendstring(hc);
       sendstring("cm\n");
       sendstring("�¶�");
       sendstring(wen);
       sendstring("���϶�\n");
       sendstring("ʱ��:");

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
	_DINT();						//���ж�
	static unsigned char times=1;
	static unsigned char i=0;
	unsigned char j;
	unsigned int t;
 
	if(times==1)
	{
		LastCCR1=TA0CCR3;		//��¼���ϴ�CCR3��ֵ
		TA0CCTL3&=~CM_1;		//�������ز���
		TA0CCTL3|=CM_2;			//��Ϊ�½��ز���
		times++;
	}
	if(times==0)
	{
		if(i<20)				//�Ѳɼ�����20�ε�ֵ���ŵ�������
		{
			dat[i]=TA0CCR3-LastCCR1;
			i++;
		}
		if(i>=20)				//Ϊ�˷�ֹ������Ѳɼ�������ͷ��β��ֵȥ����ȡ�м��ƽ��ֵ
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
		TA0CCTL3&=~CM_2;		//����½��ش���
		TA0CCTL3|=CM_1;			//��Ϊ�����ش���
		times++;				//�ı�times��ֵ
	}
	times&=0x01;				//times>1ʱ��0
	LPM0_EXIT;					//�˳��͹���ģʽ
	TA0CCTL3&=~CCIFG;			//����ж���Ӧ��־
	_EINT();					//���ж�
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