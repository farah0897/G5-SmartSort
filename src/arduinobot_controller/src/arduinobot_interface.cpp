#include "arduinobot_controller/arduinobot_interface.hpp"
#include <libserial/SerialPort.h>
#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <string>
#include <vector>
#include <cmath>

namespace arduinobot_controller
{

CallbackReturn ArduinobotInterface::on_init(const hardware_interface::HardwareInfo & hardware_info)
{
    if (hardware_interface::SystemInterface::on_init(hardware_info) != CallbackReturn::SUCCESS)
    {
        return CallbackReturn::ERROR;
    }

    // Initialize vectors based on the number of joints
    position_states_.resize(info_.joints.size(), 0.0);
    position_commands_.resize(info_.joints.size(), 0.0);
    previous_position_commands_.resize(info_.joints.size(), 0.0);
    // Get port name from hardware parameters
    for (const auto & component : info_.hardware_parameters)
    {   
        if (component.first == "serial_port")
        {
            arduinobot_port_name_ = component.second;
            RCLCPP_INFO(rclcpp::get_logger("ArduinobotInterface"), 
                "Serial port name: %s", arduinobot_port_name_.c_str());
            break;
        }
    }

    return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface> ArduinobotInterface::export_state_interfaces()
{
    std::vector<hardware_interface::StateInterface> state_interfaces;
    
    // Export position state interface for each joint
    for (size_t i = 0; i < info_.joints.size(); i++)
    {
        state_interfaces.emplace_back(
            hardware_interface::StateInterface(
                info_.joints[i].name, 
                hardware_interface::HW_IF_POSITION, 
                &position_states_[i]));
    }

    return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> ArduinobotInterface::export_command_interfaces()
{
    std::vector<hardware_interface::CommandInterface> command_interfaces;
    
    // Export position command interface for each joint
    for (size_t i = 0; i < info_.joints.size(); i++)
    {
        command_interfaces.emplace_back(
            hardware_interface::CommandInterface(
                info_.joints[i].name, 
                hardware_interface::HW_IF_POSITION, 
                &position_commands_[i]));
    }

    return command_interfaces;
}

CallbackReturn ArduinobotInterface::on_activate(const rclcpp_lifecycle::State & /*previous_state*/)
{
    // Try to open serial port
    try
    {
        arduinobot_serial_port_.Open(arduinobot_port_name_);
        arduinobot_serial_port_.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
    }
    catch (const LibSerial::OpenFailed&)
    {
        RCLCPP_ERROR(rclcpp::get_logger("ArduinobotInterface"), 
            "Failed to open serial port %s", arduinobot_port_name_.c_str());
        return CallbackReturn::ERROR;
    }

    // Initialize positions
    for (size_t i = 0; i < position_states_.size(); i++)
    {
        position_states_[i] = 0.0;
        position_commands_[i] = 0.0;
        previous_position_commands_[i] = 0.0;
    }

    RCLCPP_INFO(rclcpp::get_logger("ArduinobotInterface"), "Successfully activated!");
    return CallbackReturn::SUCCESS;
}

CallbackReturn ArduinobotInterface::on_deactivate(const rclcpp_lifecycle::State & /*previous_state*/)
{
    try
    {
        arduinobot_serial_port_.Close();
    }
    catch (const std::exception& e)
    {
        RCLCPP_ERROR(rclcpp::get_logger("ArduinobotInterface"), 
            "Error closing serial port: %s", e.what());
        return CallbackReturn::ERROR;
    }

    RCLCPP_INFO(rclcpp::get_logger("ArduinobotInterface"), "Successfully deactivated!");
    return CallbackReturn::SUCCESS;
}

hardware_interface::return_type ArduinobotInterface::read(
    const rclcpp::Time & /*time*/, 
    const rclcpp::Duration & /*period*/)
{
    if (!arduinobot_serial_port_.IsOpen())
    {
        RCLCPP_ERROR(rclcpp::get_logger("ArduinobotInterface"), "Serial port is not open");
        return hardware_interface::return_type::ERROR;
    }

    try
    {
        std::string response;
        arduinobot_serial_port_.Write("r\n");  // Request current positions
        arduinobot_serial_port_.ReadLine(response);

        // Parse response and update position states
        // Expected format: "p1,p2,p3,p4,p5"
        size_t pos = 0;
        std::string delimiter = ",";
        size_t joint_idx = 0;

        while ((pos = response.find(delimiter)) != std::string::npos && joint_idx < position_states_.size())
        {
            position_states_[joint_idx] = std::stod(response.substr(0, pos));
            response.erase(0, pos + delimiter.length());
            joint_idx++;
        }
        if (joint_idx < position_states_.size())
        {
            position_states_[joint_idx] = std::stod(response);  // Last value
        }
    }
    catch (const std::exception& e)
    {
        RCLCPP_ERROR(rclcpp::get_logger("ArduinobotInterface"), 
            "Error reading from serial port: %s", e.what());
        return hardware_interface::return_type::ERROR;
    }

    return hardware_interface::return_type::OK;
}

hardware_interface::return_type ArduinobotInterface::write(
    const rclcpp::Time & /*time*/, 
    const rclcpp::Duration & /*period*/)
{
    if (!arduinobot_serial_port_.IsOpen())
    {
        RCLCPP_ERROR(rclcpp::get_logger("ArduinobotInterface"), "Serial port is not open");
        return hardware_interface::return_type::ERROR;
    }

    try
    {
        // Only send commands if they have changed
        bool commands_changed = false;
        for (size_t i = 0; i < position_commands_.size(); i++)
        {
            if (std::abs(position_commands_[i] - previous_position_commands_[i]) > 0.01)
            {
                commands_changed = true;
                break;
            }
        }

        if (commands_changed)
        {
            // Format command string
            std::string command = "w";
            for (size_t i = 0; i < position_commands_.size(); i++)
            {
                command += std::to_string(position_commands_[i]);
                if (i < position_commands_.size() - 1)
                {
                    command += ",";
                }
                previous_position_commands_[i] = position_commands_[i];
            }
            command += "\n";

            // Send command to Arduino
            arduinobot_serial_port_.Write(command);
        }
    }
    catch (const std::exception& e)
    {
        RCLCPP_ERROR(rclcpp::get_logger("ArduinobotInterface"), 
            "Error writing to serial port: %s", e.what());
        return hardware_interface::return_type::ERROR;
    }

    return hardware_interface::return_type::OK;
}

} // namespace arduinobot_controller 