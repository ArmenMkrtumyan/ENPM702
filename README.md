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
the other thread runs the navigation node's STOP callback: 0.2 and 2.4 ms.

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

### R6: mission_node

`mission_node` keeps the latest `scan`. On each `Arrival` it reads the middle beam, `ranges[ranges.size() / 2]`, and publishes an `InspectionReport` with the checkpoint id, the result and the range used.

For a room, a range under 1.0 m is `RESULT_BLOCKED`, otherwise `RESULT_CLEAR`. For a door, a range under 1.0 m is `RESULT_DOOR_CLOSED`, otherwise `RESULT_DOOR_OPEN`. If an `Arrival` comes before any scan, it logs a warning and publishes nothing.

Its topics are relative, so a namespace applies to them: `scan`, `arrival` and `inspection_report`.

### R6: operator_node

`operator_node` runs at the root namespace and publishes on the absolute topic `/emergency_stop`. 13 s after it starts, a timer sends STOP with the reason `operator stop`. 5 s after that, a second timer sends RESUME with the reason `operator resume`. Each timer cancels itself after it fires, so each command is sent once. It uses timers only, no sleep.

### R7: Launch files

`robots_launch.py` declares `two_robots` (default `false`). The helper `make_robot(namespace, condition=None)` returns a `GroupAction` with `PushROSNamespace`, and `patrol_exe`, `stand_in_robot_exe` and `mission_exe`, with the condition on each of the three nodes. `robot_1` always starts. `robot_2` starts only when `two_robots` is true.

`patrol_launch.py` includes `robots_launch.py`, declares `with_operator` (default `true`) for `operator_exe`, and prints both arguments with `LogInfo`. An argument given on the outer command, such as `two_robots:=true`, reaches the included file.

### How to run

From the workspace root (the folder that contains `src/`):

```bash
colcon build --packages-up-to gp1_patrol
source install/setup.bash
```

Start the whole patrol:

```bash
ros2 launch gp1_patrol patrol_launch.py                      # robot_1 and the operator
ros2 launch gp1_patrol patrol_launch.py two_robots:=true     # robot_1, robot_2 and the operator
ros2 launch gp1_patrol patrol_launch.py with_operator:=false # no operator
ros2 launch gp1_patrol patrol_launch.py --show-args          # list the arguments
```

List the running nodes from a second terminal (source the workspace there too):

```bash 
ros2 node list
```

With `two_robots:=true` it should show:

``` expected
/operator_node
/robot_1/mission_node
/robot_1/navigation_node
/robot_1/patrol_log_node
/robot_1/stand_in_robot_node
/robot_2/mission_node
/robot_2/navigation_node
/robot_2/patrol_log_node
/robot_2/stand_in_robot_node
```

To run the nodes one at a time without a launch file:

```bash
ros2 run gp1_patrol mission_exe --ros-args -r __ns:=/robot_1
ros2 run gp1_patrol operator_exe
```

### R8: Demo

`run_demo.sh` builds the workspace, runs `patrol_launch.py` and writes the output to `evidence/demo.log`.
