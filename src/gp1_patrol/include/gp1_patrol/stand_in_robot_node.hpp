#pragma once


#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <chrono>
#include <functional>





namespace gp1_patrol
{
class StandInRobotNode:public rclcpp::Node{


    public:
        StandInRobotNode():rclcpp::Node{"stand_in_robot_node"}
            {
            command_velocity_subscriber_ = create_subscription<geometry_msgs::msg::Twist>("cmd_vel", 10, std::bind(&StandInRobotNode::callback_command_velocity, this, std::placeholders::_1));
            odometry_publisher_=create_publisher<nav_msgs::msg::Odometry>("odometry/filtered", 10);
            scan_publisher_=create_publisher<sensor_msgs::msg::LaserScan>("scan", 10);

            last_time_updated_ = (*this).now();
            odometry_timer_ = create_wall_timer(std::chrono::milliseconds{50}, std::bind(&StandInRobotNode::callback_odometry_timer, this));
            scan_timer_ = create_wall_timer(std::chrono::milliseconds{200}, std::bind(&StandInRobotNode::callback_scan_timer, this));
        
            }



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