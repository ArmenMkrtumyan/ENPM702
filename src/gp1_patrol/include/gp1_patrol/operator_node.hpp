/**
 * @file operator_node.hpp
 * @author mbabalol
 * @brief Operator Node
 * 
 */

 #pragma once

 #include <string>
 #include <functional>
 #include <chrono>

 #include <gp1_interfaces/msg/stop_command.hpp>
 #include <rclcpp/rclcpp.hpp>

 namespace gp1_patrol{

    class OperatorNode : public rclcpp::Node {
        public:

        /**
         * @brief Construct a new Operator Node object.
         * after certain time after start publish stop
         * after ceratin time after stop publish resume
         * 
         * topics: publishes: /emergency_stop (gp1_interfaces/msg/StopCommand)
         * 
         */
        OperatorNode() : rclcpp::Node{"operator_node"}, command_publisher_{
            create_publisher<gp1_interfaces::msg::StopCommand>("/emergency_stop", 10)},
        stop_timer_{create_wall_timer(kStopDelay, std::bind(&OperatorNode::on_stop_timer, this))} {}

        private:

        /**
         * @brief send stop and begins the resume timer after kStopDelay on start
         * 
         */
        void on_stop_timer();

        /**
         * @brief sends resume after kResumeDelay after stop command was sent
         * 
         */
        void on_resume_timer();

        /// @brief a publisher for command including time
        /// @param command stop/resume command
        /// @param label command line to be sent
        /// @param reason "reason" for command
        void send_command(std::uint8_t command, const std::string& label, const std::string& reason);

        /// @brief timer from node start to stop
        static constexpr std::chrono::seconds kStopDelay{13};

        /// @brief time from stop to resume
        static constexpr std::chrono::seconds kResumeDelay{5};

        /// @brief /emergency stop command
        rclcpp::Publisher<gp1_interfaces::msg::StopCommand>::SharedPtr command_publisher_;

        /// @brief one-shot timer for the stop
        rclcpp::TimerBase::SharedPtr stop_timer_;

        /// @brief one-shot timer for the resume command
        rclcpp::TimerBase::SharedPtr resume_timer_{};
    };

 } //namespace gp1_patrol