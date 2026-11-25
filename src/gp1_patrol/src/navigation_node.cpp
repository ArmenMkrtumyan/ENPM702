/**
 * @file navigation_node.cpp
 * @author Armen Mkrtumyan
 * @brief The methods of NavigationNode
 */

#include "gp1_patrol/navigation_node.hpp"

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <string>

#include "gp1_patrol/checkpoints.hpp"
#include "gp1_patrol/geometry.hpp"

namespace gp1_patrol {
    namespace {
        constexpr double heading_tolerance_rad{0.1}; ///< turn first if the heading is off by more
        constexpr double turn_speed_rps{0.5}; ///< angular.z while turning
        constexpr double drive_speed_mps{0.3}; ///< linear.x while driving
        constexpr double arrival_distance_m{0.15}; ///< arrived within this distance
    }

    void NavigationNode::odometry_callback(const nav_msgs::msg::Odometry &msg) {
        x_ = msg.pose.pose.position.x;
        y_ = msg.pose.pose.position.y;
        yaw_ = yaw_from_quaternion(msg.pose.pose.orientation);
        has_odometry_ = true;
    }

    void NavigationNode::control_callback() {
        geometry_msgs::msg::Twist command{};
        const bool patrol_complete{checkpoint_index_ == kCheckpoints.size()};
        if (has_odometry_ && !stopped_ && !waiting_for_report_ && !patrol_complete) {
            const Checkpoint &target{kCheckpoints[checkpoint_index_]};
            const double distance_m{distance_between(x_, y_, target.x, target.y)};
            if (distance_m <= arrival_distance_m) {
                gp1_interfaces::msg::Arrival arrival{};
                arrival.header.stamp = now();
                arrival.checkpoint_id = static_cast<std::uint32_t>(checkpoint_index_ + 1);
                arrival.kind = target.kind;
                RCLCPP_INFO_STREAM(get_logger(), "arrived at checkpoint " << arrival.checkpoint_id);
                arrival_publisher_->publish(arrival);
                waiting_for_report_ = true;
            }
            else {
                const double bearing{bearing_between(x_, y_, target.x, target.y)};
                const double heading_error{angle_difference(bearing, yaw_)};
                if (std::abs(heading_error) > heading_tolerance_rad) {
                    if (heading_error > 0.0)
                        command.angular.z = turn_speed_rps;
                    else
                        command.angular.z = -turn_speed_rps;
                }
                else {
                    command.linear.x = drive_speed_mps;
                }
            }
        }
        command_publisher_->publish(command);
    }

    void NavigationNode::report_callback(const gp1_interfaces::msg::InspectionReport &msg) {
        // Only the report of the checkpoint it waits at counts.
        if (!waiting_for_report_ || msg.checkpoint_id != checkpoint_index_ + 1)
            return;
        waiting_for_report_ = false;
        ++checkpoint_index_;
        if (checkpoint_index_ == kCheckpoints.size())
            RCLCPP_INFO_STREAM(get_logger(), "patrol complete");
    }

    void NavigationNode::stop_callback(const gp1_interfaces::msg::StopCommand &msg) {
        // How long the command took to arrive: now minus the time it was sent.
        const double delay_ms{(now() - rclcpp::Time{msg.header.stamp}).seconds() * 1000.0};
        std::string command_name{};
        if (msg.command == gp1_interfaces::msg::StopCommand::STOP) {
            command_publisher_->publish(geometry_msgs::msg::Twist{});
            stopped_ = true;
            command_name = "STOP";
        }
        else if (msg.command == gp1_interfaces::msg::StopCommand::RESUME) {
            stopped_ = false;
            command_name = "RESUME";
        }
        else {
            RCLCPP_WARN_STREAM(get_logger(), "unknown command " << static_cast<int>(msg.command) <<
                                ", ignored");
            return;
        }
        RCLCPP_INFO_STREAM(get_logger(), command_name << " (" << msg.reason << ") received "
                            << std::fixed << std::setprecision(1) << delay_ms <<
                            " ms after it was sent");
    }
} // namespace gp1_patrol
