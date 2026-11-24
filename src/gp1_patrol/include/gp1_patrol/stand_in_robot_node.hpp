/**
 * @file stand_in_robot_node.hpp
 * @author Sagar Vijayakumar
 * @brief StandInRobotNode: plays the robot without hardware (R2)
 */

#pragma once


#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <chrono>
#include <functional>





namespace gp1_patrol
{
/**
 * @brief Moves an imaginary robot with the velocities from cmd_vel. Publishes
 * odometry/filtered at 20 Hz and a three-beam scan at 5 Hz, whose range follows
 * the checkpoint table. Logs once each time the robot comes within 0.3 m of a
 * checkpoint.
 */
class StandInRobotNode:public rclcpp::Node{


    public:
        /**
         * @brief Construct a new StandInRobotNode: the cmd_vel subscription,
         * the odometry and scan publishers, and the 20 Hz odometry and 5 Hz
         * scan timers. The robot starts at the origin, facing +x.
         */
        StandInRobotNode() : rclcpp::Node{"stand_in_robot_node"},
            command_velocity_subscriber_{create_subscription<geometry_msgs::msg::Twist>(
                    "cmd_vel", 10,
                    std::bind(&StandInRobotNode::callback_command_velocity, this, std::placeholders::_1))},
            odometry_publisher_{create_publisher<nav_msgs::msg::Odometry>("odometry/filtered", 10)},
            odometry_timer_{create_wall_timer(std::chrono::milliseconds{50},
                std::bind(&StandInRobotNode::callback_odometry_timer, this))},
            scan_timer_{create_wall_timer(std::chrono::milliseconds{200},
                std::bind(&StandInRobotNode::callback_scan_timer, this))},
            scan_publisher_{create_publisher<sensor_msgs::msg::LaserScan>("scan", 10)},
            last_time_updated_{now()} {}



    private:
        /**
         * @brief Stores the commanded velocities for the next odometry tick
         *
         * @param msg The command; only linear.x and angular.z are used
         */
        void callback_command_velocity(const geometry_msgs::msg::Twist::ConstSharedPtr msg);
        /// @brief Velocity commands, from cmd_vel
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr command_velocity_subscriber_;

        /**
         * @brief Runs at 20 Hz: moves the robot by the commanded velocities
         * times the time since the last tick, then publishes odometry/filtered
         */
        void callback_odometry_timer();
        /// @brief The robot's pose, on odometry/filtered
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odometry_publisher_;
        /// @brief Calls callback_odometry_timer every 50 ms
        rclcpp::TimerBase::SharedPtr odometry_timer_;

        /**
         * @brief Runs at 5 Hz: publishes the scan with the range of the
         * checkpoint within 0.3 m, or 4.0 m away from all of them, and logs
         * "stand-in robot near checkpoint N" when the robot enters that zone
         */
        void callback_scan_timer();
        /// @brief Calls callback_scan_timer every 200 ms
        rclcpp::TimerBase::SharedPtr scan_timer_;
        /// @brief The three-beam scan, on scan
        rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_publisher_;

        double x_{0.0}; ///< position x, in m
        double y_{0.0}; ///< position y, in m
        double yaw_{0.0}; ///< heading, in rad
        double linear_velocity_{0.0}; ///< last commanded linear.x, in m/s
        double angular_velocity_{0.0}; ///< last commanded angular.z, in rad/s

        rclcpp::Time last_time_updated_; ///< time of the last odometry tick

        int last_near_checkpoint_{-1}; ///< index of the checkpoint within 0.3 m; -1 = none


};


} // namespace gp1_patrol
