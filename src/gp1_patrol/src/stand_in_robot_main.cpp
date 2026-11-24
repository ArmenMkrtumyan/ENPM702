/**
 * @file stand_in_robot_main.cpp
 * @author Sagar Vijayakumar
 * @brief Entry point for the stand-in robot ROS 2 node.
 *
 * This file initializes the ROS 2 communication system, creates
 * an instance of StandInRobotNode, and starts the ROS 2 executor
 * to process subscriber and timer callbacks.
 *
 * The node remains active while the executor is spinning.
 * When execution stops, the program shuts down ROS 2 if it
 * is still running and exits.
 */
#include <memory>
#include <rclcpp/rclcpp.hpp>

#include "gp1_patrol/stand_in_robot_node.hpp"

int main(int argc, char* argv[])
{
    // Initialize ROS 2
    rclcpp::init(argc, argv);

    // Create the stand-in robot node
    auto node{std::make_shared<gp1_patrol::StandInRobotNode>()};

    // Keep the node running and execute its callbacks
    rclcpp::spin(node);

    if (rclcpp::ok())
    {
        rclcpp::shutdown();
    }

    return 0;
}
