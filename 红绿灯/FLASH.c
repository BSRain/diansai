#include  <msp430.h>
#include "FLASH.h"
//addr:FLASH��һ�����׵�ַ�� value:������ count:Ҫ��������ݸ���
//��FLASH��ַ�������� ��Ҫ�洢�����ݵĸ��� ��������ĺ������Ϳ���д����
//**********************************************************************************
void write_flash_char (unsigned int addr, char *array,int count) //д char������
{
  char *Flash_ptr;                          // Flash pointer
  int i;
  Flash_ptr = (char *)addr;                 // Initialize Flash pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash segment
   FCTL1 = FWKEY + WRT;                     // Set WRT bit for write operation

  for (i=0; i<count; i++)
  {
    if(array[i]=='0')
       break;
    *Flash_ptr++ = array[i];                // Write value to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

//**********************************************************************************
void write_flash_int (unsigned int addr, int *array,int count) //addrΪ���׵�ַ��д int������
{
  int *Flash_ptr;                           // Flash pointer
  int i;
  Flash_ptr = (int *)addr;                   // Initialize Flash pointer
  FCTL1 = FWKEY + ERASE;                     // Set Erase bit
  FCTL3 = FWKEY;                             // Clear Lock bit
  *Flash_ptr = 0;                            // Dummy write to erase Flash segment
   FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
  for (i=0; i<count; i++)
  {
   
    *Flash_ptr++ = array[i];                 // Write value to flash
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

//**********************************************************************************
char read_flash_char0(unsigned int addr) //�����ֽ�
{ char *address;
  address=(char*)addr;
  return *address;
}

//**********************************************************************************
//��FLASH��ַ�������� ��Ҫ��ȡ�����ݵĸ��� ��������ĺ������Ϳ��Զ�����
void read_flash_char1(unsigned int addr,char *array,int count) //��һ������
{ char *address=(char *)addr;
  for(int i=0;i<count;i++)
   {
     
     array[i]=*address++;
     
   }
}

//**********************************************************************************
int read_flash_int0(unsigned int addr) //ż��ַ,��һ����
{
 int *address=(int *)addr;
 return *address;
}

//**********************************************************************************
void read_flash_int1(unsigned int addr,int *array, int count) //����������
{
 int *address=(int *)addr;
 for(int i=0;i<count;i++)
   {
     array[i]=*address++;
     
   }
}

//**********************************************************************************
void init_flash(void)
{
 //FCTL2 = FWKEY + +FSSEL1+FSSEL0 + FN0;   // (DCO)SMCLK/2 for Flash Timing Generator
 }