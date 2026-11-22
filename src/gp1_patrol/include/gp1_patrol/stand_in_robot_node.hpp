#pragma once

#include <array>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>

#include "include/gp1_patrol/checkpoints.hpp"


namespace gp1_patrol {

class StandInRobotNode:public rclcpp::Node{


    public:
        explicit StandInRobotNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());


    private:
        void on_command_velocity(const geometry_msgs::msg::Twist::SharedPtr msg);
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscribe_;

        void odometry_timer();
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_publish_;
        rclcpp::TimerBase::SharedPtr odom_timer_;

        void scan_timer();
        rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_publish_;
        rclcpp::TimerBase::SharedPtr scan_timer_;

        double x_{0.0};
        double y_{0.0};
        double yaw_{0.0};
        double linear_x_{0.0};
        double angular_z_{0.0};

        
        bool has_prev_odom_time_{false};
        rclcpp::Time prev_odom_time_{0, 0, RCL_ROS_TIME};
        
        std::array<bool, kCheckpoints.size()> near_logged_{};



};



}