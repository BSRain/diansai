#include "MSP430f5529.h"
#include "DS18B20.h"
void System_overclocking(unsigned char Fre) //倍频函数
{
  P5SEL |= BIT2|BIT3|BIT4|BIT5;//开启外部两个时钟

  UCSCTL6 |= XCAP_3|XT1OFF;          // XT1 相关 配置
  UCSCTL6 |= XT2DRIVE_0 |XT2OFF;     // XT2 相关 配置
  //以下是提升核心电压部分的代码
  PMMCTL0_H = 0xA5;                                         //开PMM电源管理
  SVSMLCTL |= SVSMLRRL_1 + SVMLE;                            //配置SVML电压
  PMMCTL0 =  PMMPW + PMMCOREV_3;                             //配置内核电压
  while((PMMIFG & SVSMLDLYIFG ) == 0);                       //等待设置完成
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG + SVSMLDLYIFG);
  if((PMMIFG & SVMLIFG) == 1)                                //判断内核电压是否上升到VSVML
    while((PMMIFG & SVMLVLRIFG) == 0);                    //如果没有等待
  SVSMLCTL &= ~SVMLE;                                        //关掉SVML模块
  PMMCTL0_H = 0X00;

  __bis_SR_register(SCG0);                 //该语法为固定格式，意为将括号内的变量置位，SCG0与系统工作模式有关，此时 MCLK 暂停工作
  UCSCTL0 = 0;                             //先清零，FLL 运行时，该寄存器系统会自动配置，不用管
  UCSCTL6 = (UCSCTL6&(~(XT2OFF|XT1OFF))|XCAP_3|XT2DRIVE_0);
  UCSCTL3 = (5<<4)|(2<<0);                 // 选择 XTAL2 的时钟信号作为参考信号 并且分频到1MHz
  if(Fre < 5)
    UCSCTL1 = DCORSEL_2;
  else if(Fre<15)
    UCSCTL1 = DCORSEL_4;
  else
    UCSCTL1 = DCORSEL_7;
  UCSCTL2 = (Fre-1);
  __bic_SR_register(SCG0);
  __delay_cycles(782000);
  while (SFRIFG1 & OFIFG) {                               // Check OFIFG fault flag
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);           // Clear OSC flaut Flags
    SFRIFG1 &= ~OFIFG;                                    // Clear OFIFG fault flag
  }
  UCSCTL4 = UCSCTL4&(~(SELS_7|SELM_7))|SELS_3|SELM_3;

  P2DIR |= BIT2;  // 配置P2.2为输出模式
  P2SEL |= BIT2;  // 功能复用:SMCLK输出 （子系统主时钟）
  P7DIR |= BIT7;  // 配置P7.7为输出模式
  P7SEL |= BIT7;  // 功能复用:MCLK输出    （主系统时钟）
}
void main()
{
   WDTCTL = WDTPW + WDTHOLD;
   System_overclocking(12);   //12MHZ
   while(1)
   {
    B20_Read_temp();          //读取温度值
   }
}