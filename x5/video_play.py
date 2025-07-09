from hobot_vio import libsrcampy
import cv2
import numpy as np
import time
import threading
from video_state import video_state


def read_nv12_frames(file_path):
    with open(file_path, 'rb') as f:
        while True:
            # ????????1920x1080x3/2
            frame_size = 1024 * 600 * 3 // 2
            data = f.read(frame_size)
            if not data:
                break
            yield np.frombuffer(data, dtype=np.uint8)


def bgr2nv12_opencv(image):
    height, width = image.shape[0], image.shape[1]
    area = height * width
    yuv420p = cv2.cvtColor(image, cv2.COLOR_BGR2YUV_I420).reshape((area * 3 // 2,))
    y = yuv420p[:area]
    uv_planar = yuv420p[area:].reshape((2, area // 4))
    uv_packed = uv_planar.transpose((1, 0)).reshape((area // 2,))

    nv12 = np.zeros_like(yuv420p)
    nv12[:height * width] = y
    nv12[height * width:] = uv_packed
    return nv12

def test_display():

    # create a display object
    disp = libsrcampy.Display()
    # create a video object
    ret_0 = disp.display(0, 1024, 600, 0, 1)

            
    if ret_0 != 0:
        print("Video layer initialization failed")
        exit(-1)
    print("Video layer initialization successful")
    
    #cap = cv2.VideoCapture("./photo/show/cropped_waste_sort_2.mp4")
    nv12_frames = read_nv12_frames("./photo/show/cropped_waste_sort_2.bin")
    
    img_0 = cv2.imread("./photo/show/rec_waste.png")
    img_1 = cv2.imread("./photo/show/haz_waste.png")
    img_2 = cv2.imread("./photo/show/kit_waste.png")
    img_3 = cv2.imread("./photo/show/oth_waste.png")
    img_4 = cv2.imread("./photo/show/full.png")
    img_5 = cv2.imread("./photo/show/empty_0.png")
    img_6 = cv2.imread("./photo/show/empty_1.png")
    img_7 = cv2.imread("./photo/show/empty_2.png")

    img_nv12_0 = bgr2nv12_opencv(img_0)
    img_nv12_1 = bgr2nv12_opencv(img_1)
    img_nv12_2 = bgr2nv12_opencv(img_2)
    img_nv12_3 = bgr2nv12_opencv(img_3)
    img_nv12_4 = bgr2nv12_opencv(img_4)
    img_nv12_5 = bgr2nv12_opencv(img_5)
    img_nv12_6 = bgr2nv12_opencv(img_6)
    img_nv12_7 = bgr2nv12_opencv(img_7)

    while True:
        
        # stop_event is not set and show state is video
        while video_state.get_state() == video_state.show_dic['vid']:
            frame = next(nv12_frames)
            
            ret_0 = disp.set_img(frame.tobytes())
            
            print("Display set_img return: %d" % ret_0)

            # ???????????(??30fps)
            time.sleep(1 / 25)

        while video_state.get_state() == video_state.show_dic['rec']:
            j = 1.5
            while j >0:
                j-=0.1
                ret_0 = disp.set_img(img_nv12_0.tobytes())
                time.sleep(0.1)
            print("rec_waste")
            time.sleep(0.1)
        
        while video_state.get_state() == video_state.show_dic['haz']:
            j = 1.5
            while j >0:
                j-=0.1
                ret_0 = disp.set_img(img_nv12_1.tobytes())
                time.sleep(0.1)
            
            print("haz_waste")
        
        while video_state.get_state() == video_state.show_dic['kit']:
            j = 1.5
            while j >0:
                j-=0.1
                ret_0 = disp.set_img(img_nv12_2.tobytes())
                time.sleep(0.1)
            print("kit_waste")
            time.sleep(0.1)
        
        while video_state.get_state() == video_state.show_dic['oth']:
            j = 1.5
            while j >0:
                j-=0.1
                ret_0 = disp.set_img(img_nv12_3.tobytes())
                time.sleep(0.1)
            print("oth_waste")
            time.sleep(0.1)
            
        while video_state.get_state() == video_state.show_dic['ful']:
            j = 1.5
            while j >0:
                j-=0.1
                ret_0 = disp.set_img(img_nv12_4.tobytes())
                time.sleep(0.1)
            print("ful")
            time.sleep(0.1)        
            
        while video_state.get_state() == video_state.show_dic['emp']:
            ret_0 = disp.set_img(img_nv12_5.tobytes())
            time.sleep(0.1) 
            ret_0 = disp.set_img(img_nv12_6.tobytes())
            time.sleep(0.1) 
            ret_0 = disp.set_img(img_nv12_7.tobytes())
            time.sleep(0.1) 
            print("emp")
           
        
    cap.release()
    disp.close()
    print("Display closed successfully")



# strating video thread
def start_video_thread():

    # create a thread object, after start thread, then start test_display
    video_thread = threading.Thread(target = test_display)
    
    # start thread
    video_thread.start()
    
    return video_thread
    
