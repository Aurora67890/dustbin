/****************************************************************************
 *	@笔者	：	Q
 *	@日期	：	2023年2月8日
 *	@所属	：	杭州友辉科技
 *	@功能	：	存放舵机相关的函数
 *	@函数列表:
 *	1.	void servo_init(void) -- 舵机gpio初始化
 *	2.	void servo_pin_set(u8 index, BitAction level) -- 设置舵机引脚电平函数
 *	3.	void duoji_doing_set(u8 index, int aim, int time) -- 设置舵机控制参数函数
 ****************************************************************************/
#include "./servo/y_servo.h"

servo_t duoji_doing[DJ_NUM];

/* 舵机gpio初始化 */
void servo_init(void)
{
//    u8 i;
    GPIO_InitTypeDef GPIO_InitStructure;
		
    RCC_APB2PeriphClockCmd(SERVO0_GPIO_CLK | SERVO1_GPIO_CLK | SERVO2_GPIO_CLK | SERVO3_GPIO_CLK | SERVO4_GPIO_CLK | SERVO5_GPIO_CLK | SERVO6_GPIO_CLK | SERVO7_GPIO_CLK, ENABLE); /* 使能 舵机 端口时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 打开AFIO时钟
		
		// 引脚重映射 关闭PB3 PB4的调试功能
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = SERVO0_PIN;         /* 配置引脚 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /* IO翻转50MHZ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* 推挽输出 */
    GPIO_Init(SERVO0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SERVO1_PIN;
    GPIO_Init(SERVO1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SERVO2_PIN;
    GPIO_Init(SERVO2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SERVO3_PIN;
    GPIO_Init(SERVO3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SERVO4_PIN;
    GPIO_Init(SERVO4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SERVO5_PIN;
    GPIO_Init(SERVO5_GPIO_PORT, &GPIO_InitStructure);
		
    GPIO_InitStructure.GPIO_Pin = SERVO6_PIN;
    GPIO_Init(SERVO6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SERVO7_PIN;
    GPIO_Init(SERVO7_GPIO_PORT, &GPIO_InitStructure);
		

}

void init_servo_pwm(void){
//		uint16_t data[8]= {837, 1520, 664, 2018, 1500, 837, 1500, 0};
		uint16_t i = 0;
		duoji_doing[0].aim = 837;
		duoji_doing[0].cur = 837;
		duoji_doing[0].inc = 0;
		duoji_doing[0].time = 5000;
		
		duoji_doing[1].aim = 1300;
		duoji_doing[1].cur = 1300;
		duoji_doing[1].inc = 0;
		duoji_doing[1].time = 5000;
	
		duoji_doing[2].aim = 1064;
		duoji_doing[2].cur = 1064;
		duoji_doing[2].inc = 0;
		duoji_doing[2].time = 5000;
	
		duoji_doing[3].aim = 2218;
		duoji_doing[3].cur = 2218;
		duoji_doing[3].inc = 0;
		duoji_doing[3].time = 5000;
		
		duoji_doing[4].aim = 1500;
		duoji_doing[4].cur = 1500;
		duoji_doing[4].inc = 0;
		duoji_doing[4].time = 5000;
		
		duoji_doing[5].aim = 1000;
		duoji_doing[5].cur = 1000;
		duoji_doing[5].inc = 0;
		duoji_doing[5].time = 5000;
//		
//		duoji_doing[6].aim = 1000;
//		duoji_doing[6].cur = 1000;
//		duoji_doing[6].inc = 0;
//		duoji_doing[6].time = 5000;
//		
//		duoji_doing[7].aim = 1000;
//		duoji_doing[7].cur = 1000;
//		duoji_doing[7].inc = 0;
//		duoji_doing[7].time = 5000;
		for (i = 6; i < DJ_NUM; i++)
		{
			duoji_doing[i].aim = 1350;
			duoji_doing[i].cur = 1350;
			duoji_doing[i].inc = 0;
			duoji_doing[i].time = 5000;
		}
	
}

/***********************************************
    功能介绍：	设置舵机引脚电平
    函数参数1：	index 要设置的舵机引脚索引
    函数参数2：	level 要设置的舵机引脚电平，1为高，0为低
    返回值：无
 ***********************************************/
void servo_pin_set(u8 index, BitAction level)
{
    switch (index)
    {
    case 0:
        SERVO0_PIN_SET(level);
        break;
    case 1:
        SERVO1_PIN_SET(level);
        break;
    case 2:
        SERVO2_PIN_SET(level);
        break;
    case 3:
        SERVO3_PIN_SET(level);
        break;
    case 4:
        SERVO4_PIN_SET(level);
        break;
    case 5:
        SERVO5_PIN_SET(level);
        break;
		case 6:
        SERVO6_PIN_SET(level);
        break;
		case 7:
        SERVO7_PIN_SET(level);
        break;
    default:
        break;
    }
}

/***********************************************
    功能介绍：	设置舵机控制参数函数
    函数参数：	index 舵机编号 aim 执行目标 time 执行时间(如果aim 执行目标==0，视为舵机停止)
    返回值：		无
 ***********************************************/
void duoji_doing_set(u8 index, int aim, int time)
{
    /* 限制输入值大小 */
    if (index >= DJ_NUM)
        return;

    if (aim == 0)
    {
        duoji_doing[index].inc = 0;
        duoji_doing[index].aim = duoji_doing[index].cur;
        return;
    }

    if (aim > 2490)
        aim = 2490;
    else if (aim < 510)
        aim = 510;

    if (time > 10000)
        time = 10000;

    if (duoji_doing[index].cur == aim)
    {
        aim = aim + 0.0077;
    }

    if (time < 20) /* 执行时间太短，舵机直接以最快速度运动 */
    {
        duoji_doing[index].aim = aim;
        duoji_doing[index].cur = aim;
        duoji_doing[index].inc = 0;
    }
    else
    {
        duoji_doing[index].aim = aim;
        duoji_doing[index].time = time;
        duoji_doing[index].inc = (duoji_doing[index].aim - duoji_doing[index].cur) / (duoji_doing[index].time / 20.000);
    }
}

/* 设置舵机每次增加的偏移量 */
void servo_inc_offset(u8 index)
{
    int aim_temp;

    if (duoji_doing[index].inc != 0)
    {

        aim_temp = duoji_doing[index].aim;

        if (aim_temp > 2490)
        {
            aim_temp = 2490;
        }
        else if (aim_temp < 500)
        {
            aim_temp = 500;
        }

        if (abs_float(aim_temp - duoji_doing[index].cur) <= abs_float(duoji_doing[index].inc + duoji_doing[index].inc))
        {
            duoji_doing[index].cur = aim_temp;
            duoji_doing[index].inc = 0;
        }
        else
        {
            duoji_doing[index].cur += duoji_doing[index].inc;
        }
    }
}

void Show_aim_PWM(void)
{
	OLED_ShowString(80,0,"aim pwm:",OLED_6X8);
	OLED_ShowNum(80, 8,  duoji_doing[SERVO0].aim, 4, OLED_6X8);
	OLED_ShowNum(80, 16, duoji_doing[SERVO1].aim, 4, OLED_6X8);
	OLED_ShowNum(80, 24, duoji_doing[SERVO2].aim, 4, OLED_6X8);
	OLED_ShowNum(80, 32, duoji_doing[SERVO3].aim, 4, OLED_6X8);
	OLED_ShowNum(80, 40, duoji_doing[SERVO4].aim, 4, OLED_6X8);
	OLED_ShowNum(80, 48, duoji_doing[SERVO5].aim, 4, OLED_6X8);
	OLED_ShowNum(80, 56, duoji_doing[SERVO6].aim, 4, OLED_6X8);
	OLED_ShowNum(105, 8, duoji_doing[PUSH0].aim, 4, OLED_6X8);
	OLED_Update();
}
