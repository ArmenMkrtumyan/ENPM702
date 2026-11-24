# GP1: Patrol and Inspect

## Team

- **Member A, Sagar Vijayakumar:** `gp1_interfaces`, `stand_in_robot_node`, `patrol_log_node` (R1, R2, R3)
- **Member B, Armen Mkrtumyan:** `navigation_node`, the two patrol executables, the timing evidence (R4, R5)
- **Member C, Mubarak Babalola:** `mission_node`, `operator_node`, the launch files, the demo script (R6, R7)

## AI use

**Member A:** AI was used (ChatGPT) for parts R1, R2, R3 to help me understand and document concepts and structures, and review code and suggest fixes. Also, it helped with creating Doxygen comments and git commands.

**Member B:** AI was used (Claude) for parts R4 and R5 to review my code and README answers, help with git merges. Also, it helped to review the code I have written, suggesting fixes wherever necessary. It also polished the Doxygen comments.

**Member C:** AI (Gemini) was used to understand concepts and structure, how to write the Python launch files, and how git merge, pull and commit work. Claude was used to review and debug code and to suggest fixes for minor warnings. AI was also used to format the README for readability.

## Build and run

From the workspace root

```bash
colcon build
source install/setup.bash
```

The whole patrol, with one robot or with two:

```bash
ros2 launch gp1_patrol patrol_launch.py
ros2 launch gp1_patrol patrol_launch.py two_robots:=true
```

The demo: two robots for 75 s, output saved in `evidence/demo.log`:

```bash
./run_demo.sh
```

One robot without a launch file, each line in its own terminal:

```bash
ros2 run gp1_patrol stand_in_robot_exe --ros-args -r __ns:=/robot_1
ros2 run gp1_patrol patrol_exe --ros-args -r __ns:=/robot_1
ros2 run gp1_patrol mission_exe --ros-args -r __ns:=/robot_1
ros2 run gp1_patrol operator_exe
```

## Member A: R1, R2, R3

### R1: Custom interfaces

`gp1_interfaces` defines three messages: `Arrival` for checkpoint arrivals, `InspectionReport` for inspection results, and `StopCommand` for the operator's STOP and RESUME.

### R2: Stand-in robot

`stand_in_robot_node` subscribes to `cmd_vel` and moves an imaginary robot with its linear and angular velocity. Every 0.05 s it updates the position and heading and publishes `odometry/filtered`. Every 0.2 s it publishes a three-beam `scan`: 4.0 m by default, or the checkpoint's range within 0.3 m of a checkpoint.

### R3: Patrol log

`patrol_log_node` subscribes to `inspection_report` and counts clear rooms, blocked rooms, open doors and closed doors. Every 2 s it does 1.5 s of simulated work, then prints the counts.

## Member B: R4, R5

### R4: navigation_node

`navigation_node` drives the robot through checkpoints 1 to 4, in order, using only `odometry/filtered`.

Every 0.1 s it turns at 0.5 rad/s if the heading is off by more than 0.1 rad, otherwise it drives at 0.3 m/s. Within 0.15 m it publishes an `Arrival` and waits for the `InspectionReport` of that checkpoint before going to the next one.

On STOP from `/emergency_stop` it publishes a zero `Twist` right away and stays stopped until RESUME.

### R5: patrol executables

`patrol_single_exe` runs `navigation_node` and `patrol_log_node` in one `SingleThreadedExecutor`. `patrol_exe` runs them in one `MultiThreadedExecutor` with 2 threads. `patrol_launch.py` starts `patrol_exe`.

### R5: Table and answers

Logs: `evidence/patrol_single_exe_run1.log` to `run3.log` and `evidence/patrol_exe_run1.log` to `run3.log`.

| Run | STOP delay, `patrol_single_exe` (ms) | STOP delay, `patrol_exe` (ms) |
|---|---|---|
| 1 | 507.6 | 2.4 |
| 2 | 480.3 | 0.5 |
| 3 | 508.9 | 0.2 |

1. Why does the STOP wait in `patrol_single_exe`, and which callback does it wait for?

Since it's a SingleThreadedExecutor, there is only one thread, so the callbacks of both nodes run
one at a time, and each callback has to wait for the one before it to finish. The STOP waits for the patrol_log_node's timer callback, which sleeps 1.5 s every 2 s. The operator sends the STOP at 13 s,
while that callback is still sleeping (from 12 s to 13.5 s), so the STOP waited 480.3 to 508.9 ms.

2. Why does it not wait in `patrol_exe`, when you made no callback group? (Lecture 11: which group
does each node’s callback join?)

From the lecture we know that callbacks with no group join their node's default callback group. And
since each node has its own default group, the 2 nodes end up in 2 separate groups. The
MultiThreadedExecutor has 2 threads, so while one thread is busy in the log node's 1.5 s callback,
the other thread runs the navigation node's STOP callback: 0.2 to 2.4 ms.

3. Why do the navigation node’s callbacks stay in one mutually exclusive group? Say what could go wrong in a reentrant group.

The navigation node's callbacks share the same variables (such as `position`, `stopped_`,
`checkpoint_index_`). A mutually exclusive group makes sure that when one callback is working on them,
the others can't access them at the same time, which would cause race conditions. In a reentrant
group the callbacks can run at the same time. For example, the STOP callback can publish a zero
Twist while the control callback publishes a drive command right after it, so the robot keeps moving.

4. Why does the robot fail to arrive in `patrol_single_exe`?

While the log node sleeps for 1.5 seconds, the single-threaded executor cannot run the navigation node's 10 Hz control timer. So the robot keeps its last command and continues driving or turning without correction: 0.3 m/s × 1.5 s = 0.45 m, whereas the arrival distance was 0.15 m. In all 3 runs the robot logged
"near checkpoint 1", but the navigation node never logged "arrived at checkpoint 1". It kept
overshooting.

## Member C: R6, R7

### R6: mission_node and operator_node

`mission_node` keeps the latest `scan`. On each `Arrival` it reads the middle beam and publishes an `InspectionReport`: under 1.0 m a room is blocked and a door is closed, otherwise the room is clear and the door is open.

`operator_node` runs at the root namespace. 13 s after it starts it sends STOP on `/emergency_stop`, and 5 s later RESUME. It uses timers, not sleep.

### R7: Launch files

`robots_launch.py` starts `robot_1`, and `robot_2` only when `two_robots:=true`. Each robot is a group with its namespace and `patrol_exe`, `stand_in_robot_exe` and `mission_exe`.

`patrol_launch.py` includes `robots_launch.py`, starts `operator_exe` unless `with_operator:=false`, and prints both arguments.
