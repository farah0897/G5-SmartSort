cmake_minimum_required(VERSION 3.8)
project(arduinobot_cpp_examples)

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
add_executable(simple_publisher src/simple_publisher.cpp)
ament_target_dependencies(simple_publisher "rclcpp" "std_msgs")

# Simple Subscriber
add_executable(simple_subscriber src/simple_subscriber.cpp)
ament_target_dependencies(simple_subscriber "rclcpp" "std_msgs")

# Simple Parameter
add_executable(simple_parameter src/simple_parameter.cpp)
ament_target_dependencies(simple_parameter "rclcpp" "rcl_interfaces")

# Simple Service Server
add_executable(simple_service_server src/simple_service_server.cpp)
ament_target_dependencies(simple_service_server "rclcpp" "arduinobot_msgs")

# Simple Service Client
add_executable(simple_service_client src/simple_service_client.cpp)
ament_target_dependencies(simple_service_client "rclcpp" "arduinobot_msgs")

# Simple Action Client
add_executable(simple_action_client src/simple_action_client.cpp)
ament_target_dependencies(simple_action_client
  "rclcpp"
  "rclcpp_action"
  "arduinobot_msgs"
)

# Simple MoveIt Interface
add_executable(simple_moveit_interface src/simple_moveit_interface.cpp)
ament_target_dependencies(simple_moveit_interface
  "rclcpp"
  "moveit_ros_planning_interface"
  "moveit_core"
  "tf2_ros"
  "tf2_geometry_msgs"
  "geometry_msgs"
)

# Simple Action Server Library
add_library(simple_action_server SHARED src/simple_action_server.cpp)
target_include_directories(simple_action_server PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_definitions(simple_action_server
  PRIVATE "SIMPLE_ACTION_SERVER_CPP_BUILDING_DLL")
ament_target_dependencies(simple_action_server
  "arduinobot_msgs"
  "rclcpp"
  "rclcpp_action"
  "rclcpp_components")
rclcpp_components_register_node(simple_action_server
  PLUGIN "arduinobot_cpp_examples::SimpleActionServer"
  EXECUTABLE simple_action_server_node)


# Install Targets
install(TARGETS
  simple_publisher
  simple_subscriber
  simple_parameter
  simple_service_server
  simple_service_client
  simple_moveit_interface
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS 
  simple_action_server 
  simple_action_client
  ARCHIVE DESTINATION lib 
  LIBRARY DESTINATION lib 
  RUNTIME DESTINATION bin)

# Testing
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
