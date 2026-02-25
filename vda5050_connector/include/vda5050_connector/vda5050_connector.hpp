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
#include <rclcpp/rclcpp.hpp>
#include "vda5050_connector/subscription_manager.hpp"
#include <pluginlib/class_loader.hpp>
#include "vda5050_connector/generic_plugin.hpp"
#include <vda5050_msgs/msg/state.hpp>
// #include <vda5050_msgs/msg/edge_state.hpp>
// #include <vda5050_msgs/msg/agv_position.hpp>
// #include <vda5050_msgs/msg/velocity.hpp>
// #include <vda5050_msgs/msg/load.hpp>
// #include <vda5050_msgs/msg/action_state.hpp>
// #include <vda5050_msgs/msg/battery_state.hpp>
// #include <vda5050_msgs/msg/error.hpp>
// #include <vda5050_msgs/msg/information.hpp>
// #include <vda5050_msgs/msg/safety_state.hpp>
// #include <vda5050_msgs/msg/interaction_zone_state.hpp>

class VDA5050Connector : public rclcpp::Node
{
public:
	VDA5050Connector() = delete;
	~VDA5050Connector() = default;
	explicit VDA5050Connector(const std::string& node_name);
	void init();
	void updateState();

private:
	pluginlib::ClassLoader<connector_plugins::GenericPlugin> plugin_loader_;
	std::shared_ptr<connector_plugins::GenericPlugin> plugin_;
	SubscriptionManager subscription_manager_;
	rclcpp::Publisher<vda5050_msgs::msg::State>::SharedPtr state_publisher_;
	rclcpp::TimerBase::SharedPtr state_pub_timer_;
	vda5050_msgs::msg::State current_state_;
};