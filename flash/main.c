#include "msp430.h"
#include "FLASH.h"
char data[14]={'1','2','3','4','5'};
      char ans[14];
void main(void)
{
      WDTCTL = WDTPW + WDTHOLD;                    // �رտ��Ź�
      /*unsigned long * Flash_ptrD;               // ����ָ����Ϣ�洢��D�ε�ָ��
      unsigned char value;
     // OLED_Init();
      Flash_ptrD = (unsigned long *) 0x1800; // ��ʼ��ָ��
      value = '1';                        // ��ʼ����д��ĳ���
      FCTL3 = FWKEY;                              // ���Flash����λ
      FCTL1 = FWKEY + ERASE;                       // ���ò�������λ   #define ERASE                  (0x0002) #define FWKEY                  (0xA500)
      _DINT();               // Flash�����ڼ䲻�����жϣ����򽫵��²���Ԥ�ƵĴ���
      *Flash_ptrD = 0;                           // ����ڵ�ַд0������д�룬������������
      while(FCTL3 & BUSY);                      // �ȴ������������
      FCTL1 = FWKEY + BLKWRT;                     // ʹ�ܳ���д�����
      *Flash_ptrD = value;                      // ������д��Ŀ��Flash��
      while(FCTL3 & BUSY);                      // �ȴ�д��������
      _EINT();                                    // ����ȫ���ж�
      FCTL1 = FWKEY;                             // Flash�˳�дģʽ
      FCTL3 = FWKEY + LOCK;                       // �ָ�Flash������λ����������
      
      unsigned long * read_point;
      unsigned char read_content ;
      read_point = (unsigned long *) 0x1800;
      read_content = *read_point;*/
      
      write_flash_char(0x1800,data,14);
      read_flash_char1(0x1800,ans,14);
      //OLED_Write_Num5(0,0,read_content);
      while(1);                                   // ��ѭ�������ڴ˴����öϵ�鿴�ڴ�ռ�
}

