#!/bin/bash

source install/setup.bash
mkdir -p evidence
timeout -s INT 75 ros2 launch gp1_patrol patrol_launch.py two_robots:=true > evidence/demo.log 2>&1
grep "ms after" evidence/demo.log
grep "patrol complete" evidence/demo.log
