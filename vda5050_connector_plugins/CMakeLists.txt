cmake_minimum_required(VERSION 3.8)
project(vda5050_connector_plugins)

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
find_package(vda5050_connector REQUIRED)

# include_directories(include ${vda5050_connector_INCLUDE_DIRS})

# Find all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Link dependencies and export plugins
ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  pluginlib
  vda5050_connector
  vda5050_msgs
  std_msgs
)

target_include_directories(vda5050_connector_plugins PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${vda5050_connector_INCLUDE_DIRS}
)

pluginlib_export_plugin_description_file(vda5050_connector vda5050_connector_pluginlib.xml)

# Install targets and other files
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(DIRECTORY
  config
  launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_dependencies(
  ${THIS_PACKAGE_INCLUDE_DEPENDS}
)
ament_export_libraries(
  ${PROJECT_NAME}
)

ament_package()