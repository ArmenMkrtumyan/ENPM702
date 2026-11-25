/**
 * @file mission_main.cpp
 * @author Mubarak Babalola
 * @brief Entry point for mission exe
 */

#include <memory>

#include <rclcpp/rclcpp.hpp>

#include "gp1_patrol/mission_node.hpp"

/**
 * @brief starts ros2 spins a missionnode and shutsdown
 *
 * @param argc number of command-line argument
 * @param argv the argument
 */
int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node{std::make_shared<gp1_patrol::MissionNode>()};
    rclcpp::spin(node);
    if(rclcpp::ok()) {
        rclcpp::shutdown();
    }
}
