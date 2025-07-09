import math
from math import pi

class Kinematics:
    def __init__(self):
        self.L0 = 0.0
        self.L1 = 0.0
        self.L2 = 0.0
        self.L3 = 0.0
        self.servo_angle = [0.0] * 6
        self.servo_range = [0.0] * 6
        self.servo_pwm = [0.0] * 6

def setup_kinematics(L0, L1, L2, L3, kinematics):
    kinematics.L0 = L0 * 10
    kinematics.L1 = L1 * 10
    kinematics.L2 = L2 * 10
    kinematics.L3 = L3 * 10

def kinematics_analysis(x, y, z, kinematics):
    theta3, theta4, theta5, theta6 = 0.0, 0.0, 0.0, 0.0
    l0, l1, l2, l3 = 0.0, 0.0, 0.0, 0.0
    aaa, a1, bbb, b1, ccc = 0.0, 0.0, 0.0, 0.0, 0.0
    flag_0, flag_1, flag_2, flag_3 = -1, -1, -1, 1

    x *= 10
    y *= 10
    z *= 10

    l0 = kinematics.L0
    l1 = kinematics.L1
    l2 = kinematics.L2
    l3 = kinematics.L3
    
    if y == 0:
        theta6 = 0.0
    else:
        theta6 = math.atan(y / x) * 180.0 / pi

    y = math.sqrt(x * x + y * y)
    z = l3 - l0 + z
    
    #print(f"y = {y}, z = {z}")
    
    if math.sqrt(y * y + z * z) > (l1 + l2):
        print("三角形：", math.sqrt(y * y + z * z) - l1 - l2)
        print("2")
        return 2

    ccc = math.acos(y / math.sqrt(y * y + z * z))
    b1 = (y * y + z * z + l1 * l1 - l2 * l2) / (2 * l1 * math.sqrt(y * y + z * z))
    if b1 > 1 or b1 < -1:
        print(f"b1 = {b1}")
        print("5")
        return 5
    bbb = math.acos(b1)

    theta5 = 90 - (bbb + ccc) * 180.0 / pi

    if theta5 > 90.0 or theta5 < -50.0:
        print(f"theta5 = {theta5}")
        print("theta5")
        return 6

    a1 = -(y * y + z * z - l1 * l1 - l2 * l2) / (2 * l1 * l2)
    if a1 > 1 or a1 < -1:
        print("3")
        return 3
    aaa = math.acos(a1)
    
    theta4 = 180.0 - aaa * 180.0 / pi
    if theta4 > 135.0:
        print(f"theta4 = {theta4}")
        print("4")
        return 4
 
    theta3 = 180 - theta4 - theta5

    kinematics.servo_angle[0] = theta6
    kinematics.servo_angle[1] = theta5
    kinematics.servo_angle[2] = theta4
    kinematics.servo_angle[3] = theta3

    kinematics.servo_pwm[0] = int(1500 + flag_0 * 2000.0 * kinematics.servo_angle[0] / 270.0)
    kinematics.servo_pwm[1] = int(1500 + flag_1 * 2000.0 * kinematics.servo_angle[1] / 270.0)
    kinematics.servo_pwm[2] = int(1500 + flag_2 * 2000.0 * kinematics.servo_angle[2] / 270.0)
    kinematics.servo_pwm[3] = int(1500 + flag_3 * 2000.0 * kinematics.servo_angle[3] / 270.0)
    kinematics.servo_pwm[4] = kinematics.servo_pwm[0]+50

    return 0

def kinematics_move(x, y, z, kinematics):
    if y < 0:
        return 0

    if kinematics_analysis(x, y, z, kinematics) != 0:
        print("无法运动")
        return 0

    for j in range(4):
        print(f"第{j}个舵机，角度为{kinematics.servo_angle[j]}")
        print(f"第{j}个舵机，pwm波为{kinematics.servo_pwm[j]}")
    return 1
