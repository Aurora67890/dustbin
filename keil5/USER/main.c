#include "main.h" /* 包含各类驱动文件 */

static uint32_t tick = 0;

int main(void)
{
    rcc_init();      /* 时钟初始化 */
    SysTick_Init();  /* 初始化系统嘀答定时器，1ms定时一次 */
	
		motor_init();
    servo_init();             /* 舵机初始化 */
		init_servo_pwm();  
	
		app_uart_init(); /*  初始化相关串口 */
		
    interrupt_open();  /* 初始化总中断 */
	
		uart3_send_byte(0x24);
		delay_ms(100);
		uart3_send_byte(0x30);
		delay_ms(100);
		uart3_send_byte(0x24);
		delay_ms(100);
		uart3_send_byte(0x30);
		delay_ms(100);
		uart3_send_byte(0x24);
		delay_ms(100);
		uart3_send_byte(0x30);

		TIM2_init(20000, 72 - 1); /* 初始化定时器2，用于pwm控制舵机 */
	
		OLED_Init();


    while (1)
    {		
				Show_aim_PWM();
////				set_servo(SERVO0, 500, MV_Tim);
////				set_servo(SERVO1, 1000, MV_Tim);
////				set_servo(SERVO2, 1500, MV_Tim);
////				set_servo(SERVO3, 2000, MV_Tim);
////				set_servo(SERVO4, 2500, MV_Tim);

    }
}
