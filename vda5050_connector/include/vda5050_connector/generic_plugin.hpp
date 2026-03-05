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
#include <vda5050_connector/subscription_manager.hpp>
#include <rclcpp/generic_subscription.hpp>
#include <rclcpp/serialized_message.hpp>

namespace connector_plugins
{
template<typename T>
class GenericPlugin
{
public:
    using SerializedMsg = rclcpp::SerializedMessage;
    using SerializedMsgPtr = std::shared_ptr<rclcpp::SerializedMessage>;
    virtual void init() = 0;
    
    virtual void update(T& msg) = 0;

    std::vector<SubscriptionFields> getSubscriptionRequest()
    {
        std::vector<SubscriptionFields> subscriptions = generateSubscriptionFields();
        return subscriptions;
    }

protected:
    virtual std::vector<SubscriptionFields> generateSubscriptionFields()
    {
        return std::vector<SubscriptionFields>();
    }

    template<typename MsgType>
    MsgType deserializeMessage(const SerializedMsgPtr& msg)
    {
        auto typed_msg = MsgType();
        SerializedMsg ser_msg(*msg);
        rclcpp::Serialization<MsgType> ser;
        ser.deserialize_message(&ser_msg, &typed_msg);
        return typed_msg;
    }
};
    
} // namespace connector_plugins
