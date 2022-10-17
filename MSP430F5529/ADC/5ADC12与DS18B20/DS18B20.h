#ifndef TEMPATURE_H_
#define TEMPATURE_H_
/*精准延时函数*/
#define CPU_F                               ((double)12000000)
#define DELAY_US(x)                       __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)                      __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int

void  B20_Init(void);                                                           //复位DS18B20
unsigned char B20_ReadByte(void);                                                       //读取一个字节的数据
void  B20_WriteByte(unsigned char  data);                                              //写入一个字节的数据,data为DS18B20的ROM控制，用户不用管它，不用对它赋值
float B20_Read_temp(void);                                                      //读取温度值

#endif
