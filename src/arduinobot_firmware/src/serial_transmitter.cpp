#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <functional>
#include <memory>
#include <libserial/SerialPort.h>
#include <filesystem>

using std::placeholders::_1;

class SerialTransmitter : public rclcpp::Node
{
public:
    SerialTransmitter() : Node("serial_transmitter")
    {
        // Deklarer og få port-parameter
        declare_parameter<std::string>("port", "/dev/ttyACM0");
        auto port = get_parameter("port").as_string();
        
        // Sjekk om porten eksisterer
        if (!std::filesystem::exists(port)) {
            RCLCPP_ERROR(get_logger(), "Port %s does not exist!", port.c_str());
            RCLCPP_INFO(get_logger(), "Available ports:");
            for (const auto & entry : std::filesystem::directory_iterator("/dev/")) {
                std::string path = entry.path().string();
                if (path.find("tty") != std::string::npos) {
                    RCLCPP_INFO(get_logger(), "  %s", path.c_str());
                }
            }
            rclcpp::shutdown();
            return;
        }

        subscription_ = create_subscription<std_msgs::msg::String>(
            "serial_transmitter", 
            10, 
            std::bind(&SerialTransmitter::messageCallback, this, _1));
        
        try {
            RCLCPP_INFO(get_logger(), "Attempting to open %s", port.c_str());
            arduino_.Open(port);
            arduino_.SetBaudRate(LibSerial::BaudRate::BAUD_115200);
            arduino_.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
            arduino_.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
            arduino_.SetParity(LibSerial::Parity::PARITY_NONE);
            arduino_.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
            
            // Test connection
            arduino_.Write("\n");
            std::string response;
            try {
                arduino_.ReadLine(response, '\n', 100); // 100ms timeout
                RCLCPP_INFO(get_logger(), "Successfully connected to %s", port.c_str());
            } catch (const LibSerial::ReadTimeout&) {
                RCLCPP_WARN(get_logger(), "No response from device, but port is open");
            }
            
        } catch (const LibSerial::OpenFailed& e) {
            RCLCPP_ERROR(get_logger(), "Failed to open port %s: %s", port.c_str(), e.what());
            RCLCPP_ERROR(get_logger(), "Try: sudo chmod 666 %s", port.c_str());
            rclcpp::shutdown();
        } catch (const std::exception& e) {
            RCLCPP_ERROR(get_logger(), "Error configuring port %s: %s", port.c_str(), e.what());
            rclcpp::shutdown();
        }
    }

    ~SerialTransmitter()
    {
        if (arduino_.IsOpen()) {
            arduino_.Close();
        }
    }

    
    void messageCallback(const std_msgs::msg::String & msg)
    {
        try {
            if (arduino_.IsOpen()) {
                RCLCPP_INFO_STREAM(get_logger(), "Sending: " << msg.data);
                arduino_.Write(msg.data + "\n");
                
                // Vent på respons
                std::string response;
                try {
                    arduino_.ReadLine(response, '\n', 1000);  // 1 second timeout
                    RCLCPP_INFO(get_logger(), "Received response: %s", response.c_str());
                } catch (const LibSerial::ReadTimeout&) {
                    RCLCPP_WARN(get_logger(), "No response received within timeout");
                }
            } else {
                RCLCPP_ERROR(get_logger(), "Serial port is not open");
            }
        } catch (const std::exception& e) {
            RCLCPP_ERROR(get_logger(), "Error writing to serial port: %s", e.what());
        }
    }

private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    LibSerial::SerialPort arduino_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SerialTransmitter>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
} 