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
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/generic_subscription.hpp>
#include <rclcpp/serialized_message.hpp>

struct SubscriptionFields
{
	std::string topic_name;
	std::string topic_type;
	std::function<void(std::shared_ptr<rclcpp::SerializedMessage>)> callback;

	SubscriptionFields(const std::string& name, const std::string& type, std::function<void(std::shared_ptr<rclcpp::SerializedMessage>)> cb)
	: topic_name(name), topic_type(type), callback(cb) {}

	SubscriptionFields() = default;
};

class SubscriptionManager
{
public:
	SubscriptionManager(rclcpp::Node::SharedPtr node);
	SubscriptionManager();
	~SubscriptionManager() = default;
	void init(rclcpp::Node::SharedPtr node);

	void registerSubscription(SubscriptionFields fields);

private:
	rclcpp::Node::WeakPtr node_;
	std::vector<rclcpp::GenericSubscription::SharedPtr> subscriptions_;
	std::unordered_map<std::string, std::vector<std::function<void(std::shared_ptr<void>)>>> topic_callbacks_;
};
