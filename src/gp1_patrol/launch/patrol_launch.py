""" starts the whole patrol """

from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import (DeclareLaunchArgument, IncludeLaunchDescription, LogInfo)
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    with_operator = DeclareLaunchArgument(
        "with_operator", default_value="true", 
        description="Start operator_exe, sends STOP then Resume",
    )

    robot_file = PathJoinSubstitution([FindPackageShare("gp1_patrol"), "launch", "robots_launch.py"])

    robots = IncludeLaunchDescription(robot_file)
    log_arguments = LogInfo (msg=[ "two_robots = ", LaunchConfiguration("two_robots"),
    ", with_operator = ", LaunchConfiguration("with_operator"),])

    operator = Node(package="gp1_patrol", executable="operator_exe", 
                    condition=IfCondition(LaunchConfiguration("with_oprator")))

    return LaunchDescription([with_operator, robots, log_arguments, operator])
