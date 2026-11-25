/**
 * @file navigation_node.hpp
 * @author Armen Mkrtumyan
 * @brief drives robot through four checkpoints
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
     * @brief Drives the robot through checkpoints 1 to 4, in order, using
     * odometry. At each checkpoint it publishes an Arrival and
     * waits for the InspectionReport of that checkpoint before it goes on.
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
            /**
             * @brief Stores the robot's latest position and heading
             *
             * @param msg the odometry: the yaw comes from its quaternion
             */
            void odometry_callback(const nav_msgs::msg::Odometry &msg);
            /**
             * @brief Goes on to the next checkpoint when the report of the
             * checkpoint the robot waits at arrives. After checkpoint 4, logs
             * "patrol complete".
             *
             * @param msg The report: one with another checkpoint_id is ignored
             */
            void report_callback(const gp1_interfaces::msg::InspectionReport &msg);
            /**
             * @brief Handles the operator's STOP or RESUME.
             *
             * @param msg The command thats sent. header.stamp is the time it was sent
             */
            void stop_callback(const gp1_interfaces::msg::StopCommand &msg);
            /**
             * @brief Runs at 10 Hz and publishes one command on cmd_vel: turns
             * at 0.5 rad/s while the heading is off by more than 0.1 rad,
             * otherwise drives at 0.3 m/s. Within 0.15 m of the checkpoint it
             * publishes the Arrival. Publishes zero while stopped, waiting for
             * a report, or done.
             */
            void control_callback();

            /// @brief Velocity commands to the robot, on cmd_vel
            rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr command_publisher_;
            /// @brief One message per checkpoint reached, on arrival
            rclcpp::Publisher<gp1_interfaces::msg::Arrival>::SharedPtr arrival_publisher_;
            /// @brief The robot's pose, from odometry/filtered
            rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odometry_subscription_;
            /// @brief The mission node's reports, from inspection_report
            rclcpp::Subscription<gp1_interfaces::msg::InspectionReport>::SharedPtr report_subscription_;
            /// @brief The operator's STOP and RESUME, from the absolute /emergency_stop
            rclcpp::Subscription<gp1_interfaces::msg::StopCommand>::SharedPtr stop_subscription_;
            /// @brief Calls control_callback every 100 ms
            rclcpp::TimerBase::SharedPtr control_timer_;
            double x_{0.0}; ///< position, in m
            double y_{0.0}; ///< position, in m
            double yaw_{0.0}; ///< heading, in rad
            bool has_odometry_{false}; ///< true once the first odometry arrived
            bool stopped_{false}; ///< true from a STOP to the next RESUME
            bool waiting_for_report_{false}; ///< true from an arrival to its report
            std::size_t checkpoint_index_{0}; ///< next checkpoint in kCheckpoints; 4 = patrol complete
    }; // class NavigationNode
} // namespace gp1_patrol
