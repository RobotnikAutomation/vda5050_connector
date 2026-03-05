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
#include "vda5050_connector/vda5050_publisher.hpp"
#include <pluginlib/class_loader.hpp>
#include "vda5050_connector/generic_plugin.hpp"
#include <vda5050_msgs/msg/state.hpp>
#include <vda5050_msgs/msg/connection.hpp>
#include <vda5050_msgs/msg/factsheet.hpp>
#include <vda5050_msgs/msg/visualization.hpp>
#include <vda5050_msgs/msg/order.hpp>
#include <vda5050_msgs/msg/valid_order.hpp>
#include <vda5050_msgs/msg/instant_action.hpp>
#include <vda5050_msgs/msg/valid_instant_action.hpp>
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
template<typename T>
using Plugin = connector_plugins::GenericPlugin<T>;
using State = vda5050_msgs::msg::State;
using Visualization = vda5050_msgs::msg::Visualization;
using Connection = vda5050_msgs::msg::Connection;
using Factsheet = vda5050_msgs::msg::Factsheet;
using Order = vda5050_msgs::msg::Order;
using InstantAction = vda5050_msgs::msg::InstantAction;

public:
	VDA5050Connector() = delete;
	~VDA5050Connector() = default;
	explicit VDA5050Connector(const std::string& node_name);
	void init();

	template<typename T>
	bool readParam(const std::string& param_name, T& param_value, const T& default_value)
	{
		declare_parameter<T>(param_name, default_value);
		if (!has_parameter(param_name))
		{
			param_value = default_value;
			return false;
		}
		
		get_parameter(param_name, param_value);
		return true;
	}

private:
	void updateState();
	void updateConnection();
	void updateFactsheet();
	void updateVisualization();
	void orderCallback(Order::SharedPtr msg);
	void instantActionCallback(InstantAction::SharedPtr msg);

	void rejectOrder();
	void rejectInstantAction();

	template<typename T>
	void loadPlugins(std::vector<std::string>& plugin_names,
		std::vector<std::shared_ptr<Plugin<T>>>& plugin_instances,
		pluginlib::ClassLoader<Plugin<T>>& loader)
	{
		for (const auto& plugin_name : plugin_names)
		{
			try
			{
				std::shared_ptr<Plugin<T>> plugin = loader.createSharedInstance(plugin_name);
				plugin_instances.push_back(plugin);
			}
			catch (const pluginlib::LibraryLoadException& ex)
			{
				RCLCPP_ERROR(get_logger(), "Failed to load plugin '%s': %s", plugin_name.c_str(), ex.what());
			}
		}
	}

	// VDA Topic Subscribers
	rclcpp::Subscription<Order>::SharedPtr order_subscriber_;
	rclcpp::Subscription<InstantAction>::SharedPtr instant_action_subscriber_;
	
	// VDA Topic Publishers
	// rclcpp::Publisher<State>::SharedPtr state_publisher_;
	// rclcpp::Publisher<Connection>::SharedPtr connection_publisher_;
	// rclcpp::Publisher<Factsheet>::SharedPtr factsheet_publisher_;
	// rclcpp::Publisher<Visualization>::SharedPtr visualization_publisher_;
	std::shared_ptr<VDA5050Publisher<State>> state_publisher_;
	std::shared_ptr<VDA5050Publisher<Connection>> connection_publisher_;
	std::shared_ptr<VDA5050Publisher<Factsheet>> factsheet_publisher_;
	std::shared_ptr<VDA5050Publisher<Visualization>> visualization_publisher_;

	// Plugin management
	pluginlib::ClassLoader<Plugin<State>> state_plugin_loader_;
	pluginlib::ClassLoader<Plugin<Visualization>> visualization_plugin_loader_;
	
	// Subscription management for plugin topics
	SubscriptionManager subscription_manager_;

	// ROS messages to be published
	State current_state_;
	Connection current_connection_;
	Factsheet current_factsheet_;
	Visualization current_visualization_;

	// ROS Parameters
	// double state_publish_rate_;
	// double connection_publish_rate_;
	// double factsheet_publish_rate_;
	// double visualization_publish_rate_;

	// Plugin instance
	std::vector<std::shared_ptr<Plugin<State>>> state_plugins_;
	std::vector<std::shared_ptr<Plugin<Visualization>>> visualization_plugins_;

	// Timers for periodic publishing
	rclcpp::TimerBase::SharedPtr state_pub_timer_;
	rclcpp::TimerBase::SharedPtr connection_pub_timer_;
	rclcpp::TimerBase::SharedPtr factsheet_pub_timer_;
	rclcpp::TimerBase::SharedPtr visualization_pub_timer_;
};