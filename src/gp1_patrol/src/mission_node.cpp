/**
 * @file mission_node.cpp
 * @author Mubarak Babalola
 * @brief methods for missionnode
 */

#include "gp1_patrol/mission_node.hpp"

#include <iomanip>
#include <string>

namespace gp1_patrol {

    using gp1_interfaces::msg::Arrival;
    using gp1_interfaces::msg::InspectionReport;

    void MissionNode::on_scan(const sensor_msgs::msg::LaserScan& msg) {
        latest_scan_ = msg;
    }

    void MissionNode::on_arrival(const Arrival& msg) {
        if (latest_scan_.ranges.empty()) {
            RCLCPP_WARN_STREAM(get_logger(), "arrival at checkpoint " << msg.checkpoint_id << " but no scan received yet");

            return;
        }

        const float range_m{latest_scan_.ranges[latest_scan_.ranges.size() / 2]};
        const bool is_short{range_m < short_range_m};

        InspectionReport report{};
        report.header.stamp = now();
        report.checkpoint_id = msg.checkpoint_id;
        report.range_m = range_m;

        std::string kind_name{};
        std::string result_name{};
        if (msg.kind == Arrival::KIND_DOOR) {
            kind_name = "door";
            report.result = is_short ? InspectionReport::RESULT_DOOR_CLOSED : InspectionReport::RESULT_DOOR_OPEN;
            result_name = is_short ? "door closed" : "door open";
        } else {
            kind_name = "room";
            report.result = is_short ? InspectionReport::RESULT_BLOCKED : InspectionReport::RESULT_CLEAR;
            result_name = is_short ? "blocked" : "clear";
        }

        RCLCPP_INFO_STREAM(get_logger(), "checkpoint " << msg.checkpoint_id << " (" << kind_name << "): "
                                                        << result_name << ", range " << std::fixed
                                                        << std::setprecision(1) << range_m << " m");
        report_publisher_ -> publish(report);
    }

} // namespace gp1_patrol
