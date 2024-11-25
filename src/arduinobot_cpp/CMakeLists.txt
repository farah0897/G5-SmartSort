cmake_minimum_required(VERSION 3.8)
project(arduinobot_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(arduinobot_msgs REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(moveit_core REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)


# Simple Publisher
add_executable(publisher src/publisher.cpp)
ament_target_dependencies(publisher "rclcpp" "std_msgs")

# Simple Subscriber
add_executable(subscriber src/subscriber.cpp)
ament_target_dependencies(subscriber "rclcpp" "std_msgs")

# Simple Parameter
add_executable(parameter src/parameter.cpp)
ament_target_dependencies(parameter "rclcpp" "rcl_interfaces")

# Simple Service Server
add_executable(service_server src/service_server.cpp)
ament_target_dependencies(service_server "rclcpp" "arduinobot_msgs")

# Simple Service Client
add_executable(service_client src/service_client.cpp)
ament_target_dependencies(service_client "rclcpp" "arduinobot_msgs")

# Simple Action Client
add_executable(action_client src/action_client.cpp)
ament_target_dependencies(action_client
  "rclcpp"
  "rclcpp_action"
  "arduinobot_msgs"
)


# Simple MoveIt Interface
add_executable(moveit_interface src/moveit_interface.cpp)
ament_target_dependencies(moveit_interface
  "rclcpp"
  "moveit_ros_planning_interface"
  "moveit_core"
  "tf2_ros"
  "tf2_geometry_msgs"
  "geometry_msgs"
  "arduinobot_msgs"
)

# Simple Action Server Library
add_library(action_server SHARED src/action_server.cpp)
target_include_directories(action_server PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_definitions(action_server
  PRIVATE "ACTION_SERVER_CPP_BUILDING_DLL")
ament_target_dependencies(action_server
  "arduinobot_msgs"
  "rclcpp"
  "rclcpp_action"
  "rclcpp_components")
rclcpp_components_register_node(action_server
  PLUGIN "arduinobot_cpp::SimpleActionServer"
  EXECUTABLE action_server_node)


# Install Targets
install(TARGETS
  publisher
  subscriber
  parameter
  service_server
  service_client
  moveit_interface
  DESTINATION lib/${PROJECT_NAME}
)

target_link_libraries(action_server
  rclcpp::rclcpp
  rclcpp_action::rclcpp_action
)

install(TARGETS 
  action_server 
  action_client
  ARCHIVE DESTINATION lib 
  LIBRARY DESTINATION lib 
  RUNTIME DESTINATION bin)

# Testing
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
