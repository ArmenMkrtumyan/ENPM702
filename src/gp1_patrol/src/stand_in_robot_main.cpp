/**
 * @file stand_in_robot_main.cpp
 * @author Sagar Vijayakumar
 * @brief stand_in_robot_exe (R2): runs one StandInRobotNode
 */

#include <memory>
#include <rclcpp/rclcpp.hpp>

#include "gp1_patrol/stand_in_robot_node.hpp"

/// @brief Runs the stand-in robot node until Ctrl+C
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
