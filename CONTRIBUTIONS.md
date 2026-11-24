AI Disclosure

Member A:
AI was used (ChatGPT) for parts R1, R2, R3 to help me understand and document concepts and structures, and review code and suggest fixes.
Also, it helped with creating Doxygen comments and git commands.

Member B:
AI was used (Claude) for parts R4 and R5 to review my code and README answers, help with git merges.
Also, it helped to review the code I have written, suggesting fixes wherever necessary. It also
polished the Doxygen comments.

## Member A:
- **Wrote:** `src/gp1_interfaces/msg/StopCommand.msg`,`src/gp1_interfaces/msg/Arrival.msg` and `src/gp1_interfaces/msg/InspectionReport.msg` for (R1). `stand_in_robot_node.hpp`, `stand_in_robot_node.cpp`, and `stand_in_robot_main.cpp` for R2 and `patrol_log_node.hpp` and `patrol_log_node.cpp` for (R3) and my sections of `README.md`.
- 
- **Tested:** Built and verified R1-R3 packages using colcon build and after un-commenting the appropriate CMakeList executables. Independently tested the stand-in robot using ROS 2 terminal commands. Verified node startup, topic availability, odometry publishing at approximately 20 Hz, and laser scan publishing at approximately 5 Hz. Published `Twist` messages to test forward motion, rotation, and stopping. Inspected odometry values to confirm position and orientation updates, and verified checkpoint detection and simulated laser scan readings of 4.0 m and 0.6 m. Also performed R3 compilation checks using a temporary CMake target but didn't independently test this.
- **Reviewed:** R4 and R5 codes and log files and merged it with main


## Member B: Armen Mkrtumyan

- **Wrote:** `navigation_node` (R4), `patrol_single_exe` and `patrol_exe` with their CMake entries (R5), the Doxygen comments for these files, the six timing logs in `evidence/`, and my sections of `README.md`.
- **Tested:** `navigation_node` with the stand-in robot and STOP/RESUME sent with `ros2 topic pub`; both executables three times each with the stand-in, mission and operator nodes.
- **Reviewed:** All the changes of other team members. Given them comments via github and confirmed
Pull Requests before they were merged

## Member C:
