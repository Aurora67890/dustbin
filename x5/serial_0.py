#!/usr/bin/env python3

import sys
import signal
import os
import time
import serial
import serial.tools.list_ports

data6 = 0
data7 = 0

def signal_handler(signal, frame):
    sys.exit(0)

def serialTest( kinematics ):
    
    signal.signal(signal.SIGINT, signal_handler)
    
    # uart_dev= input("?????????????:")
    uart_dev = '/dev/ttyS1'
    # baudrate = input("??????(9600,19200,38400,57600,115200,921600):")
    baudrate = '115200'
    try:
        ser = serial.Serial(port=uart_dev, baudrate=int(baudrate), bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=1) # 1s timeout
    except Exception as e:
        print("open serial failed!\n")
    
    data0 = int(kinematics.servo_pwm[0])
    data1 = int(kinematics.servo_pwm[1])
    data2 = int(kinematics.servo_pwm[2])
    data3 = int(kinematics.servo_pwm[3])
    data4 = int(kinematics.servo_pwm[4])
    data5 = int(kinematics.servo_pwm[5])
    #data6 = 0
    #data7 = 0
    
    ser.write(data0.to_bytes(2, byteorder='big'))
    time.sleep(0.1)
    ser.write(data1.to_bytes(2, byteorder='big'))
    time.sleep(0.1)
    ser.write(data2.to_bytes(2, byteorder='big'))
    time.sleep(0.1)
    ser.write(data3.to_bytes(2, byteorder='big'))
    time.sleep(0.1)
    ser.write(data4.to_bytes(2, byteorder='big'))
    time.sleep(0.1)
    ser.write(data5.to_bytes(2, byteorder='big'))
    time.sleep(0.1)
    ser.write(data6.to_bytes(2, byteorder='big'))
    time.sleep(0.1)
    ser.write(data7.to_bytes(2, byteorder='big'))
    #ser.write(b'\n')  # ?????
    
    #ser.write(data0.to_bytes(2, byteorder='little'))
    #ser.write(data1.to_bytes(2, byteorder='little'))
    #ser.write(data2.to_bytes(2, byteorder='little'))
    #ser.write(data3.to_bytes(2, byteorder='little'))
    #ser.write(data4.to_bytes(2, byteorder='little'))
    #ser.write(data5.to_bytes(2, byteorder='little'))

    print(f"Send:{data0} {data1} {data2} {data3} {data4} {data5} {data6} {data7}")

    ser.close()
    return 0

def serial_receive():
    uart_dev = '/dev/ttyS1'
    baudrate = '115200'
    try:
        ser = serial.Serial(port=uart_dev, baudrate=int(baudrate), bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=0.5) # 1s timeout
    except Exception as e:
        print("open serial failed!\n")
        
    data = 0
    # read  one bit
    data = ser.read(1)
    print("Received:", data.hex() if data else 'No data') 
    
    if data.hex() == '24':
        ser.close()
        print(24)
        return data.hex()
        
    elif data.hex() == '30':
        ser.close()
        print(30)
        return data.hex()
        
    else:
        ser.close()
        print(False)
        return False

