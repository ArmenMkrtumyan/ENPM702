/**
 * @file stand_in_robot_node.hpp
 * @author Sagar Vijayakumar
 * @brief Declares the StandInRobotNode class for simulated robot operation.
 *
 * This header defines a ROS 2 node that simulates robot movement
 * and sensor feedback for the GP1 patrol system.
 *
 * The node receives velocity commands, updates its simulated position
 * and orientation, publishes odometry, and generates laser scan readings
 * based on proximity to predefined checkpoints.
 */

#pragma once


#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <chrono>
#include <functional>





namespace gp1_patrol
{
/**
 * @class StandInRobotNode
 * @brief Simulates robot movement, odometry, and laser scan measurements.
 *
 * @details
 * This class implements a stand-in mobile robot for testing the GP1
 * patrol system without requiring physical robot hardware.
 *
 * The node subscribes to the "cmd_vel" topic to receive linear
 * and angular velocity commands.
 *
 * A periodic timer updates the robot's position and heading using
 * the commanded velocities and elapsed time. The resulting pose
 * and velocity information are published on "odometry/filtered".
 *
 * A separate timer generates simulated laser scan readings based
 * on the robot's proximity to predefined checkpoints and publishes
 * the readings on the "scan" topic.
 *
 * The class maintains the robot's position, orientation, commanded
 * velocities, previous update timestamp, and most recently detected
 * checkpoint.
 */
class StandInRobotNode:public rclcpp::Node{


    public:
/**
 * @brief Constructs and initializes the stand-in robot node.
 *
 * @details
 * Initializes the ROS 2 node with the name "stand_in_robot_node".
 *
 * Creates a subscriber for velocity commands on "cmd_vel",
 * an odometry publisher on "odometry/filtered", and a laser
 * scan publisher on "scan".
 *
 * Initializes the previous odometry update timestamp using
 * the current ROS 2 time.
 *
 * Creates two periodic wall timers:
 * - Odometry timer: 50 milliseconds.
 * - Laser scan timer: 200 milliseconds.
 *
 * Each timer invokes its corresponding callback to update
 * and publish simulated robot data.
 */
        StandInRobotNode():rclcpp::Node{"stand_in_robot_node"}
            {
            command_velocity_subscriber_ = create_subscription<geometry_msgs::msg::Twist>("cmd_vel", 10, std::bind(&StandInRobotNode::callback_command_velocity, this, std::placeholders::_1));
            odometry_publisher_=create_publisher<nav_msgs::msg::Odometry>("odometry/filtered", 10);
            scan_publisher_=create_publisher<sensor_msgs::msg::LaserScan>("scan", 10);

            last_time_updated_ = (*this).now();
            odometry_timer_ = create_wall_timer(std::chrono::milliseconds{50}, std::bind(&StandInRobotNode::callback_odometry_timer, this));
            scan_timer_ = create_wall_timer(std::chrono::milliseconds{200}, std::bind(&StandInRobotNode::callback_scan_timer, this));
        
            }



    private:
/**
 * @brief Processes incoming robot velocity commands.
 *
 * @details
 * Receives a Twist message containing the commanded linear
 * and angular velocities.
 *
 * Extracts the forward velocity from the linear.x field
 * and the angular velocity from the angular.z field.
 *
 * Stores these values for use during subsequent odometry updates.
 *
 * @param msg Shared pointer to a constant Twist message
 * containing the commanded robot velocities.
 */
        
        void callback_command_velocity(const geometry_msgs::msg::Twist::ConstSharedPtr msg);
/**
 * @brief Subscriber for commanded robot velocities.
 *
 * Receives Twist messages from the "cmd_vel" topic and
 * invokes callback_command_velocity() when a message arrives.
 */
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr command_velocity_subscriber_;
/**
 * @brief Updates and publishes simulated robot odometry.
 *
 * @details
 * Calculates the elapsed time since the previous odometry update.
 *
 * Uses the commanded linear and angular velocities to update
 * the robot's x-position, y-position, and heading.
 *
 * Normalizes the heading angle using wrap_angle().
 *
 * Creates an Odometry message containing the updated pose,
 * orientation, and velocity information.
 *
 * Publishes the message on the "odometry/filtered" topic.
 */
        void callback_odometry_timer();
/**
 * @brief Publisher for simulated robot odometry.
 *
 * Publishes nav_msgs::msg::Odometry messages containing
 * the robot's simulated pose and commanded velocities
 * on the "odometry/filtered" topic.
 */        
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odometry_publisher_;
/**
 * @brief Timer responsible for periodic odometry updates.
 *
 * Invokes callback_odometry_timer() every 50 milliseconds
 * to update the simulated robot pose and publish odometry.
 */
        rclcpp::TimerBase::SharedPtr odometry_timer_;
/**
 * @brief Generates and publishes simulated laser scan readings.
 *
 * @details
 * Creates a LaserScan message containing three simulated
 * range measurements.
 *
 * Initially sets all ranges to the predefined far-distance value.
 *
 * Calculates the distance between the robot and each predefined
 * checkpoint.
 *
 * If the robot is within the configured detection distance
 * of a checkpoint, updates the scan ranges using that
 * checkpoint's simulated range measurement.
 *
 * Logs a message when the robot enters a newly detected
 * checkpoint region.
 *
 * Publishes the resulting scan message on the "scan" topic.
 */
        void callback_scan_timer();
/**
 * @brief Timer responsible for generating laser scan data.
 *
 * Invokes callback_scan_timer() every 200 milliseconds
 * to generate and publish simulated laser range measurements.
 */
        rclcpp::TimerBase::SharedPtr scan_timer_;
/**
 * @brief Publisher for simulated laser scan messages.
 *
 * Publishes sensor_msgs::msg::LaserScan messages containing
 * simulated range measurements on the "scan" topic.
 */
        rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_publisher_;
/**
 * @brief Current robot position along the x-axis.
 *
 * Stored in meters relative to the odometry coordinate frame.
 */
        double x_{0.0};
/**
 * @brief Current robot position along the y-axis.
 *
 * Stored in meters relative to the odometry coordinate frame.
 */
        double y_{0.0};
/**
 * @brief Current robot heading angle.
 *
 * Stored in radians and updated using the commanded
 * angular velocity and elapsed time.
 */
        double yaw_{0.0};
/**
 * @brief Most recently commanded forward velocity.
 *
 * Stored in meters per second and obtained from the
 * linear.x field of the received Twist message.
 */
        double linear_velocity_{0.0};
/**
 * @brief Most recently commanded angular velocity.
 *
 * Stored in radians per second and obtained from the
 * angular.z field of the received Twist message.
 */
        double angular_velocity_{0.0};

/**
 * @brief Timestamp of the previous odometry update.
 *
 * Used to calculate the elapsed time between consecutive
 * odometry timer callbacks.
 */
        rclcpp::Time last_time_updated_;
/**
 * @brief Index of the most recently detected nearby checkpoint.
 *
 * Stores the index of the checkpoint currently within
 * the simulated sensor detection distance.
 *
 * A value of -1 indicates that no checkpoint is nearby.
 */

        int last_near_checkpoint_{-1};


};


}
