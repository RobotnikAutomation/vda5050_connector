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
, plugin_loader_("vda5050_connector", "connector_plugins::GenericPlugin")
, state_publisher_(create_publisher<vda5050_msgs::msg::State>("state", 10))
{
}

void VDA5050Connector::init()
{
	subscription_manager_.init(shared_from_this());
	plugin_ = plugin_loader_.createSharedInstance("vda5050_connector_plugins/TestPlugin");
	plugin_->init();
	std::vector<SubscriptionFields> subscription_requests = plugin_->getSubscriptionRequest();
	for (auto& request : subscription_requests)
	{		
		subscription_manager_.registerSubscription(request);
	}
	state_pub_timer_ = create_wall_timer(std::chrono::milliseconds(1000),
		std::bind(&VDA5050Connector::updateState, this));
}

void VDA5050Connector::updateState()
{
	plugin_->update(current_state_);
	state_publisher_->publish(current_state_);
}