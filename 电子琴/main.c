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
unsigned int LastCCR1;		//��һ�θߵ�ƽ������ʱ�䣬�;��������
unsigned int dat[20];		//ͨ�����鱸�ݲɼ���������
char juli[10]="0";
char * Flash_ptr; 
void play();
void play2();
void play3();
static char data[14];//�洢��������
//static char data2[14];//�洢��������
char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//������
    unsigned unum;//���Ҫת���������ľ���ֵ,ת�������������Ǹ���
    int i=0,j,k;//i����ָʾ�����ַ�����Ӧλ��ת��֮��i��ʵ�����ַ����ĳ��ȣ�ת����˳��������ģ��������������k����ָʾ����˳��Ŀ�ʼλ��;j����ָʾ����˳��ʱ�Ľ�����
 
    //��ȡҪת���������ľ���ֵ
    if(radix==10&&num<0)//Ҫת����ʮ�����������Ǹ���
    {
        unum=(unsigned)-num;//��num�ľ���ֵ����unum
        str[i++]='-';//���ַ�����ǰ������Ϊ'-'�ţ�����������1
    }
    else unum=(unsigned)num;//����numΪ����ֱ�Ӹ�ֵ��unum
 
    //ת�����֣�ע��ת�����������
    do
    {
        str[i++]=index[unum%(unsigned)radix];//ȡunum�����һλ��������Ϊstr��Ӧλ��ָʾ������1
        unum/=radix;//unumȥ�����һλ
 
    }while(unum);//ֱ��unumΪ0�˳�ѭ��
 
    str[i]='\0';//���ַ���������'\0'�ַ���c�����ַ�����'\0'������
 
    //��˳���������
    if(str[0]=='-') k=1;//����Ǹ��������Ų��õ������ӷ��ź��濪ʼ����
    else k=0;//���Ǹ�����ȫ����Ҫ����
 
    char temp;//��ʱ��������������ֵʱ�õ�
    for(j=k;j<=(i-1)/2;j++)//ͷβһһ�Գƽ�����i��ʵ�����ַ����ĳ��ȣ��������ֵ�ȳ�����1
    {
        temp=str[j];//ͷ����ֵ����ʱ����
        str[j]=str[i-1+k-j];//β����ֵ��ͷ��
        str[i-1+k-j]=temp;//����ʱ������ֵ(��ʵ����֮ǰ��ͷ��ֵ)����β��
    }
 
    return str;//����ת������ַ���
 
}

__ramfunc void Flash_Write_Byte(uchar data)//(uint16_t segmentNo,uint16_t offset,uint8_t data)
{
  char * Addr;                         // Initialize Flash pointer
  Addr = (char *) 0x1880;
  FCTL3 = FWKEY;                            //�������
  FCTL1 = FWKEY+WRT;                        //��λ���λ
  _DINT();  
  *Addr = data;                             //д��һ����
  while (FCTL3 & BUSY);                     //�ȴ��������
  _EINT(); 
  FCTL1 = FWKEY;                            //���д��λ
  FCTL3 = FWKEY+LOCK;                       //��FLASH�������ɶ�������д�����ɲ�������������
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
void FlashRead(uchar *pc_byte,uchar *Dataout,uint count)
{
   while(count--)
  {
    *Dataout = *pc_byte;
    Dataout++;
    pc_byte++;
    }
}


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
  HC_SR04Init();			//��ʼ������ģ��HC_SR04
  TimeGetInit();			//��ʼ�������������ţ�ģʽ��
  _EINT();
  while(1)
  {
    Disp_HZ(List0,"����:",0);
    Disp_HZ(List1,"ģʽ:",0);
    Disp_HZ(List1+3,"��ͨ",0);
    HC_SR04Start();
    /*P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;*/
    
     /*UART�ĳ�ʼ��*/
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
        Disp_HZ(List1+3,"����",0);
        //play();
        delay_ms(1000); 
      }
      else if(key=='9')
      {
        Disp_HZ(List1+3,"����",0);
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
#pragma vector=USCI_A0_VECTOR  // �����ж�������ַ
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
        Disp_HZ(List0,"����:",0);
        Disp_HZ(List1,"ģʽ:",0);
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
			dat[i]=TA0CCR3-LastCCR1;;
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
			k=t/5;//
			i=0;
		}
		TA0CCTL3&=~CM_2;		//����½��ش���
		TA0CCTL3|=CM_1;			//��Ϊ�����ش���
		times++;				//�ı�times��ֵ
	}
	times&=0x01;				//times>1ʱ��0
	Disp_HZ(List2,"����:",0);
        k*=1000000;
        length=340*(k/2)/100;//cm
        itoa(length,juli,10);
        Disp_SZ(List2+3,juli[0]);
        Disp_SZ(List2+4,juli[1]);
        Disp_SZ(List2+5,juli[2]);
        LPM0_EXIT;					//�˳��͹���ģʽ
	TA0CCTL3&=~CCIFG;			//����ж���Ӧ��־
        _EINT();					//���ж�
}

void play()
{
     Disp_HZ(List1+3,"����",0);
     jby_1(400);
     jby_2(400);
     jby_3(400);
     jby_1(400);
     _delay_cycles(3000); //ÿ���������ĺ��һ����ʱ
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
     Disp_HZ(List0,"����:",0);
     Disp_HZ(List1,"ģʽ:",0);
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