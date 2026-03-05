// Proprietary Software License
// 
// Copyright (c) 2026, Robotnik Automation S.L.
// All rights reserved.
// 
// Redistribution and use of this software are permitted for internal use by the
// authorized Client, and as part of an integrated hardware and software solution,
// with or without modification, provided that the following conditions are met:
// 
// 1. The software may be used, modified, and extended for internal purposes or as
//    part of an integrated system delivered to third parties.
// 
// 2. Redistribution of the software is permitted in the form it is provided, 
//    as part of an integrated hardware solution, and not as a standalone product 
// 
// 3. The integrated solution incorporating the software may be sold or
//    commercialized, either directly by the authorized Client or through
//    distributors or resellers, provided that no independent rights to the
//    software are granted to third parties.
// 
// 4. The software may not be sublicensed, extracted, or redistributed separately
//    from the integrated solution, in source or binary form.
// 
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED. ANY MODIFICATIONS MADE TO THE SOFTWARE ARE THE SOLE RESPONSIBILITY OF
// THE PARTY WHO MAKES THEM. IN NO EVENT SHALL ROBOTNIK AUTOMATION S.L. BE LIABLE
// FOR ANY DAMAGES ARISING FROM THE USE OF THE SOFTWARE OR ITS INTEGRATION IN A
// THIRD-PARTY SYSTEM.

#include "vda5050_connector/visualization_plugin.hpp"

namespace connector_plugins
{
void VisualizationPlugin::init()
{
}

std::vector<SubscriptionFields> VisualizationPlugin::generateSubscriptionFields()
{
    std::vector<SubscriptionFields> subscriptions;
    subscriptions.reserve(2);

    subscriptions.emplace_back("/robot/robotnik_base_control/odom",
        "nav_msgs/msg/Odometry", std::bind(&VisualizationPlugin::odomCallback, this, std::placeholders::_1));
    subscriptions.emplace_back("/robot/amcl/pose",
        "geometry_msgs/msg/PoseWithCovarianceStamped", std::bind(&VisualizationPlugin::amclPoseCallback, this, std::placeholders::_1));
    return subscriptions;
}

void VisualizationPlugin::odomCallback(SerializedMsgPtr msg)
{
    last_odometry_ = deserializeMessage<nav_msgs::msg::Odometry>(msg);
}

void VisualizationPlugin::amclPoseCallback(SerializedMsgPtr msg)
{
    last_amcl_pose_ = deserializeMessage<geometry_msgs::msg::PoseWithCovarianceStamped>(msg);
}

void VisualizationPlugin::update(Visualization& msg)
{
    msg.agv_position.x = last_amcl_pose_.pose.pose.position.x;
    msg.agv_position.y = last_amcl_pose_.pose.pose.position.y;
    msg.agv_position.theta = std::atan2(last_amcl_pose_.pose.pose.orientation.z, last_amcl_pose_.pose.pose.orientation.w) * 2.0;
    msg.velocity.vx = last_odometry_.twist.twist.linear.x;
    msg.velocity.vy = last_odometry_.twist.twist.linear.y;
    msg.velocity.omega = last_odometry_.twist.twist.angular.z;
}

} // namespace connector_plugins
