/**
 * @file operator_main.cpp
 * @author mbabalol
 * @brief Entry point for operator_exe:
 * 
 */

#include <memory>

#include <rclcpp/rclcpp.hpp>

#include "gp1_patrol/operator_node.hpp"

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node {std::make_shared<gp1_patrol::OperatorNode>()};
    rclcpp::spin(node);
    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }
}

