#include "gp1_patrol/patrol_log_node.hpp"
#include <functional>
#include <chrono>
#include <thread>




namespace gp1_patrol
{
  PatrolLogNode::PatrolLogNode():rclcpp::Node("patrol_log")

    {
      inspection_report_subscriber_ = create_subscription<gp1_interfaces::msg::InspectionReport>("inspection_report", 10, std::bind(&PatrolLogNode::callback_inspection_report, this, std::placeholders::_1));
      
      print_inspection_log_timer_= create_wall_timer(std::chrono::seconds(2), std::bind(&PatrolLogNode::callback_print_inspection_log_timer, this));
      
    }



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

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        RCLCPP_INFO(get_logger(), "patrol log: clear %zu, blocked %zu, door open %zu, door closed %zu", clear_count_, blocked_count_, door_open_count_, door_closed_count_);

    }



  
}
