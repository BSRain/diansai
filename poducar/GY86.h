#ifndef GY86_H_
#define GY86_H_
#include <msp430.h>
#include "Black_OLED.h"
#define uchar unsigned char

#define CPU_F ((double)8000000)
#define delayus(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))  //宏定义延时函数
#define delayms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))


#define SCL1 P4OUT |=BIT1
#define SCL0 P4OUT &=~BIT1

#define SCLOUT P4DIR |= BIT1;


#define SDA1 P4OUT |=BIT2   //IIC数据引脚

#define SDA0 P4OUT &=~BIT2

#define SDAIN P4DIR &=~BIT2

#define SDAOUT P4DIR |=BIT2

#define SDADATA (P4IN & BIT2)


void MPU_IIC_Delay(void);               //MPU IICÑÓÊ±º¯Êý
void MPU_IIC_Start(void);               //·¢ËÍIIC¿ªÊ¼ÐÅºÅ
void MPU_IIC_Stop(void);                //·¢ËÍIICÍ£Ö¹ÐÅºÅ
void MPU_IIC_Send_Byte(u8 txd);         //IIC·¢ËÍÒ»¸ö×Ö½Ú
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC¶ÁÈ¡Ò»¸ö×Ö½Ú
u8 MPU_IIC_Wait_Ack(void);              //IICµÈ´ýACKÐÅºÅ
void MPU_IIC_Ack(void);                 //IIC·¢ËÍACKÐÅºÅ
void MPU_IIC_NAck(void);                //IIC²»·¢ËÍACKÐÅºÅ
u8 MPU_Read_Byte(u8 reg);                       //IIC¶ÁÒ»¸ö×Ö½Ú
u8 MPU_Write_Byte(u8 reg,u8 data);
//****************************************

// 定义MPU6050内部地址

//****************************************

#define SMPLRT_DIV  0x19 //陀螺仪采样率典型值0x07(125Hz)

#define CONFIG   0x1A //低通滤波频率典型值0x06(5Hz)

#define GYRO_CONFIG  0x1B //陀螺仪自检及测量范围典型值0x18(不自检2000deg/s)

#define ACCEL_CONFIG 0x1C //加速计自检、测量范围及高通滤波频率典型值0x01(不自检2G5Hz)

#define ACCEL_XOUT_H 0x3B

#define ACCEL_XOUT_L 0x3C

#define ACCEL_YOUT_H 0x3D

#define ACCEL_YOUT_L 0x3E

#define ACCEL_ZOUT_H 0x3F

#define ACCEL_ZOUT_L 0x40

#define TEMP_OUT_H  0x41

#define TEMP_OUT_L  0x42

#define GYRO_XOUT_H  0x43

#define GYRO_XOUT_L  0x44

#define GYRO_YOUT_H  0x45

#define GYRO_YOUT_L  0x46

#define GYRO_ZOUT_H  0x47

#define GYRO_ZOUT_L  0x48

#define PWR_MGMT_1  0x6B //电源管理典型值0x00(正常启用)

#define WHO_AM_I   0x75 //IIC地址寄存器(默认数值0x68只读)

#define SlaveAddress 0xD0 //IIC写入时的地址字节数据+1为读取



void I2C_Start();

void I2C_Stop();

void I2C_SendACK(uchar ack);

uchar I2C_RecvACK();

void I2C_SendByte(uchar dat);

uchar I2C_RecvByte();
void Single_WriteI2C(uchar REG_Address,uchar REG_data);

uchar Single_ReadI2C(uchar REG_Address);

void InitMPU6050();
int GetData(uchar REG_Address);
  
#endif