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
#include "boost/date_time/posix_time/posix_time.hpp"
#include <rclcpp/rclcpp.hpp>

template<typename T>
class VDA5050Publisher
{
public:
    VDA5050Publisher(rclcpp::Node::SharedPtr node, const std::string& version, const std::string& manufacturer, const std::string& serial_number, const std::string& topic_name)
    : node_(node)
    , version_(version)
    , manufacturer_(manufacturer)
    , serial_number_(serial_number)
    , header_id_(0)
    , publisher_(node_.lock()->create_publisher<T>(topic_name, rclcpp::QoS(10)))
    {
    }

    ~VDA5050Publisher() = default;

    void publish(T msg)
    {
        // Publish the message using the ROS2 publisher
        msg.header_id = header_id_++;
        msg.timestamp = GetISOCurrentTimestamp();
        msg.version = version_;
        msg.manufacturer = manufacturer_;
        msg.serial_number = serial_number_;
        publisher_->publish(msg);
    }

private:
    std::string GetISOCurrentTimestamp()
    {
        return "";
    }

    rclcpp::Node::WeakPtr node_;
    std::string version_;
    std::string manufacturer_;
    std::string serial_number_;
    std::shared_ptr<rclcpp::Publisher<T>> publisher_;
    int32_t header_id_;
};