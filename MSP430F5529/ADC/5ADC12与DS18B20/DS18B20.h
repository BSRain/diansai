#ifndef TEMPATURE_H_
#define TEMPATURE_H_
/*��׼��ʱ����*/
#define CPU_F                               ((double)12000000)
#define DELAY_US(x)                       __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)                      __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int

void  B20_Init(void);                                                           //��λDS18B20
unsigned char B20_ReadByte(void);                                                       //��ȡһ���ֽڵ�����
void  B20_WriteByte(unsigned char  data);                                              //д��һ���ֽڵ�����,dataΪDS18B20��ROM���ƣ��û����ù��������ö�����ֵ
float B20_Read_temp(void);                                                      //��ȡ�¶�ֵ

#endif
