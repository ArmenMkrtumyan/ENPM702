#include <memory>

#include <rclcpp/rclcpp.hpp>

#include "gp1_patrol/navigation_node.hpp"
#include "gp1_patrol/patrol_log_node.hpp"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto navigation{std::make_shared<gp1_patrol::NavigationNode>()};
    auto patrol_log{std::make_shared<gp1_patrol::PatrolLogNode>()};
    rclcpp::executors::MultiThreadedExecutor executor{
        rclcpp::ExecutorOptions{}, 2};
    executor.add_node(navigation);
    executor.add_node(patrol_log);
    executor.spin();
    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }
}
