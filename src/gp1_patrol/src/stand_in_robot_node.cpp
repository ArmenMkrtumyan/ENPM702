#include "gp1_patrol/stand_in_robot_node.hpp"
#include <functional>
#include <chrono>
namespace gp1_patrol
{
  StandInRobotNode::StandInRobotNode():rclcpp::Node("stand_in_robot")
    // Your constructor goes here.
    {
      command_velocity_subscriber_ = create_subscription<geometry_msgs::msg::Twist>("cmd_vel", 10, std::bind(&StandInRobotNode::callback_command_velocity, this, std::placeholders::_1));
      odometry_publisher_=create_publisher<nav_msgs::msg::Odometry>("odometry/filtered", 10);
      scan_publisher_=create_publisher<sensor_msgs::msg::LaserScan>("scan", 10);
          
      last_time_updated_ = (*this).now();
      odometry_timer_ = create_wall_timer(std::chrono::milliseconds(50), std::bind(&StandInRobotNode::callback_odometry_timer, this));
      scan_timer_ = create_wall_timer(std::chrono::milliseconds(200), std::bind(&StandInRobotNode::callback_scan_timer, this));
    }



    void StandInRobotNode::callback_command_velocity(const geometry_msgs::msg::Twist::ConstSharedPtr msg)
    {
      linear_velocity_ = msg->linear.x;
      angular_velocity_ = msg->angular.z;
    }



    void StandInRobotNode::callback_odometry_timer()
    {
        auto time_now = (*this).now();
        double elapsed_time = (time_now - last_time_updated_).seconds();
        last_time_updated_ = time_now;
    }
}