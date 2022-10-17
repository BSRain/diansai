#include  <msp430.h>
#include "FLASH.h"
//addr:FLASH的一个段首地址， value:数组名 count:要储存的数据个数
//把FLASH地址、数组名 和要存储的数据的个数 赋给下面的函数，就可以写入了
//**********************************************************************************
void write_flash_char (unsigned int addr, char *array,int count) //写 char型数组
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
    *Flash_ptr++ = array[i];                // Write value to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}

//**********************************************************************************
void write_flash_int (unsigned int addr, int *array,int count) //addr为段首地址，写 int型数组
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
char read_flash_char0(unsigned int addr) //读单字节
{ char *address;
  address=(char*)addr;
  return *address;
}

//**********************************************************************************
//把FLASH地址、数组名 和要读取的数据的个数 赋给下面的函数，就可以读入了
void read_flash_char1(unsigned int addr,char *array,int count) //读一串数据
{ char *address=(char *)addr;
  for(int i=0;i<count;i++)
   {
     array[i]=*address++;
     
   }
}

//**********************************************************************************
int read_flash_int0(unsigned int addr) //偶地址,读一个字
{
 int *address=(int *)addr;
 return *address;
}

//**********************************************************************************
void read_flash_int1(unsigned int addr,int *array, int count) //读整形数组
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