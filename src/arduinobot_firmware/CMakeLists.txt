cmake_minimum_required(VERSION 3.8)
project(arduinobot_firmware)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(SERIAL libserial)

# Finn LibSerial
find_path(LIBSERIAL_INCLUDE_DIR libserial/SerialPort.h
    PATHS /usr/include /usr/local/include
)
find_library(LIBSERIAL_LIBRARY
    NAMES serial
    PATHS /usr/lib /usr/local/lib
)

if(NOT LIBSERIAL_INCLUDE_DIR OR NOT LIBSERIAL_LIBRARY)
    message(FATAL_ERROR "LibSerial not found!")
endif()

add_executable(serial_transmitter src/serial_transmitter.cpp)
ament_target_dependencies(serial_transmitter
    rclcpp
    std_msgs
)

target_include_directories(serial_transmitter PUBLIC
    ${LIBSERIAL_INCLUDE_DIR}
)

target_link_libraries(serial_transmitter
    ${LIBSERIAL_LIBRARY}
)

add_executable(serial_receiver src/serial_receiver.cpp)
ament_target_dependencies(serial_receiver
    rclcpp
    std_msgs
)

target_include_directories(serial_receiver PUBLIC
    ${LIBSERIAL_INCLUDE_DIR}
)

target_link_libraries(serial_receiver
    ${LIBSERIAL_LIBRARY}
)

install(TARGETS
    serial_transmitter
    serial_receiver
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    DESTINATION lib/${PROJECT_NAME}
)


if(BUILD_TESTING)
    find_package(ament_lint_auto REQUIRED)
    set(ament_cmake_copyright_FOUND TRUE)
    set(ament_cmake_cpplint_FOUND TRUE)
    ament_lint_auto_find_test_dependencies()
endif()

ament_package()
