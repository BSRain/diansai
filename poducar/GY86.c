#include "GY86.h"


//mpu6050.c

uchar dis[4];

int dis_data;

//**************************************

//I2C起始信号

//**************************************

void I2C_Start()

{
    SDAOUT;
    SDA1;                    //拉高数据线
    SCLOUT;
    SCL1;                    //拉高时钟线
delayus(2);                 //延时
    SDA0;                    //产生下降沿
delayus(2);                  //延时
    SCL0;                    //拉低时钟线
}

//**************************************

//I2C停止信号

//**************************************

void I2C_Stop()

{

    SDAOUT;
    SDA0;                    //拉低数据线
    SCL0;
    SCLOUT;
    delayus(2);                  //延时
    SCL1;                    //拉高时钟线
    SDA1;
    delayus(2);                  //延时
}

//**************************************

//I2C发送应答信号

//入口参数:ack (0:ACK 1:NAK)

//**************************************

void I2C_SendACK(uchar ack)
{
SDAOUT;
SCLOUT;
if(ack) SDA1;
else SDA0;
//    SDA = ack;                  //写应答信号
    SCL1;                    //拉高时钟线
delayus(2);                  //延时
    SCL0;                    //拉低时钟线
delayus(2);                  //延时
}

//**************************************

//I2C接收应答信号

//**************************************

uchar I2C_RecvACK()
{
uchar cy;
    SCLOUT;
    SCL1;                    //拉高时钟线
    SDAIN;
delayus(2);                 //延时
if(SDADATA)
    {
cy=1;
    }
else
    {
cy=0;
    }
//    cy = SDA;                   //读应答信号
    SCL0;                    //拉低时钟线
delayus(2);                //延时
    SDAOUT;
return cy;
}

//**************************************

//向I2C总线发送一个字节数据

//**************************************

void I2C_SendByte(uchar dat)
{
uchar i;
    SDAOUT;
    for (i=0; i<8; i++)         //8位计数器
{
if((dat<<i)&0x80)
        {
            SDA1;
        }
        else
       {
            SDA0;
        }
       // SDA = cy;               //送数据口
        SCLOUT;
        SCL1;                //拉高时钟线
        delayus(2);              //延时
        SCL0;                //拉低时钟线
        delayus(2);              //延时
    }
    I2C_RecvACK();
}

//**************************************

//从I2C总线接收一个字节数据

//**************************************

uchar I2C_RecvByte()
{
uchar i;
uchar dat = 0,cy;

    SDAOUT;
    SCLOUT;
    SDA1;                    //使能内部上拉,准备读取数据,
    SDAIN;
    for (i=0; i<8; i++)         //8位计数器
    {
    dat<<= 1;
    SCL1;                //拉高时钟线
    delayus(2);             //延时

    if(SDADATA)
        {
        cy=1;
        }
    else
        {
        cy=0;
        }
    dat |= cy;             //读数据         SCL0;                //拉低时钟线
    delayus(2);             //延时
    }

    SDAOUT;

return dat;

}

//**************************************

//向I2C设备写入一个字节数据

//**************************************

void Single_WriteI2C(uchar REG_Address,uchar REG_data)

{

    I2C_Start();                  //起始信号

    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号

    I2C_SendByte(REG_Address);    //内部寄存器地址

    I2C_SendByte(REG_data);       //内部寄存器数据

    I2C_Stop();                   //发送停止信号

}

//**************************************

//从I2C设备读取一个字节数据

//**************************************

uchar Single_ReadI2C(uchar REG_Address)

{

 uchar REG_data;

 I2C_Start();                   //起始信号

 I2C_SendByte(SlaveAddress);    //发送设备地址+写信号

 I2C_SendByte(REG_Address);     //发送存储单元地址从0开始

 I2C_Start();                   //起始信号

 I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号

 REG_data=I2C_RecvByte();       //读出寄存器数据

 I2C_SendACK(1);                //接收应答信号

 I2C_Stop();                    //停止信号

 return REG_data;

}

//**************************************

//初始化MPU6050

//**************************************

void InitMPU6050()

{
 Single_WriteI2C(PWR_MGMT_1, 0x00); //解除休眠状态
 Single_WriteI2C(SMPLRT_DIV, 0x07);
 Single_WriteI2C(CONFIG, 0x06);
 Single_WriteI2C(GYRO_CONFIG, 0x18);
 Single_WriteI2C(ACCEL_CONFIG, 0x01);
}


//合成数据

//**************************************

int GetData(uchar REG_Address)

{

 char H,L;

 H=MPU_Read_Byte(REG_Address);

 L=MPU_Read_Byte(REG_Address+1);
 return (H<<8)+L;   //合成数据

}





void MPU_IIC_Delay(void)
{
    delayus(2);
}


void MPU_IIC_Start(void)
{
    SDAOUT;     //sdaÏßÊä³ö
    SDA1;
    SCL1;
    MPU_IIC_Delay();
    SDA0;//START:when CLK is high,DATA change form high to low
    MPU_IIC_Delay();
    SCL0;//Ç¯×¡I2C×ÜÏß£¬×¼±¸·¢ËÍ»ò½ÓÊÕÊý¾Ý

}

void MPU_IIC_Stop(void)
{
    SDAOUT;//sdaÏßÊä³ö
    SCL0;
    SDA0;//STOP:when CLK is high DATA change form low to high
    MPU_IIC_Delay();
    SCL1;
    SDA1;//·¢ËÍI2C×ÜÏß½áÊøÐÅºÅ
    MPU_IIC_Delay();
}

u8 MPU_IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDAIN;      //SDAÉèÖÃÎªÊäÈë
    SDA1;MPU_IIC_Delay();
    SCL1;MPU_IIC_Delay();
    while(SDADATA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    SCL0;//Ê±ÖÓÊä³ö0
    return 0;
}

void MPU_IIC_Ack(void)
{
    SCL0;
    SDAOUT;
    SDA0;
    MPU_IIC_Delay();
    SCL1;
    MPU_IIC_Delay();
    SCL0;
}
void MPU_IIC_NAck(void)
{
    SCL0;
    SDAOUT;
    SDA1;
    MPU_IIC_Delay();
    SCL1;
    MPU_IIC_Delay();
    SCL0;
}
void MPU_IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDAOUT;
    SCL0;//À­µÍÊ±ÖÓ¿ªÊ¼Êý¾Ý´«Êä
    for(t=0;t<8;t++)
    {
        if(((txd&0x80)>>7)==1)
            SDA1;
        else SDA0;

        txd<<=1;
            SCL1;
            MPU_IIC_Delay();
            SCL0;
            MPU_IIC_Delay();
    }
}

u8 MPU_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDAIN;//SDAÉèÖÃÎªÊäÈë
    for(i=0;i<8;i++ )
    {
        SCL0;
        MPU_IIC_Delay();
        SCL1;
        receive<<=1;
        if(SDADATA)receive++;
        MPU_IIC_Delay();
    }
    if (!ack)
        MPU_IIC_NAck();//·¢ËÍnACK
    else
        MPU_IIC_Ack(); //·¢ËÍACK
    return receive;
}


u8 MPU_Read_Byte(u8 reg)
{
    u8 res;
    MPU_IIC_Start();

    MPU_IIC_Send_Byte((0X68<<1)|0);//·¢ËÍÆ÷¼þµØÖ·+Ð´ÃüÁî
    MPU_IIC_Wait_Ack();     //µÈ´ýÓ¦´ð
    MPU_IIC_Send_Byte(reg); //Ð´¼Ä´æÆ÷µØÖ·
    MPU_IIC_Wait_Ack();     //µÈ´ýÓ¦´ð
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((0X68<<1)|1);//·¢ËÍÆ÷¼þµØÖ·+¶ÁÃüÁî
    MPU_IIC_Wait_Ack();     //µÈ´ýÓ¦´ð
    res=MPU_IIC_Read_Byte(0);//¶ÁÈ¡Êý¾Ý,·¢ËÍnACK
    MPU_IIC_Stop();         //²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ
    return res;
}

u8 MPU_Write_Byte(u8 reg,u8 data)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((0X68<<1)|0);//·¢ËÍÆ÷¼þµØÖ·+Ð´ÃüÁî
    if(MPU_IIC_Wait_Ack())  //µÈ´ýÓ¦´ð
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg); //Ð´¼Ä´æÆ÷µØÖ·
    MPU_IIC_Wait_Ack();     //µÈ´ýÓ¦´ð
    MPU_IIC_Send_Byte(data);//·¢ËÍÊý¾Ý
    if(MPU_IIC_Wait_Ack())  //µÈ´ýACK
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Stop();
    return 0;
}
