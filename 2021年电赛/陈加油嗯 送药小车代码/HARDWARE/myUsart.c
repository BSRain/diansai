#include "myUsart.h"
#include "sys.h"

char RxData='0';
extern int mode;


/*
USART1
*/
void USART1_Init(uint32_t bound)
{
		GPIO_InitTypeDef GPIO_Initstructure;
    NVIC_InitTypeDef NVIC_InitStruct;
		USART_InitTypeDef USART1_InitStructure;
	
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


		// USART1_TX     PA9
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstructure);

    // USART1_RX     PA10
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstructure);

    // USART3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART1_InitStructure.USART_BaudRate = bound;
    USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
    USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //��ģʽ
    USART1_InitStructure.USART_Parity = USART_Parity_No;                             //����żУ��λ
    USART1_InitStructure.USART_StopBits = USART_StopBits_1;                          //һ��ֹͣλ
    USART1_InitStructure.USART_WordLength = USART_WordLength_8b;                     //�ֳ�Ϊ8λ���ݸ�ʽ

    USART_Init(USART1, &USART1_InitStructure);
    USART_Cmd(USART1, ENABLE); //ʹ��USART1

    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

    NVIC_Init(&NVIC_InitStruct);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);
}

void USART1_IRQHandler(void)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
    {
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
        RxData = USART_ReceiveData(USART1);
	
//        USART_SendData(USART1, RxData);
//        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) //�ȴ����ݷ���
//            ;
    }
}

