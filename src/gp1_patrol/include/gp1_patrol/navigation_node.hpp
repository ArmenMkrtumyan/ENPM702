/**
 * @file navigation_node.hpp
 * @author Armen Mkrtumyan
 * @brief #TODO add the brief
 */

#pragma once

#include <chrono>
#include <cstddef>
#include <functional>

#include <geometry_msgs/msg/twist.hpp>
#include <gp1_interfaces/msg/arrival.hpp>
#include <gp1_interfaces/msg/inspection_report.hpp>
#include <gp1_interfaces/msg/stop_command.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>

namespace gp1_patrol {
    /**
     * @brief
     *
     */
    class NavigationNode : public rclcpp::Node {
        public:
            /**
             * @brief Construct a new Navigation Node object
             *
             */
            NavigationNode() : rclcpp::Node{"navigation_node"},
                command_publisher_{create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10)},
                arrival_publisher_{create_publisher<gp1_interfaces::msg::Arrival>("arrival", 10)},
                odometry_subscription_{create_subscription<nav_msgs::msg::Odometry>(
                        "odometry/filtered", 10,
                        std::bind(&NavigationNode::odometry_callback, this, std::placeholders::_1))},
                report_subscription_{create_subscription<gp1_interfaces::msg::InspectionReport>(
                        "inspection_report", 10,
                        std::bind(&NavigationNode::report_callback, this, std::placeholders::_1))},
                stop_subscription_{create_subscription<gp1_interfaces::msg::StopCommand>(
                        "/emergency_stop", 10,
                        std::bind(&NavigationNode::stop_callback, this, std::placeholders::_1))},
                control_timer_{create_wall_timer(std::chrono::milliseconds{100},
                    std::bind(&NavigationNode::control_callback, this))} {}

        private:
            void odometry_callback(const nav_msgs::msg::Odometry &msg);
            void report_callback(const gp1_interfaces::msg::InspectionReport &msg);
            void stop_callback(const gp1_interfaces::msg::StopCommand &msg);
            void control_callback();

            rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr command_publisher_;
            rclcpp::Publisher<gp1_interfaces::msg::Arrival>::SharedPtr arrival_publisher_;
            rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odometry_subscription_;
            rclcpp::Subscription<gp1_interfaces::msg::InspectionReport>::SharedPtr report_subscription_;
            rclcpp::Subscription<gp1_interfaces::msg::StopCommand>::SharedPtr stop_subscription_;
            rclcpp::TimerBase::SharedPtr control_timer_;
            double x_{0.0}; //< position, in m
            double y_{0.0};  //< position, in m
            double yaw_{0.0};  //< heading, in rad
            bool has_odometry_{false}; //< true once the first odometry arrived
            bool stopped_{false}; //< true from a STOP to the next RESUME
            bool waiting_for_report_{false}; //< true from an arrival to its report
            std::size_t checkpoint_index_{0}; //< next checkpoint in kCheckpoints; 4 = patrol complete
    }; // class NavigationNode
} // namespace gp1_patrol
