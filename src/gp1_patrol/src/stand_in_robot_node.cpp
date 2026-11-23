#include "gp1_patrol/stand_in_robot_node.hpp"
#include <functional>
#include <chrono>
#include <cmath>
#include "gp1_patrol/geometry.hpp"
namespace gp1_patrol
{
  StandInRobotNode::StandInRobotNode():rclcpp::Node("stand_in_robot")
    // Your constructor goes here.
    {
      command_velocity_subscriber_ = create_subscription<geometry_msgs::msg::Twist>("cmd_vel", 10, std::bind(&StandInRobotNode::callback_command_velocity, this, std::placeholders::_1));
      odometry_publisher_=create_publisher<nav_msgs::msg::Odometry>("odometry/filtered", 10);
      scan_publisher_=create_publisher<sensor_msgs::msg::LaserScan>("scan", 10);
          
      last_time_updated_ = (*this).now();
      odometry_timer_ = create_wall_timer(std::chrono::milliseconds(50), std::bind(&StandInRobotNode::callback_odometry_timer, this));
      scan_timer_ = create_wall_timer(std::chrono::milliseconds(200), std::bind(&StandInRobotNode::callback_scan_timer, this));
    }



    void StandInRobotNode::callback_command_velocity(const geometry_msgs::msg::Twist::ConstSharedPtr msg)
    {
      linear_velocity_ = msg->linear.x;
      angular_velocity_ = msg->angular.z;
    }



    void StandInRobotNode::callback_odometry_timer()
    {
        auto time_now = (*this).now();
        double elapsed_time = (time_now - last_time_updated_).seconds();
        last_time_updated_ = time_now;
        x_ = x_+ linear_velocity_ * std::cos(yaw_) * elapsed_time;
        y_ = y_+ linear_velocity_ * std::sin(yaw_) * elapsed_time;
        yaw_ = wrap_angle(yaw_+ angular_velocity_ * elapsed_time);
        
        nav_msgs::msg::Odometry odometry_msg{};
        
        odometry_msg.header.stamp = time_now;
        odometry_msg.header.frame_id = "odom";
        odometry_msg.child_frame_id = "base_link";

        odometry_msg.pose.pose.position.x = x_;
        odometry_msg.pose.pose.position.y = y_;
        odometry_msg.pose.pose.position.z = 0.0;

        odometry_msg.pose.pose.orientation = quaternion_from_yaw(yaw_);
        odometry_msg.twist.twist.linear.x = linear_velocity_;
        odometry_msg.twist.twist.angular.z = angular_velocity_;
        (*odometry_publisher_).publish(odometry_msg);
      }


       void StandInRobotNode::callback_scan_timer()
    {

        sensor_msgs::msg::LaserScan scan_msg{};
        scan_msg.header.stamp = (*this).now();
        scan_msg.angle_min = -0.1F;
        scan_msg.angle_max = 0.1F;
        scan_msg.angle_increment = 0.1F;
        scan_msg.range_min = 0.1F;
        scan_msg.range_max = 10.0F;

        scan_msg.header.frame_id = "base_link";
        scan_msg.ranges = {kFarRangeM, kFarRangeM, kFarRangeM};

        int current_near_checkpoint = -1;

        for (std::size_t i = 0; i < kCheckpoints.size(); ++i)
          {
            const auto& checkpoint = kCheckpoints[i];
            auto distance_bw_robot_and_scan = distance_between(x_, y_, checkpoint.x, checkpoint.y);

            if (distance_bw_robot_and_scan <= kNearDistanceM)
              {
                scan_msg.ranges[0] = checkpoint.scan_range_m;
                scan_msg.ranges[1] = checkpoint.scan_range_m;
                scan_msg.ranges[2] = checkpoint.scan_range_m;
                current_near_checkpoint = static_cast<int>(i);
                break;
              }
          }

        if (current_near_checkpoint != last_near_checkpoint_)
          {
              if (current_near_checkpoint != -1)
              {
                  RCLCPP_INFO(
                      (*this).get_logger(),
                      "stand-in robot near checkpoint %d",
                      current_near_checkpoint + 1
                  );
              }

              last_near_checkpoint_ = current_near_checkpoint;
          }


        (*scan_publisher_).publish(scan_msg);
      }
}





