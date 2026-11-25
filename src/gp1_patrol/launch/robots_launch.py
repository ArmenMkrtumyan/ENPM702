"""starts one or two robots"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node, PushROSNamespace

def make_robot(namespace, condition=None) :
    """
    returns a robot, with namespace
    none conditon states robot always start, relevant for robot 2
    """

    return GroupAction(
        [
            PushROSNamespace(namespace), #pushnamespace into reach robot instance
            Node(package="gp1_patrol", executable="patrol_exe", condition=condition),
            Node(package="gp1_patrol", executable="stand_in_robot_exe", condition=condition),
            Node(package="gp1_patrol", executable="mission_exe", condition=condition),
        ]
    )

def generate_launch_description():
    two_robots = DeclareLaunchArgument("two_robots", default_value="false", description="Start robot_2 as well as robot_1",)

    second_robot = IfCondition(LaunchConfiguration("two_robots")) #condition states two robots

    return LaunchDescription(
        [
            two_robots,
            make_robot("robot_1"),
            make_robot("robot_2", second_robot),
        ]
    )
