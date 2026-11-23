#pragma once

#include <array>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>

#include "gp1_patrol/checkpoints.hpp"



namespace gp1_patrol
{
class StandInRobotNode:public rclcpp::Node{


    public:
        explicit StandInRobotNode();


    private:
        
        void callback_command_velocity(const geometry_msgs::msg::Twist::ConstSharedPtr msg);
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr command_velocity_subscriber_;

        void callback_odometry_timer();
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odometry_publisher_;
        rclcpp::TimerBase::SharedPtr odometry_timer_;

        void callback_scan_timer();
        rclcpp::TimerBase::SharedPtr scan_timer_;
        rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_publisher_;

        double x_{0.0};
        double y_{0.0};
        double yaw_{0.0};
        double linear_velocity_{0.0};
        double angular_velocity_{0.0};

        rclcpp::Time last_time_updated_;

        int last_near_checkpoint_{-1};


};


}