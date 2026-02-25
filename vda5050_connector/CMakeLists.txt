cmake_minimum_required(VERSION 3.8)
project(vda5050_connector)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(vda5050_msgs REQUIRED)
find_package(pluginlib REQUIRED)

include_directories(include)

add_executable(vda5050_connector_node
  src/vda5050_connector_node.cpp
  src/vda5050_connector.cpp
  src/subscription_manager.cpp
)

ament_target_dependencies(vda5050_connector_node
  rclcpp
  std_msgs
  pluginlib
  vda5050_msgs
)

ament_export_include_directories(
  include
)


if(BUILD_TESTING)

endif()

install(TARGETS
  vda5050_connector_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  config
  launch
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
)

ament_package()
