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
// THIRD-PARTY SYSTEM.-e 

#include "vda5050_connector/subscription_manager.hpp"

SubscriptionManager::SubscriptionManager(rclcpp::Node::SharedPtr node)
: node_(node)
{
}

SubscriptionManager::SubscriptionManager()
: node_()
{
}

void SubscriptionManager::init(rclcpp::Node::SharedPtr node)
{
	if (node_.expired())
	{
		node_ = node;
	}
}

void SubscriptionManager::registerSubscription(SubscriptionFields fields)
{
	auto callback = fields.callback;
	std::string topic_name = fields.topic_name;
	std::string topic_type = fields.topic_type;
	auto wrapper = [callback](std::shared_ptr<void> msg) {
		callback(std::static_pointer_cast<rclcpp::SerializedMessage>(msg));
	};

	auto it = topic_callbacks_.find(topic_name);
	if (it != topic_callbacks_.end())
	{
		it->second.push_back(wrapper);
	}
	else
	{
		topic_callbacks_[topic_name] = {wrapper};
		auto subscription = node_.lock()->create_generic_subscription(
			topic_name,
			topic_type,
			rclcpp::QoS(10),
			[this, topic_name](const std::shared_ptr<rclcpp::SerializedMessage> msg)
			{
				for (auto &cb : topic_callbacks_[topic_name])
				{
					cb(msg);
				}
			}
		);

		subscriptions_.push_back(subscription);
	}
}