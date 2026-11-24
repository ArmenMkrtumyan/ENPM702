/**
 * @file mission_node.hpp
 * @author mbabalola
 * @brief Declearation of MissionNode, turns attival into inpesction reports
 * 
 */
#pragma once

#include <functional>

#include <gp1_interfaces/msg/arrival.hpp>
#include <gp1_interfaces/msg/inspection_report.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>


namespace gp1_patrol {

    /**
     * @brief Inspect a Checkpoint when robot arrives.
     * 
     * Node keeps latest laser scan, on each arrival reads middle beam and publishes inspection report
     * which includes clear/blocked rooms, doors; open/closed door. A range under kShortRange_m coun ts as short.
     * 
     * Topics; Subscribe(sensor_msgs/msg/LaserScan, gp1_interfaces/msg/Arrival)
     *         publish(gp1_interfaces/msg/InspectionReport)
     * 
     */
    class MissionNode : public rclcpp::Node {
        public:
        /**
         * @brief Construct a new Mission Node object and its publisher and 2 subscribition.
         * 
         */
        MissionNode() : rclcpp::Node{"mission_node"}, 
        scan_subscription_{create_subscription < sensor_msgs::msg::LaserScan > (
            "scan", 10, std::bind(&MissionNode::on_scan, this, std::placeholders::_1))}, 
        arrival_subscription_{create_subscription < gp1_interfaces::msg::Arrival> (
            "arrival", 10, std::bind(&MissionNode::on_arrival, this, std::placeholders::_1))},
        report_publisher_{create_publisher < gp1_interfaces::msg::InspectionReport > (
            "inspection_report", 10)} {}
        
        private:

        /**
         * @brief stores newest scan
         * 
         * @param msg the scan recieved. 
         */
        void on_scan(const sensor_msgs::msg::LaserScan& msg);

        /**
         * @brief decides the results of each checkpoint and publish report.
         * 
         * @param msg arrival msg that triggeres inspection 
         */
        void on_arrival(const gp1_interfaces::msg::Arrival& msg);

        //ranges under this values indicates closed in m
        static constexpr float kShortRange_m{1.0F};

        /// @brief recieves scans
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_subscription_;

        /// @brief recieves arrival msgs
        rclcpp::Subscription<gp1_interfaces::msg::Arrival>::SharedPtr arrival_subscription_;

        /// @brief sends reports
        rclcpp::Publisher<gp1_interfaces::msg::InspectionReport>::SharedPtr report_publisher_;

        /// @brief the newest scan
        sensor_msgs::msg::LaserScan latest_scan_{};

    };
} // namespace gp1_patrol