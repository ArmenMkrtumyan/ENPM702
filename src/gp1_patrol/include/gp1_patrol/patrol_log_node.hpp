/**
 * @file patrol_log_node.hpp
 * @author Sagar Vijayakumar
 * @brief PatrolLogNode: counts the inspection reports and writes the patrol log (R3)
 */
#pragma once
#include <chrono>
#include <cstddef>
#include <functional>
#include <rclcpp/rclcpp.hpp>

#include "gp1_interfaces/msg/inspection_report.hpp"

namespace gp1_patrol
{
/**
 * @brief Counts the inspection reports per result. Every 2 s a timer sleeps
 * 1.5 s in place of real work, then logs the four counts. The subscription and
 * the timer share the counts, so both stay in the node's default callback group.
 */
 class PatrolLogNode: public rclcpp::Node{


    public:
      /**
       * @brief Construct a new PatrolLogNode: the inspection_report
       * subscription and the 2 s log timer
       */
      PatrolLogNode():rclcpp::Node{"patrol_log_node"}

      {
        inspection_report_subscriber_ = create_subscription<gp1_interfaces::msg::InspectionReport>("inspection_report", 10, std::bind(&PatrolLogNode::callback_inspection_report, this, std::placeholders::_1));

        print_inspection_log_timer_= create_wall_timer(std::chrono::seconds{2}, std::bind(&PatrolLogNode::callback_print_inspection_log_timer, this));

      }



    private:
      /**
       * @brief Adds one to the count of the report's result; warns on an
       * unknown result
       *
       * @param msg The inspection report
       */
      void callback_inspection_report(const gp1_interfaces::msg::InspectionReport::ConstSharedPtr msg);
      /// @brief The mission node's reports, from inspection_report
      rclcpp::Subscription<gp1_interfaces::msg::InspectionReport>::SharedPtr inspection_report_subscriber_;

      /**
       * @brief Runs every 2 s: sleeps 1.5 s, then logs
       * "patrol log: clear N, blocked N, door open N, door closed N"
       */
      void callback_print_inspection_log_timer();
      /// @brief Calls callback_print_inspection_log_timer every 2 s
      rclcpp::TimerBase::SharedPtr print_inspection_log_timer_;

      std::size_t clear_count_{0}; ///< number of RESULT_CLEAR reports
      std::size_t blocked_count_{0}; ///< number of RESULT_BLOCKED reports
      std::size_t door_open_count_{0}; ///< number of RESULT_DOOR_OPEN reports
      std::size_t door_closed_count_{0}; ///< number of RESULT_DOOR_CLOSED reports


 };
}// namespace gp1_patrol
