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

#include <rclcpp/rclcpp.hpp>
#include "vda5050_connector/vda5050_connector.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<VDA5050Connector>("vda5050_connector");
  node->init();
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.spin();

  rclcpp::shutdown();
  return 0;
}