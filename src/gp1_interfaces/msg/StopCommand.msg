# Command or state message for stopping or resuming a patrol mission
std_msgs/Header header # time when the command was sent

uint8 command # command to stop or resume the robot

string reason # reason for stopping or resuming the robot

uint8 STOP = 0  # halt movement

uint8 RESUME = 1 # resume to continue movement
