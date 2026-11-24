/**
 * @file patrol_log_node.cpp
 * @author Sagar Vijayakumar
 * @brief Implements the PatrolLogNode callbacks for inspection logging.
 *
 * This file implements the inspection report subscriber callback
 * and the periodic patrol log timer callback.
 *
 * Incoming inspection reports are classified by their result
 * and counted as clear, blocked, door open, or door closed.
 * Unknown inspection results generate a warning message.
 *
 * The timer callback introduces an intentional 1.5-second delay
 * before logging the accumulated inspection counts.
 * This delay is used to demonstrate blocking callback behavior
 * in a ROS 2 executor.
 */
#include "gp1_patrol/patrol_log_node.hpp"

#include <thread>
#include <chrono>



namespace gp1_patrol
{




    void PatrolLogNode::callback_inspection_report(const gp1_interfaces::msg::InspectionReport::ConstSharedPtr msg)
    {
        if (msg->result == gp1_interfaces::msg::InspectionReport::RESULT_CLEAR)
        {
            ++clear_count_;
        }


        else if (msg->result == gp1_interfaces::msg::InspectionReport::RESULT_BLOCKED)
        {
            ++blocked_count_;
        }


        else if (msg->result == gp1_interfaces::msg::InspectionReport::RESULT_DOOR_OPEN)
        {
            ++door_open_count_;
        }


        else if (msg->result == gp1_interfaces::msg::InspectionReport::RESULT_DOOR_CLOSED)
        {
            ++door_closed_count_;
        }



        else
        {
            RCLCPP_WARN(get_logger(), "Unknown inspection result");
        }


      
    }

    void PatrolLogNode::callback_print_inspection_log_timer()
    {

        std::this_thread::sleep_for(std::chrono::milliseconds{1500});
        RCLCPP_INFO(get_logger(), "patrol log: clear %zu, blocked %zu, door open %zu, door closed %zu", clear_count_, blocked_count_, door_open_count_, door_closed_count_);

    }



  
} // namespace gp1_patrol
