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

#include "vda5050_connector/test_plugin.hpp"

namespace connector_plugins
{
void TestPlugin::update(vda5050_msgs::msg::State& state)
{
    std::vector<vda5050_msgs::msg::Error> errors;
    vda5050_msgs::msg::Error error, error2;
    error.error_type = "Malo";
    error2.error_type = "Malisimo";
    errors.push_back(error);
    errors.push_back(error2);
    state.errors = errors;
}

void TestPlugin::init()
{
}

std::vector<SubscriptionFields> TestPlugin::generateSubscriptionFields()
{
    // Example subscription request
    std::vector<SubscriptionFields> subscriptions;

    subscriptions.push_back(SubscriptionFields("test_topic", "std_msgs/msg/String",
        std::bind(&TestPlugin::testSubscriptionCb, this, std::placeholders::_1)));
    subscriptions.push_back(SubscriptionFields("test_topic", "std_msgs/msg/String",
        std::bind(&TestPlugin::testSubscriptionCb2, this, std::placeholders::_1)));
    return subscriptions;
}

void TestPlugin::testSubscriptionCb(std::shared_ptr<rclcpp::SerializedMessage> msg)
{
    auto typed_msg = std::make_shared<std_msgs::msg::String>();
	rclcpp::SerializedMessage ser_msg(*msg);
	rclcpp::Serialization<std_msgs::msg::String> ser;
	ser.deserialize_message(&ser_msg, typed_msg.get());
    std::cout << "Received message: " << typed_msg->data << std::endl;
}

void TestPlugin::testSubscriptionCb2(std::shared_ptr<rclcpp::SerializedMessage> msg)
{
	auto typed_msg = std::make_shared<std_msgs::msg::String>();
	rclcpp::SerializedMessage ser_msg(*msg);
	rclcpp::Serialization<std_msgs::msg::String> ser;
	ser.deserialize_message(&ser_msg, typed_msg.get());
    std::cout << "Received message in callback 2: " << typed_msg->data << std::endl;
}

} // namespace connector_plugins

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(connector_plugins::TestPlugin, connector_plugins::GenericPlugin)