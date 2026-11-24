/**
 * @file operator_node.cpp
 * @author mbabalol
 * @brief Methods for OperatorNodes. 
 * 
 */

#include "gp1_patrol/operator_node.hpp"

namespace gp1_patrol {

    using gp1_interfaces::msg::StopCommand;

    void OperatorNode::on_stop_timer(){
        stop_timer_-> cancel();
        send_command(StopCommand::STOP, "STOP", "operator stop");
        resume_timer_ = create_wall_timer(kResumeDelay, std::bind(&OperatorNode::on_resume_timer, this));

    }

    void OperatorNode::on_resume_timer(){
        resume_timer_ -> cancel();
        send_command(StopCommand::RESUME, "RESUME", "operator resume");
    }

    void OperatorNode::send_command(std::uint8_t command, const std::string& label, const std::string& reason){
        StopCommand msg{};
        msg.header.stamp = now();
        msg.command = command;
        msg.reason = reason;
        command_publisher_ -> publish(msg);
        RCLCPP_INFO_STREAM(get_logger(), "sent " << label << " (" << reason << ")");
    }

} // namespace gp1_patrol