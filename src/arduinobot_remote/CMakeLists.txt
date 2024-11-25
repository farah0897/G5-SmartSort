cmake_minimum_required(VERSION 3.8)
project(arduinobot_remote)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(moveit_core REQUIRED)
find_package(arduinobot_msgs REQUIRED)
find_package(rclpy REQUIRED)

# Task Server Library
add_library(task_server SHARED
  src/task_server.cpp
)

target_include_directories(task_server PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(task_server
  rclcpp
  rclcpp_action
  arduinobot_msgs
  rclcpp_components
  moveit_ros_planning_interface
  moveit_core
)

rclcpp_components_register_nodes(task_server
  PLUGIN "arduinobot_remote::TaskServer"
  EXECUTABLE task_server_node
)

# Install Python modules
ament_python_install_package(${PROJECT_NAME})

# Install launch files
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

# Install C++ targets
install(TARGETS task_server
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
