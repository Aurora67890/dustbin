/****************************************************************************
 *	@笔者	：	Q
 *	@日期	：	2023年2月8日
 *	@所属	：	杭州友辉科技
 *	@功能	：	存放usart串口相关的函数
 *	@函数列表:
 *	1.	void uart1_init(u32 baud) -- 初始化串口1
 *	2.	void uart3_init(u32 baud) -- 初始化串口3
 *	3.	void uart1_send_byte(u8 dat) -- 串口1发送字节
 *	4.	void uart3_send_byte(u8 dat) -- 串口3发送字节
 *	5.	void uart1_send_str(char *s) -- 串口1发送字符串
 *	6.	void uart3_send_str(char *s) -- 串口3发送字符串
 ****************************************************************************/

#include "./usart/y_usart.h"
#include <stdio.h>
#include <stdbool.h> 

static ServoData_Fifo sd_fifo = { { 0 }, 0, { 0 } };
u8 uart_receive_buf[UART_BUF_SIZE];
uint16_t uart1_get_ok;
u8 uart1_mode;

/* 初始化串口1 */
void uart1_init(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 使能端口时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

	USART_DeInit(USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		/* PA.9 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /* 复用推挽输出 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; /* 浮空输入 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = BaudRate;									/* 串口波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						/* 字长为8位数据格式 */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							/* 字长为8位数据格式 */
	USART_InitStructure.USART_Parity = USART_Parity_No;								/* 无奇偶校验位 */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					/* 收发模式 */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* 无硬件数据流控制 */
	USART_Init(USART1, &USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; /* 抢占优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  /* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  /* IRQ通道使能 */
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /* 开启串口接受中断 */
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE); /* 禁止串口发送中断 */

	USART_Cmd(USART1, ENABLE); /* 使能串口1  */
}

/***********************************************
	函数名称:	uart3_init()
	功能介绍:	初始化串口3
	函数参数:	baud 波特率
	返回值:		无
 ***********************************************/
//用官方的初始化可以进中断
void uart3_init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART3, &USART_ClockInitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE); /* 禁止串口发送中断 */

	USART_Cmd(USART3, ENABLE);
}

/***********************************************
	功能介绍：	串口1发送字节
	函数参数：	dat 发送的字节
	返回值：		无
 ***********************************************/
void uart1_send_byte(u8 dat)
{
	USART_SendData(USART1, dat);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/***********************************************
	功能介绍：	串口1发送字符串
	函数参数：	*s 发送的字符串
	返回值：		无
 ***********************************************/
void uart1_send_str(u8 *s)
{
	while (*s)
	{
		uart1_send_byte(*s++);
	}
}

/***********************************************
	功能介绍：	串口1发送数字
	函数参数：	tmp 发送的数字
	返回值：		无
 ***********************************************/
void uart1_send_int(int tmp)
{
	static u8 str[20];
	sprintf((char *)str, "%d", tmp);
	uart1_send_str(str);
}

/* 重定义fputc函数,写这个函数可以使用printf,记得开启Use MicroLIB */
int fputc(int ch, FILE *f)
{
	while ((USART3->SR & 0X40) == 0); // 循环发送,直到发送完毕
	USART3->DR = (u8)ch;
	return ch;
}

/***********************************************
	函数名称:	uart3_send_byte()
	功能介绍:	串口3发送字节
	函数参数:	dat 发送的字节
	返回值:		无
 ***********************************************/
void uart3_send_byte(u8 dat)
{
	USART_SendData(USART3, dat);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	return;
}

void uart3_send_weight(s32 value) {
    // 将s32值右移24位，然后与0xFF进行AND操作以提取第一个字节
//    u8 first_byte = (u8)((value >> 24) & 0xFF);
//    uart3_send_byte(first_byte); // 发送第一个字节
//    first_byte = (u8)((value >> 16) & 0xFF);
//    uart3_send_byte(first_byte);
    u8 first_byte = (u8)((value >> 0) & 0xFF);
    uart3_send_byte(first_byte);
//		printf("first_byte=%d",first_byte);
}

/***********************************************
	函数名称:	uart3_send_str()
	功能介绍:	串口3发送字符串
	函数参数:	*s 发送的字符串
	返回值:		无
 ***********************************************/
void uart3_send_str(u8 *s)
{
	while (*s)
	{
		uart3_send_byte(*s++);
	}
}

/* 串口1中断服务程序 */
void USART1_IRQHandler(void) /* 最后数据发送\r\n结束 */
{
	u8 sbuf_bak;
	static u16 buf_index = 0;

	if (USART_GetFlagStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		sbuf_bak = USART_ReceiveData(USART1);
		// USART_SendData(USART1, sbuf_bak);
		if (uart1_get_ok)
			return;
		if (sbuf_bak == '<')
		{
			uart1_mode = 4;
			buf_index = 0;
		}
		else if (uart1_mode == 0)
		{
			if (sbuf_bak == '$')
			{
				// 命令模式 $XXX!
				uart1_mode = 1;
			}
			else if (sbuf_bak == '#')
			{
				// 单舵机模式	#000P1500T1000! 类似这种命令
				uart1_mode = 2;
			}
			else if (sbuf_bak == '{')
			{
				// 多舵机模式	{#000P1500T1000!#001P1500T1000!} 多个单舵机命令用大括号括起来
				uart1_mode = 3;
			}
			else if (sbuf_bak == '<')
			{
				// 保存动作组模式	<G0000#000P1500T1000!#001P1500T1000!B000!> 用尖括号括起来 带有组序号
				uart1_mode = 4;
			}
			buf_index = 0;
		}

		uart_receive_buf[buf_index++] = sbuf_bak;

		if ((uart1_mode == 4) && (sbuf_bak == '>'))
		{
			uart_receive_buf[buf_index] = '\0';
			uart1_get_ok = 1;
		}
		else if ((uart1_mode == 1) && (sbuf_bak == '!'))
		{
			uart_receive_buf[buf_index] = '\0';
			uart1_get_ok = 1;
		}
		else if ((uart1_mode == 2) && (sbuf_bak == '!'))
		{
			uart_receive_buf[buf_index] = '\0';
			uart1_get_ok = 1;
		}
		else if ((uart1_mode == 3) && (sbuf_bak == '}'))
		{
			uart_receive_buf[buf_index] = '\0';
			uart1_get_ok = 1;
		}

		if (buf_index >= UART_BUF_SIZE)
			buf_index = 0;
	}
}

void USART3_IRQHandler(void)
{
	static bool isRec = false; // 是否接收到x5发送的0
	if (USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//		delay_ms(1);
//		OLED_ShowString(0, 0, "x5", OLED_6X8);
//		OLED_Update();
		// fifo接受舵机控制量
		sd_fifo.buffer[sd_fifo.buffer_length++] = USART_ReceiveData(USART3);
//		OLED_ShowNum( 50, 0, sd_fifo.buffer_length, 2, OLED_6X8);
//		OLED_Update();
//		
		// 当缓冲区满时处理
		if ( sd_fifo.buffer_length >= BUFFER_SIZE )
		{
			// 处理接收到的两个整数
			sd_fifo.data[SERVO0] = ( ( uint16_t )sd_fifo.buffer[0] << 8)  | sd_fifo.buffer[1];
			sd_fifo.data[SERVO1] = ( ( uint16_t )sd_fifo.buffer[2] << 8)  | sd_fifo.buffer[3];
			sd_fifo.data[SERVO2] = ( ( uint16_t )sd_fifo.buffer[4] << 8)  | sd_fifo.buffer[5];
			sd_fifo.data[SERVO3] = ( ( uint16_t )sd_fifo.buffer[6] << 8)  | sd_fifo.buffer[7];
			sd_fifo.data[SERVO4] = ( ( uint16_t )sd_fifo.buffer[8] << 8)  | sd_fifo.buffer[9];
			sd_fifo.data[SERVO5] = ( ( uint16_t )sd_fifo.buffer[10] << 8) | sd_fifo.buffer[11];
			sd_fifo.data[SERVO6] = ( ( uint16_t )sd_fifo.buffer[12] << 8) | sd_fifo.buffer[13];
			sd_fifo.data[PUSH0] = ( ( uint16_t )sd_fifo.buffer[14] << 8) | sd_fifo.buffer[15];
					
//			sd_fifo.data[SERVO0] = ( ( uint16_t )sd_fifo.buffer[1] << 8)  | sd_fifo.buffer[2];
//			sd_fifo.data[SERVO1] = ( ( uint16_t )sd_fifo.buffer[3] << 8)  | sd_fifo.buffer[4];
//			sd_fifo.data[SERVO2] = ( ( uint16_t )sd_fifo.buffer[5] << 8)  | sd_fifo.buffer[6];
//			sd_fifo.data[SERVO3] = ( ( uint16_t )sd_fifo.buffer[7] << 8)  | sd_fifo.buffer[8];
//			sd_fifo.data[SERVO4] = ( ( uint16_t )sd_fifo.buffer[9] << 8)  | sd_fifo.buffer[10];
//			sd_fifo.data[SERVO5] = ( ( uint16_t )sd_fifo.buffer[11] << 8) | sd_fifo.buffer[12];
//			sd_fifo.data[SERVO6] = ( ( uint16_t )sd_fifo.buffer[13] << 8) | sd_fifo.buffer[14];
//			sd_fifo.data[PUSH0] = ( ( uint16_t )sd_fifo.buffer[15] << 8) | sd_fifo.buffer[16];
			
//				OLED_ShowNum(0, 0,  sd_fifo.data[SERVO0], 4, OLED_6X8);
//				OLED_ShowNum(0, 8,  sd_fifo.data[SERVO1], 4, OLED_6X8);
//				OLED_ShowNum(0, 16, sd_fifo.data[SERVO2], 4, OLED_6X8);
//				OLED_ShowNum(0, 24, sd_fifo.data[SERVO3], 4, OLED_6X8);
//				OLED_ShowNum(0, 32, sd_fifo.data[SERVO4], 4, OLED_6X8);
//				OLED_ShowNum(0, 40, sd_fifo.data[SERVO5], 4, OLED_6X8);
//				OLED_ShowNum(0, 48, sd_fifo.data[SERVO6], 4, OLED_6X8);
//				OLED_ShowNum(0, 56, sd_fifo.data[PUSH0], 4, OLED_6X8);
				
			
			if( isRec  )
			{
			
				if (sd_fifo.data[SERVO0] < 2500 && sd_fifo.data[SERVO0]>500 )
				{
				// 把PWM波输入到结构体里面
				set_servo(SERVO0, sd_fifo.data[SERVO0], MV_Tim);
				set_servo(SERVO1, sd_fifo.data[SERVO1], MV_Tim);
				set_servo(SERVO2, sd_fifo.data[SERVO2], MV_Tim);
				set_servo(SERVO3, sd_fifo.data[SERVO3], MV_Tim);
				set_servo(SERVO4, sd_fifo.data[SERVO4], MV_Tim);
				set_servo(SERVO5, sd_fifo.data[SERVO5], MV_Tim);
				set_servo(SERVO6, sd_fifo.data[SERVO6], MV_Tim);
				set_servo(PUSH0, sd_fifo.data[PUSH0], MV_Tim);
				
//					delay_ms(20);
//				// 显示在OLED屏幕上
//				OLED_ShowHexNum(0, 0,  sd_fifo.buffer[0], 2, OLED_6X8);
//				OLED_ShowHexNum(0, 8,  sd_fifo.buffer[2], 2, OLED_6X8);
//				OLED_ShowHexNum(0, 16, sd_fifo.buffer[4], 2, OLED_6X8);
//				OLED_ShowHexNum(0, 24, sd_fifo.buffer[6], 2, OLED_6X8);
//				OLED_ShowHexNum(0, 32, sd_fifo.buffer[8], 2, OLED_6X8);
//				OLED_ShowHexNum(0, 40, sd_fifo.buffer[10],2, OLED_6X8);
//				OLED_ShowHexNum(0, 48, sd_fifo.buffer[12],2, OLED_6X8);
//				OLED_ShowHexNum(0, 56, sd_fifo.buffer[14],2, OLED_6X8);
//						
//				OLED_ShowHexNum(25, 0,  sd_fifo.buffer[1], 2, OLED_6X8);
//				OLED_ShowHexNum(25, 8,  sd_fifo.buffer[3], 2, OLED_6X8);
//				OLED_ShowHexNum(25, 16, sd_fifo.buffer[5], 2, OLED_6X8);
//				OLED_ShowHexNum(25, 24, sd_fifo.buffer[7], 2, OLED_6X8);
//				OLED_ShowHexNum(25, 32, sd_fifo.buffer[9], 2, OLED_6X8);
//				OLED_ShowHexNum(25, 40, sd_fifo.buffer[11], 2, OLED_6X8);
//				OLED_ShowHexNum(25, 48, sd_fifo.buffer[13], 2, OLED_6X8);
//				OLED_ShowHexNum(25, 56, sd_fifo.buffer[15], 2, OLED_6X8);
//				
//				OLED_Update();
				}
			}else
			{
				if(sd_fifo.data[SERVO0] == 837)
				{
					isRec = true;
				}
			}

			// 重置缓冲区索引
			sd_fifo.buffer_length = 0;
	 }
 }
}

////#define   BUFFER_SIZE    12
//static u8 buffer_index = 0;
//static u8 buffer[BUFFER_SIZE] = {0};

///***********************************************
//	函数名称:	void USART3_IRQHandler(void)
//	功能介绍:	串口3中断函数
//	函数参数:	无
//	返回值:		无
// ***********************************************/
//void USART3_IRQHandler(void)
//{
//	static bool isRec = false;
////	if (USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
//	if (USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
//	{
//		
//		OLED_ShowString(0, 0, "X5", OLED_6X8);
//		OLED_Update();
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//		buffer[buffer_index++] = USART_ReceiveData(USART3);
//		OLED_ShowNum( 0, 8, buffer_index, 2, OLED_6X8);
//		OLED_Update();
//		
//		// 当缓冲区满时处理
//		if (buffer_index == BUFFER_SIZE)
//		{
//			OLED_ShowString(0, 8, "entry", OLED_6X8);
//			OLED_Update();
//			// 处理接收到的两个整数
//			uint16_t data0 = ( ( uint16_t )buffer[1] << 8)  | buffer[2];
//			uint16_t data1 = ( ( uint16_t )buffer[3] << 8)  | buffer[4];
//			uint16_t data2 = ( ( uint16_t )buffer[5] << 8)  | buffer[6];
//			uint16_t data3 = ( ( uint16_t )buffer[7] << 8)  | buffer[8];
//			uint16_t data4 = ( ( uint16_t )buffer[9] << 8)  | buffer[10];
//			uint16_t data5 = ( ( uint16_t )buffer[11] << 8) | buffer[12];
//			uint16_t data6 = ( ( uint16_t )buffer[13] << 8) | buffer[14];
//			uint16_t data7 = ( ( uint16_t )buffer[15] << 8) | buffer[16];
//			
//			OLED_ShowHexNum(0, 8,  buffer[0], 2, OLED_6X8);
//			OLED_ShowHexNum(0, 16, buffer[2], 2, OLED_6X8);
//			OLED_ShowHexNum(0, 24, buffer[4], 2, OLED_6X8);
//			OLED_ShowHexNum(0, 32, buffer[6], 2, OLED_6X8);
//			OLED_ShowHexNum(0, 40, buffer[8], 2, OLED_6X8);
//			OLED_ShowHexNum(0, 48, buffer[10],2, OLED_6X8);
//			OLED_ShowHexNum(0, 56, buffer[12],2, OLED_6X8);
//			OLED_ShowHexNum(0, 0, buffer[14],2, OLED_6X8);
//					
//			OLED_ShowHexNum(25, 8,  buffer[1], 2, OLED_6X8);
//			OLED_ShowHexNum(25, 16, buffer[3], 2, OLED_6X8);
//			OLED_ShowHexNum(25, 24, buffer[5], 2, OLED_6X8);
//			OLED_ShowHexNum(25, 32, buffer[7], 2, OLED_6X8);
//			OLED_ShowHexNum(25, 40, buffer[9], 2, OLED_6X8);
//			OLED_ShowHexNum(25, 48, buffer[11], 2, OLED_6X8);
//			OLED_ShowHexNum(25, 56, buffer[13], 2, OLED_6X8);
//			OLED_ShowHexNum(25, 0, buffer[15], 2, OLED_6X8);

//			if( isRec  )
//			{
//				if (data0 < 2500 && data0>500 )
//				{
//					set_servo(SERVO0, data0, MV_Tim);
//					set_servo(SERVO1, data1, MV_Tim);
//					set_servo(SERVO2, data2, MV_Tim);
//					set_servo(SERVO3, data3, MV_Tim);
//					set_servo(SERVO4, data4, MV_Tim);
//					set_servo(SERVO5, data5, MV_Tim);
//					set_servo(SERVO6, data6, MV_Tim);
//		
//					// 显示在OLED屏幕上
////					OLED_ShowNum(50, 0,  data0, 4, OLED_6X8);
////					OLED_ShowNum(50, 8,  data1, 4, OLED_6X8);
////					OLED_ShowNum(50, 16, data2, 4, OLED_6X8);
////					OLED_ShowNum(50, 24, data3, 4, OLED_6X8);
////					OLED_ShowNum(50, 32, data4, 4, OLED_6X8);
////					OLED_ShowNum(50, 40, data5, 4, OLED_6X8);
////					OLED_ShowNum(50, 48, data6, 4, OLED_6X8);
////					OLED_ShowNum(50, 56, data7, 4, OLED_6X8);
////					OLED_Update();
//				}
//			}else
//			{
//				if(data0 == 0)
//				{
//					isRec = true;
//				}
//			}
//			// 重置缓冲区索引
//			buffer_index = 0; 
//	 }
// }
//}
