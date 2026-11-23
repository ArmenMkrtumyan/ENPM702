#include <memory>
#include <rclcpp/rclcpp.hpp>

#include "gp1_patrol/stand_in_robot_node.hpp"

int main(int argc, char* argv[])
{
    // Initialize ROS 2
    rclcpp::init(argc, argv);

    // Create the stand-in robot node
    auto node = std::make_shared<gp1_patrol::StandInRobotNode>();

    // Keep the node running and execute its callbacks
    rclcpp::spin(node);

    // Shut down ROS 2
    rclcpp::shutdown();

    return 0;
}