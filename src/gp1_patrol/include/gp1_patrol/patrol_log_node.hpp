#pragma once
#include <chrono>
#include <cstddef>
#include <functional>
#include <rclcpp/rclcpp.hpp>

#include "gp1_interfaces/msg/inspection_report.hpp"

namespace gp1_patrol
{
 class PatrolLogNode: public rclcpp::Node{
    
  
    public:
      
      PatrolLogNode():rclcpp::Node{"patrol_log_node"}

      {
        inspection_report_subscriber_ = create_subscription<gp1_interfaces::msg::InspectionReport>("inspection_report", 10, std::bind(&PatrolLogNode::callback_inspection_report, this, std::placeholders::_1));
        
        print_inspection_log_timer_= create_wall_timer(std::chrono::seconds{2}, std::bind(&PatrolLogNode::callback_print_inspection_log_timer, this));
        
      }

    
    
    private:
      void callback_inspection_report(const gp1_interfaces::msg::InspectionReport::ConstSharedPtr msg);
      rclcpp::Subscription<gp1_interfaces::msg::InspectionReport>::SharedPtr inspection_report_subscriber_;
      
      void callback_print_inspection_log_timer();
      rclcpp::TimerBase::SharedPtr print_inspection_log_timer_;
      
      std::size_t clear_count_{0};
      std::size_t blocked_count_{0};
      std::size_t door_open_count_{0};
      std::size_t door_closed_count_{0};


 };
}