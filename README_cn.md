├─keil5/                  # STM32 机械臂底层驱动 (Keil 工程)
│  ├─SRC/                 # 外设驱动模块
│  │  ├─kinematics        # 运动学计算
│  │  │      y_kinematics.c  # 运动学算法实现
│  │  │      y_kinematics.h
│  │  │
│  │  ├─servo             # 舵机控制
│  │  │      y_servo.c    # PWM 信号生成
│  │  │      y_servo.h
│  │  │
│  │  ├─timer             # 定时器管理
│  │  │      y_timer.c    # 定时中断处理
│  │  │      y_timer.h
│  │  │
│  │  └─usart             # 串口通信
│  │          y_usart.c   # 串口配置与中断
│  │          y_usart.h
│  │
│  └─USER/                # 用户代码
│          main.c         # 主控制逻辑
│
└─x5/                     # 上位机控制 (Python)
        control111.py     # 垃圾桶主控制器
        kinematics.py     # 机械臂逆解算法
        serial_0.py       # STM32 串口通信
        video_play.py     # 视频播放控制
        video_state.py    # 显示屏状态标志位
        yolov11.py        # 视觉识别模块

1. x5 文件夹

kinematics.py：

    机械臂逆解算法，计算目标位置对应的关节角度

serial_0.py：

    STM32 串口通信，接收传感器数据/发送控制指令

video_state.py：

    显示屏状态标志管理（垃圾桶状态/错误代码）

video_play.py：

    用户界面视频播放控制

yolov11.py：

    基于 YOLO 的物体识别模块

control111.py：

    主控制器，协调所有模块


2. keil5 文件夹

y_usart.c/h：

    115200 波特率串口通信

    中断接收 X5 的逆解数据包

y_kinematics.c/h：

    底层运动学辅助计算

    关节角度限位保护

y_servo.c/h：

    舵机 PWM 控制信号生成

    多舵机同步控制

y_timer.c/h：

    定时器中断配置

main.c：

    主状态机实现

    协调串口/定时器/舵机模块

