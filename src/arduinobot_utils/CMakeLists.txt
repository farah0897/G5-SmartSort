cmake_minimum_required(VERSION 3.8)
project(arduinobot_utils)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(rclcpp REQUIRED)
find_package(arduinobot_msgs REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(angle_conversion src/angle_conversion.cpp)
ament_target_dependencies(angle_conversion rclcpp arduinobot_msgs tf2 tf2_ros)

add_executable(lifecycle_node src/lifecycle_node.cpp)
ament_target_dependencies(lifecycle_node 
    rclcpp 
    rclcpp_lifecycle 
    lifecycle_msgs
    std_msgs
)

target_link_libraries(lifecycle_node
  rclcpp::rclcpp
  rclcpp_lifecycle::rclcpp_lifecycle
)


install(TARGETS 
    angle_conversion
    lifecycle_node
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  
  set(ament_cmake_copyright_FOUND TRUE)
  
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
