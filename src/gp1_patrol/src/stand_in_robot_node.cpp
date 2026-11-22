#include "gp1_patrol/stand_in_robot_node.hpp"

#include <chrono>
#include <cmath>
#include <cstddef>

using namespace std::chrono_literals;

namespace gp1_patrol
{


void StandInRobotNode::on_command_velocity(const geometry_msgs::msg::Twist::SharedPtr msg)
{
  linear_x_ = msg->linear.x;
  angular_z_ = msg->angular.z;
}
void StandInRobotNode::odometry_timer()
{

  }
  prev_odom_time_ = now;
  has_prev_odom_time_ = true;

  nav_msgs::msg::Odometry odom_msg;
  odom_msg.header.stamp = now;
  odom_msg.header.frame_id = "odom";
  odom_msg.child_frame_id = "base_link";
  odom_msg.pose.pose.position.x = x_;
  odom_msg.pose.pose.position.y = y_;
  odom_msg.pose.pose.orientation.z = std::sin(yaw_ / 2.0);
  odom_msg.pose.pose.orientation.w = std::cos(yaw_ / 2.0);
  odom_publish_->publish(odom_msg);

}