# Published after evaluating a range scan at a checkpoint

std_msgs/Header header          # Timestamp when the inspection report was created

uint32 checkpoint_id            # Checkpoint index inspected (1 to 4)

uint8 result                    # Inspection outcome constant (RESULT_*)

float32 range_m                 # Range reading in meters used for the decision

uint8 RESULT_CLEAR = 0          # Room is clear

uint8 RESULT_BLOCKED = 1        # Room is blocked

uint8 RESULT_DOOR_OPEN = 2      # Door is open

uint8 RESULT_DOOR_CLOSED = 3    # Door is closed
