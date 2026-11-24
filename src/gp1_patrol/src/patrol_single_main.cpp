/**
 * @file patrol_single_main.cpp
 * @author Armen Mkrtumyan
 * @brief patrol_single_exe: robot's navigation_node & patrol_log_node in one SingleThreadedExecutor
 */

#include <memory>

#include <rclcpp/rclcpp.hpp>

#include "gp1_patrol/navigation_node.hpp"
#include "gp1_patrol/patrol_log_node.hpp"

/// @brief  Runs both nodes on one thread, so a callback waits while another one runs
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto navigation{std::make_shared<gp1_patrol::NavigationNode>()};
    auto patrol_log{std::make_shared<gp1_patrol::PatrolLogNode>()};
    rclcpp::executors::SingleThreadedExecutor executor{};
    executor.add_node(navigation);
    executor.add_node(patrol_log);
    executor.spin();
    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }
}
