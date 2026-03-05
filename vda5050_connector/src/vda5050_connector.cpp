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

#include "vda5050_connector/vda5050_connector.hpp"

VDA5050Connector::VDA5050Connector(const std::string& node_name)
: rclcpp::Node(node_name)
, order_subscriber_(create_subscription<Order>("order", 10, std::bind(&VDA5050Connector::orderCallback, this, std::placeholders::_1)))
, instant_action_subscriber_(create_subscription<InstantAction>("instant_action", 10, std::bind(&VDA5050Connector::instantActionCallback, this, std::placeholders::_1)))
// , state_publisher_(create_publisher<State>("state", 10))
// , connection_publisher_(create_publisher<Connection>("connection", 10))
// , factsheet_publisher_(create_publisher<Factsheet>("factsheet", 10))
// , visualization_publisher_(create_publisher<Visualization>("visualization", 10))
, state_plugin_loader_("vda5050_connector", "connector_plugins::GenericPlugin")
, visualization_plugin_loader_("vda5050_connector", "connector_plugins::GenericPlugin")
, subscription_manager_()
{
	// readParam<double>("state_publish_rate", state_publish_rate_, 1.0);
	// readParam<double>("connection_publish_rate", connection_publish_rate_, 25.0);
	// readParam<double>("factsheet_publish_rate", factsheet_publish_rate_, 1.0);
	// readParam<double>("visualization_publish_rate", visualization_publish_rate_, 0.5);
	std::vector<std::string> state_plugins, visualization_plugins;
	readParam<std::vector<std::string>>("state_plugins", state_plugins, std::vector<std::string>());
	readParam<std::vector<std::string>>("visualization_plugins", visualization_plugins, std::vector<std::string>());
	
	loadPlugins(state_plugins, state_plugins_, state_plugin_loader_);
	loadPlugins(visualization_plugins, visualization_plugins_, visualization_plugin_loader_);
}

void VDA5050Connector::init()
{
	subscription_manager_.init(shared_from_this());
	for (auto& plugin : state_plugins_)
	{
		plugin->init();
		for (const auto& sub : plugin->getSubscriptionRequest())
		{
			subscription_manager_.registerSubscription(sub);
		}
	}
	for (auto& plugin : visualization_plugins_)
	{
		plugin->init();
		for (const auto& sub : plugin->getSubscriptionRequest())
		{
			subscription_manager_.registerSubscription(sub);
		}
	}

	double state_publish_rate, connection_publish_rate, factsheet_publish_rate, visualization_publish_rate;
	readParam<double>("state_publish_rate", state_publish_rate, 1.0);
	readParam<double>("connection_publish_rate", connection_publish_rate, 25.0);
	readParam<double>("factsheet_publish_rate", factsheet_publish_rate, 1.0);
	readParam<double>("visualization_publish_rate", visualization_publish_rate, 0.5);
	std::string version, manufacturer, serial_number;
	readParam<std::string>("version", version, "1.0");
	readParam<std::string>("manufacturer", manufacturer, "Robotnik");
	readParam<std::string>("serial_number", serial_number, "12345");

	state_publisher_ = std::make_shared<VDA5050Publisher<State>>(
		shared_from_this(), version, manufacturer, serial_number, "state");
	connection_publisher_ = std::make_shared<VDA5050Publisher<Connection>>(
		shared_from_this(), version, manufacturer, serial_number, "connection");
	factsheet_publisher_ = std::make_shared<VDA5050Publisher<Factsheet>>(
		shared_from_this(), version, manufacturer, serial_number, "factsheet");
	visualization_publisher_ = std::make_shared<VDA5050Publisher<Visualization>>(
		shared_from_this(), version, manufacturer, serial_number, "visualization");

	state_pub_timer_ = create_wall_timer(
		std::chrono::milliseconds(static_cast<int>(1000 / state_publish_rate)),
		std::bind(&VDA5050Connector::updateState, this));
	connection_pub_timer_ = create_wall_timer(
		std::chrono::milliseconds(static_cast<int>(1000 / connection_publish_rate)),
		std::bind(&VDA5050Connector::updateConnection, this));
	factsheet_pub_timer_ = create_wall_timer(
		std::chrono::milliseconds(static_cast<int>(1000 / factsheet_publish_rate)),
		std::bind(&VDA5050Connector::updateFactsheet, this));
	visualization_pub_timer_ = create_wall_timer(
		std::chrono::milliseconds(static_cast<int>(1000 / visualization_publish_rate)),
		std::bind(&VDA5050Connector::updateVisualization, this));
}

void VDA5050Connector::updateState()
{
	for (auto& plugin : state_plugins_)
	{
		plugin->update(current_state_);
	}
	state_publisher_->publish(current_state_);
}

void VDA5050Connector::updateConnection()
{
	connection_publisher_->publish(current_connection_);
}

void VDA5050Connector::updateFactsheet()
{
	factsheet_publisher_->publish(current_factsheet_);
}

void VDA5050Connector::updateVisualization()
{
	for (auto& plugin : visualization_plugins_)
	{
		plugin->update(current_visualization_);
	}
	visualization_publisher_->publish(current_visualization_);
}

void VDA5050Connector::orderCallback(Order::SharedPtr msg)
{
	(void)msg; // Avoid unused parameter warning
	std::cout << "Received order message" << std::endl;
	// Handle order message
}

void VDA5050Connector::instantActionCallback(InstantAction::SharedPtr msg)
{
	(void)msg; // Avoid unused parameter warning
	std::cout << "Received instant action message" << std::endl;
	// Handle instant action message
}

void VDA5050Connector::rejectOrder()
{
	std::cout << "Order rejected" << std::endl;
	// Implement order rejection logic
}

void VDA5050Connector::rejectInstantAction()
{
	std::cout << "Instant action rejected" << std::endl;
	// Implement instant action rejection logic
}
