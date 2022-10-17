#include "msp430.h"
#include "FLASH.h"
char data[14]={'1','2','3','4','5'};
      char ans[14];
void main(void)
{
      WDTCTL = WDTPW + WDTHOLD;                    // 关闭看门狗
      /*unsigned long * Flash_ptrD;               // 定义指向信息存储器D段的指针
      unsigned char value;
     // OLED_Init();
      Flash_ptrD = (unsigned long *) 0x1800; // 初始化指针
      value = '1';                        // 初始化需写入的长字
      FCTL3 = FWKEY;                              // 清除Flash锁定位
      FCTL1 = FWKEY + ERASE;                       // 设置擦除控制位   #define ERASE                  (0x0002) #define FWKEY                  (0xA500)
      _DINT();               // Flash操作期间不允许中断，否则将导致不可预计的错误
      *Flash_ptrD = 0;                           // 向段内地址写0，即空写入，启动擦除操作
      while(FCTL3 & BUSY);                      // 等待擦除操作完成
      FCTL1 = FWKEY + BLKWRT;                     // 使能长字写入操作
      *Flash_ptrD = value;                      // 将长字写入目的Flash段
      while(FCTL3 & BUSY);                      // 等待写入操作完成
      _EINT();                                    // 启动全局中断
      FCTL1 = FWKEY;                             // Flash退出写模式
      FCTL3 = FWKEY + LOCK;                       // 恢复Flash的锁定位，保护数据
      
      unsigned long * read_point;
      unsigned char read_content ;
      read_point = (unsigned long *) 0x1800;
      read_content = *read_point;*/
      
      write_flash_char(0x1800,data,14);
      read_flash_char1(0x1800,ans,14);
      //OLED_Write_Num5(0,0,read_content);
      while(1);                                   // 主循环，可在此处设置断点查看内存空间
}

