├─keil5/                  # STM32 Low-level Driver (Keil Project)
│  ├─SRC/                 # Peripheral Modules
│  │  ├─kinematics        # Kinematics Calculations
│  │  │      y_kinematics.c  # Kinematics Algorithms
│  │  │      y_kinematics.h
│  │  │
│  │  ├─servo             # Servo Control
│  │  │      y_servo.c    # PWM Signal Generation
│  │  │      y_servo.h
│  │  │
│  │  ├─timer             # Timer Management
│  │  │      y_timer.c    # Timer Interrupt Handling
│  │  │      y_timer.h
│  │  │
│  │  └─usart             # UART Communication
│  │          y_usart.c   # UART Configuration
│  │          y_usart.h
│  │
│  └─USER/                # Application Code
│          main.c         # Main Control Logic
│
└─x5/                     # Upper-level Control (Python)
        control111.py     # Main Controller
        kinematics.py     # Inverse Kinematics
        serial_0.py       # STM32 Serial Communication
        video_play.py     # Video Playback Control
        video_state.py    # Display Status Flags
        yolov11.py        # Vision Recognition (YOLO)

1. x5 Folder

kinematics.py：

    Inverse kinematics for target position → joint angles

serial_0.py：

    UART communication with STM32 (send/receive)

video_state.py：

    Display status flags management

video_play.py：

    UI video playback controller

yolov11.py：

    Object detection using YOLO algorithm

control111.py：

    Main coordinator for all modules


2. keil5 Folder

y_usart.c/h：

    115200 baud UART with interrupt handling


y_kinematics.c/h：

    Low-level kinematics assistance

    Joint angle limit protection

y_servo.c/h：

    Servo PWM control signal generation

    Multi-servo synchronization

y_timer.c/h：

    Timer interrupt configuration


main.c：

    Finite state machine implementation

    Module coordination core