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

#pragma once
#include <vda5050_connector/generic_plugin.hpp>
#include <rclcpp/rclcpp.hpp>
#include <vda5050_msgs/msg/visualization.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>

namespace connector_plugins
{
class VisualizationPlugin : public GenericPlugin<vda5050_msgs::msg::Visualization>
{
    using Visualization = vda5050_msgs::msg::Visualization;
    public:
        VisualizationPlugin() = default;
        ~VisualizationPlugin() = default;
        void init() override;
        void update(Visualization& msg) override;

    private:
        std::vector<SubscriptionFields> generateSubscriptionFields() override;
        void odomCallback(SerializedMsgPtr msg);
        void amclPoseCallback(SerializedMsgPtr msg);
        
        nav_msgs::msg::Odometry last_odometry_;
        geometry_msgs::msg::PoseWithCovarianceStamped last_amcl_pose_;
};
    
} // namespace connector_plugins

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(connector_plugins::VisualizationPlugin, connector_plugins::GenericPlugin<vda5050_msgs::msg::Visualization>)