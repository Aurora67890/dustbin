import time
import cv2
import Hobot.GPIO as GPIO

import serial_0 as se
import kinematics as ki 
import yolov11
from video_play import start_video_thread
from video_state import video_state

coco_names = [
    "recyclable_waste", "hazardous_waste", "kitchen_waste", "other_waste"
    ]############ change to your labels

servo6_dict = {"horizontal": 1300, "higher": 1700, "dump_rec": 1100, "dump_oth": 800}# horizontal:1400(1325), dump_rec:1150//1050

jaw_dict = {"opening": 500, "closing": 1100, "half_opening": 600}


thresholds = {
    'w': 5,   
    'h': 5,   
    'x': 5,   
    'y': 5,   
    'conf': 0.1  
}


def full_sensor_0():
    input_pin = 16
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(input_pin, GPIO.IN)

    value = GPIO.input(input_pin)
    print(f"value:{value}")
    
    if value == 1:
        GPIO.cleanup()
        return True
    
    return False
    
def full_sensor_1():
    input_pin = 31
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(input_pin, GPIO.IN)

    value = GPIO.input(input_pin)
    print(f"value:{value}")
    
    if value == 1:
        GPIO.cleanup()
        return True
    
    return False


def motor_moving():
    output_pin_0 = 29
    output_pin_1 = 37
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(output_pin_0, GPIO.OUT, initial=GPIO.HIGH)
    GPIO.setup(output_pin_1, GPIO.OUT, initial=GPIO.HIGH)
    
    # pushing
    #GPIO.output(output_pin_0, GPIO.LOW)
    #GPIO.output(output_pin_1, GPIO.HIGH)
    #time.sleep(5)#9
    
    # pulling
    GPIO.output(output_pin_0, GPIO.HIGH)
    GPIO.output(output_pin_1, GPIO.LOW)
    time.sleep(3)

    GPIO.output(output_pin_0, GPIO.HIGH)
    GPIO.output(output_pin_1, GPIO.HIGH)

    GPIO.cleanup()

def reset32():
    output_pin = 37 # BOARD37
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(output_pin, GPIO.OUT, initial=GPIO.HIGH)
    GPIO.output(output_pin, GPIO.LOW)
    time.sleep(0.1)
    GPIO.output(output_pin, GPIO.HIGH)
    GPIO.cleanup()
    print("reset sucess")

def send_data():
    se.serialTest( kinematics )
    #i = 0.2
    #while i >0:
        #i-=0.1
        #se.serialTest( kinematics )
        #time.sleep(0.1)
        
def camera_to_robot(camera_x, camera_y):

    a = 0.33394426130996147
    b = 0.01115911585640252
    c = 50.78577281294428 
    
    d = 0.023926167680919395
    e = 0.2931151821949718
    f = 0.1495716233249852
    
    robot_x = (a * camera_x) + (b * camera_y) + c
    robot_y = (d * camera_x) + (e * camera_y) + f
    
    return robot_x, robot_y

def init_mode():

    ki.setup_kinematics(85.0, 188.0, 116.0, 210.0, kinematics )#75,standard_mode
    
    if ki.kinematics_move(1, 150, 70.0, kinematics):
        # must be 837
        kinematics.servo_pwm[0] = 837
        
        # keep horizontal
        kinematics.servo_pwm[4] = kinematics.servo_pwm[4] + 700
        kinematics.servo_pwm[5] = jaw_dict["opening"]
        se.data6 = servo6_dict["horizontal"]
        se.data7 = 1300
        #change fifth servo in the kinematics
        send_data()
        print("-----------send_data---------------")

# mode:   

def rec_waste_mode_push0(data, robot_x):

    if ki.kinematics_move(robot_x, 30, 70.0, kinematics):
        kinematics.servo_pwm[4] = kinematics.servo_pwm[4] #+ 700
        kinematics.servo_pwm[5] = jaw_dict["opening"]
        se.data6 = data
        se.data7 = 1300
        send_data()
        print("-----------send_data---------------")
    
    kinematics.servo_pwm[5] = jaw_dict["closing"]
    send_data()
    
    if ki.kinematics_move(robot_x, 30, 0.0, kinematics):
        kinematics.servo_pwm[4] = kinematics.servo_pwm[4] #+ 700
        kinematics.servo_pwm[5] = jaw_dict["closing"]
        se.data6 = data
        se.data7 = 1300
        send_data()
        print("-----------send_data---------------")

    
def rec_waste_mode_push1(data, robot_x):

    if ki.kinematics_move(150, 200, 0.0, kinematics):
        kinematics.servo_pwm[4] = kinematics.servo_pwm[4] #+ 700
        kinematics.servo_pwm[5] = jaw_dict["closing"]
        se.data6 = data
        se.data7 = 1300
        send_data()
        print("-----------send_data---------------")
        
    
    if ki.kinematics_move(150, 100, 70.0, kinematics):
        kinematics.servo_pwm[4] = kinematics.servo_pwm[4] #+ 700
        kinematics.servo_pwm[5] = jaw_dict["closing"]
        se.data6 = data
        se.data7 = 1300
        send_data()
        print("-----------send_data---------------")


def rec_waste_mode(data):
    kinematics.servo_pwm[0] = 837
    kinematics.servo_pwm[1] = 1300
    kinematics.servo_pwm[2] = 1064
    kinematics.servo_pwm[3] = 2218
    kinematics.servo_pwm[4] = 1000
    # opening jaw, need to find out
    kinematics.servo_pwm[5] = jaw_dict["opening"]
    
    # shuiping
    se.data6 = data
    
    se.data7 = 1300
    send_data()
 
def kit_waste_mode(value):

    if ki.kinematics_move(1, 100, 70.0, kinematics):
    
        kinematics.servo_pwm[4] = kinematics.servo_pwm[4] + 700
        kinematics.servo_pwm[5] = value
        
        se.data6 = servo6_dict["horizontal"]
        se.data7 = 1300
        
        send_data()
        print("-----------send_data---------------")
    
    
def oth_waste_mode():
    if ki.kinematics_move(1, 230, 70.0, kinematics):
        # keep horizontal
        kinematics.servo_pwm[4] = kinematics.servo_pwm[4] + 700
        kinematics.servo_pwm[5] = jaw_dict["closing"]
        
        se.data6 = servo6_dict["horizontal"]
        se.data7 = 1300
        
        send_data()
        print("-----------send_data---------------") 

def haz_waste_mode(data):

    if ki.kinematics_move(1, 150, 70.0, kinematics):
        # keep horizontal
        kinematics.servo_pwm[4] = kinematics.servo_pwm[4] + 700
        kinematics.servo_pwm[5] = jaw_dict["opening"]
        
        se.data6 = data
        se.data7 = 1300
        
        send_data()
        print("-----------send_data---------------")


# catching:
def rec_multiple(robot_x):

    se.data6 = servo6_dict["dump_rec"]
    send_data()
    
    robot_x_min_threshold = 130
    robot_x_max_threshold = 200

    if robot_x < robot_x_min_threshold:
        robot_x = robot_x_min_threshold
        
    if robot_x > robot_x_max_threshold:
        robot_x = robot_x_max_threshold
        
    
    #rec_pushing_0,,,change its position based on rec_wate x,y
    rec_waste_mode_push0(servo6_dict["dump_rec"], robot_x)
    
    se.data6 = servo6_dict["horizontal"]
    send_data()
    
    # rec_pushing_1
    rec_waste_mode_push1(servo6_dict["horizontal"], robot_x)
    
    if ki.kinematics_move(1, 220, 70.0, kinematics):
        # keep horizontal
        kinematics.servo_pwm[4] = kinematics.servo_pwm[4] + 700
        kinematics.servo_pwm[5] = jaw_dict["opening"]
        
        se.data6 = servo6_dict["horizontal"]
        se.data7 = 1300
        
        send_data()
        print("-----------send_data---------------") 

def kit_catching(robot_x, robot_y):

    kinematics.servo_pwm[5] = jaw_dict["closing"]
    send_data()
    
    if ki.kinematics_move(robot_x, robot_y, 70.0, kinematics):

        kinematics.servo_pwm[5] = jaw_dict["closing"]
        se.data6 = servo6_dict["horizontal"]
        se.data7 = 1300
        send_data()
        print("-----------send_data---------------")
    
    oth_waste_mode()
    kit_waste_mode(jaw_dict["closing"])
    
    kinematics.servo_pwm[5] = jaw_dict["opening"]
    send_data()
    init_mode()
    
def haz_catching():

    haz_waste_mode(servo6_dict["dump_oth"])
    time.sleep(0.5)
    haz_waste_mode(servo6_dict["horizontal"])

def oth_catching(robot_x, robot_y):
    
    kinematics.servo_pwm[5] = jaw_dict["closing"]
    send_data()
    
    if ki.kinematics_move(robot_x, robot_y, 70.0, kinematics):

        kinematics.servo_pwm[5] = jaw_dict["closing"]
        se.data6 = servo6_dict["horizontal"]
        se.data7 = 1300
        send_data()
        print("-----------send_data---------------")

    oth_waste_mode()
    
    kinematics.servo_pwm[5] = jaw_dict["opening"]
    send_data()
    init_mode()

def switching(result):
    x_center = result[0]['x']+result[0]['w']/2
    y_center = result[0]['y']+result[0]['h']/2
    print(f"--------------x_center:{x_center}, y_center:{y_center}--------------------")
    
    robot_x_min_threshold = 130
    robot_x_max_threshold = 200
    
    robot_y_min_threshold = 30
    robot_y_max_threshold = 135

    robot_x, robot_y = camera_to_robot(x_center, y_center)

    x_edge = 0
    y_edge = 0
    
    if robot_x < robot_x_min_threshold:
        robot_x = robot_x_min_threshold
        x_edge = 1 
        
    if robot_x > robot_x_max_threshold:
        robot_x = robot_x_max_threshold
        x_edge = 1

    if robot_y < robot_y_min_threshold:
        robot_y = robot_y_min_threshold
        y_edge = 1
        
    if robot_y > robot_y_max_threshold:
        robot_y = robot_y_max_threshold
        y_edge = 1
        
    print(f"--------------robot_x:{robot_x}, robot_y:{robot_y}--------------------")
    
    if ki.kinematics_move(robot_x, robot_y, 70.0, kinematics):
        if x_edge == 1 or y_edge == 1:
            kinematics.servo_pwm[4] = kinematics.servo_pwm[4] + 700
            
        elif x_edge == 1:
            kinematics.servo_pwm[4] = kinematics.servo_pwm[4]
            
        elif y_edge == 1:
            kinematics.servo_pwm[4] = kinematics.servo_pwm[4] + 700
            
        send_data()
        print("-----------send_data---------------")
        
    if ki.kinematics_move(robot_x, robot_y, -10.0, kinematics):
        if x_edge == 1 or y_edge == 1:
            kinematics.servo_pwm[4] = kinematics.servo_pwm[4] + 700
            
        elif x_edge == 1:
            kinematics.servo_pwm[4] = kinematics.servo_pwm[4]
            
        elif y_edge == 1:
            kinematics.servo_pwm[4] = kinematics.servo_pwm[4] + 700
            
        send_data()
        print("-----------send_data---------------")
            
    return robot_x, robot_y

def compare_result(result1, result2):
    
    data = 0
    # judge if empty
    if result1 and result2:
        required_keys = ['w', 'h', 'x', 'y', 'conf']
        differences = {
            'w': abs(result1[0]['w'] - result2[0]['w']),
            'h': abs(result1[0]['h'] - result2[0]['h']),
            'x': abs(result1[0]['x'] - result2[0]['x']),
            'y': abs(result1[0]['y'] - result2[0]['y']),
            'conf': abs(result1[0]['conf'] - result2[0]['conf'])
        }
        # judge thresholds
        if all(differences[key] <= thresholds[key] for key in required_keys):
            # the same
            data = 0 
        else:
            # not the same
            data = 1 
    else:
        # not the same
        data = 1 
    
    return data

def if_success(class_type):
    
    j = 0.2
    while j >0:
        j-=0.01
        _ ,frame = cap.read()
        time.sleep(0.01)
    
    #clear_buffer(cap) 
    _ ,frame = cap.read()
    if frame is None:
        print("Failed to get image from usb camera")
    
    cv2.imwrite('./photo/inference/capture_photo/capture.jpg', frame)
    print("-------------------capture_new_photo-------------------------")
    
    img_path = './photo/inference/capture_photo/capture.jpg'
    # get yolo result
    results = yolov11.predict(model, img_path, "./photo/inference/result/result.jpg", coco_names)
    
    for result in results:
        if result['class'] == class_type and result['conf'] > 0.7:
            return False
    
    return True
    #if results == []:
        #return True
    #else:
        #return False

if __name__ == '__main__':
    
    #motor_moving()
    
    
    if_break_24 = 0
    if_break_30 = 0
    # if can not read "0x01" from stm32, keep reset
    
    
    while True:
        reset32()
        for i in range(2):
            result = se.serial_receive()
            if result == '24': 
                if_break_24 += 1
                print(f"if_break_24: {if_break_24}")
                
            elif result == '30':
                if_break_30 += 1
                print(f"if_break_30: {if_break_30}")
                    
        if if_break_24 >= 1 and if_break_30 >= 1:
            break
    
    time.sleep(1)
  
    # start video
    video_state.update_state(video_state.show_dic['vid'])
    video_thread = start_video_thread()
    
    cap = cv2.VideoCapture(0) 
    if not cap.isOpened():
        print("can not open")
        exit()
        
    codec = cv2.VideoWriter_fourcc( 'M', 'J', 'P', 'G' )
    cap.set(cv2.CAP_PROP_FOURCC, codec)
    # fps
    cap.set(cv2.CAP_PROP_FPS, 30)
    # width
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    # height
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
        
    rec_result = []
    haz_result = []
    kit_result = []
    oth_result = []
    last_results = []
    
    kinematics = ki.Kinematics()

    init_mode()
    init_mode()

    
    model_path = "./models/yolo11s_detect.bin"
    model = yolov11.load_model(model_path, conf_thres = 0.7, iou_thres = 0.45)
    
    # state
    empty_state = 0
    
    rec_num = 0
    
    empty_num_0 = 0
    empty_num_1 = 0
    empty_num_max = 5
    
    while True:
        
        #if if_success_current_state == if_success_state["Yes"]:
            #if_success_current_state = if_success_state["No"]
            
            #se.data6 = servo6_dict["higher"]
            #send_data()
         
        rec_result = []
        haz_result = []
        kit_result = []
        oth_result = []
        
        j = 0.2
        while j >0:
            j-=0.01
            _ ,frame = cap.read()
            time.sleep(0.01)
        
        #clear_buffer(cap) 
        _ ,frame = cap.read()
        if frame is None:
            print("Failed to get image from usb camera")
        
        cv2.imwrite('./photo/inference/capture_photo/capture.jpg', frame)
        print("-------------------capture_new_photo-------------------------")
        
        img_path = './photo/inference/capture_photo/capture.jpg'
        # get yolo result
        results = yolov11.predict(model, img_path, "./photo/inference/result/result.jpg", coco_names)
        
        print(f"------------results:{results}----------------")
        # get yolo result
        
        results = [item for item in results if item.get('conf', 0) > 0.7]
        
        if results == []:
            print("No results available. Skipping this frame.")
            print(f"empty_num_0:{empty_num_0}")
            
            if empty_num_1 == 2:
                empty_num_0 = 0      
                empty_num_1 = 0
                
                se.data6 = servo6_dict["dump_oth"]
                send_data()
                video_state.update_state(video_state.show_dic['haz'])
                
                se.data6 = servo6_dict["horizontal"]
                send_data()   
            
            empty_num_0 += 1
            
            if empty_num_0 >= empty_num_max:
                empty_num_0 = 0
                
                se.data6 = servo6_dict["higher"]
                send_data()
                
                se.data6 = servo6_dict["horizontal"]
                send_data() 
                empty_num_1 += 1
                
            if empty_state == 1:
                video_state.update_state(video_state.show_dic['emp'])
          
        else:
            empty_num_0 = 0
            empty_num_1 = 0
            
            for result in results:
                if result['class'] == 'recyclable_waste' and result['conf'] > 0.7:
                    rec_result.append(result)
                elif result['class'] == 'hazardous_waste' and result['conf'] > 0.7:
                    haz_result.append(result)
                elif result['class'] == 'kitchen_waste' and result['conf'] > 0.7:
                    kit_result.append(result)
                elif result['class'] == 'other_waste' and result['conf'] > 0.7:
                    oth_result.append(result)
                        
            print("recyclable_waste:")
            print(rec_result)
            print("\nhazardous_waste:")
            print(haz_result)
            print("\nkitchen_waste:")
            print(kit_result)
            print("\nother_waste:")
            print(oth_result)
            
            #se.data6 = servo6_dict["horizontal"]
            #send_data()
            
            if empty_state == 1:
                video_state.update_state(video_state.show_dic['emp'])
           
            if rec_result != []:
                print("-----------------catching_recyclable_waste---------------------")
                
                x_center = rec_result[0]['x']+rec_result[0]['w']/2
                y_center = rec_result[0]['y']+rec_result[0]['h']/2
                
                robot_x, robot_y = camera_to_robot(x_center, y_center)
                print(f"--------------robot_x:{robot_x}, robot_y:{robot_y}--------------------")
                rec_multiple(robot_x)
                
                time.sleep(0.1)
                #if if_success('recyclable_waste'):
                video_state.update_state(video_state.show_dic['rec'])
                    
                empty_state = 1
                rec_num = rec_num+1
            
            elif oth_result != []:
                print("-----------------catching_other_waste---------------------")
                #########
                if oth_result[0]['y'] > 400:
                    se.data6 = servo6_dict["higher"]
                    send_data()                
                    se.data6 = servo6_dict["horizontal"]
                    send_data()
                    
                else:
                    robot_x, robot_y = switching(oth_result)
                    oth_catching(robot_x, robot_y)
                    
                    #if if_success('other_waste'):
                    video_state.update_state(video_state.show_dic['oth'])
                        
                    empty_state = 1
                ########

                    
            elif kit_result != []:
                #if not compare_result(results, last_results):
                print("-----------------catching_kitchen_waste---------------------")
                
                if kit_result[0]['y'] > 400:
                    se.data6 = servo6_dict["higher"]
                    send_data()                
                    se.data6 = servo6_dict["horizontal"]
                    send_data()
                    
                else:
                    robot_x, robot_y = switching(kit_result)
                    kit_catching(robot_x, robot_y)
                    
                    #if if_success('kitchen_waste'):
                    video_state.update_state(video_state.show_dic['kit'])
                    
                    empty_state = 1
            
            elif haz_result != []:
                #if not compare_result(results, last_results):
                print("-----------------catching_hazarous_waste---------------------")

                haz_catching()
                
                time.sleep(0.1)
                #video_state.update_state(video_state.show_dic['haz'])
                
                empty_state = 1


        last_results = results
        print(f"--------------------last_results={last_results}--------------")
        
        
        #except Exception as e:
            #print(f"An error occurred while processing the image: {e}")
            #print("Skipping this frame and continuing with the next one.")
            #continue  # ???????
        

    cap.release()
    cv2.destroyAllWindows()