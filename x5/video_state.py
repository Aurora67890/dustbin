class VideoState:

    show_dic = {
        'vid' :0,
        'rec' :1,
        'haz' :2,
        'kit' :3,
        'oth' :4,
        'ful' :5,
        'emp' :6
    }
    
    def __init__(self):
        self.show_state = self.show_dic['vid']
        
    def update_state(self, new_state):
        self.show_state = new_state
        
    def get_state(self):
        return self.show_state

video_state = VideoState()
