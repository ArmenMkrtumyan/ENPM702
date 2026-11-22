#Arrival message for robot reaching a checkpoint, either a room or door
std_msgs/Header header      #time when the robot arrived at checkpoint 

uint32 checkpoint_id        #which of the 4 checkpoints were reached


uint8 kind                  #type of checkpoint, door or room 

uint8 KIND_ROOM = 0         #room checkpoint

uint8 KIND_DOOR = 1         #door checkpoint