#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <libserial/SerialPort.h>
#include <string>

using LibSerial::SerialPort;

class SerialReceiver : public rclcpp::Node {
public:
    SerialReceiver() : Node("serial_receiver") {
        // Create publisher
        publisher_ = this->create_publisher<std_msgs::msg::String>("serial_receiver", 10);
        
        try {
            serial_port_.Open("/dev/ttyACM0");  // Make sure this matches your Arduino port
            serial_port_.SetBaudRate(LibSerial::BaudRate::BAUD_9600);
            
            // Create timer for reading serial port
            timer_ = this->create_wall_timer(
                std::chrono::milliseconds(100),
                std::bind(&SerialReceiver::timer_callback, this)
            );
            
            RCLCPP_INFO(this->get_logger(), "Serial port opened successfully");
        } catch (const LibSerial::OpenFailed& e) {
            RCLCPP_ERROR(this->get_logger(), "Failed to open serial port: %s", e.what());
        }
    }

private:
    void timer_callback() {
        std::string received_data;
        try {
            // Check if data is available to read
            if (serial_port_.IsDataAvailable()) {
                serial_port_.ReadLine(received_data);
                
                auto message = std_msgs::msg::String();
                message.data = received_data;
                publisher_->publish(message);
                
                RCLCPP_INFO(this->get_logger(), "Received and published: '%s'", received_data.c_str());
            }
        } catch (const std::exception& e) {
            RCLCPP_ERROR(this->get_logger(), "Error reading from serial port: %s", e.what());
        }
    }

    SerialPort serial_port_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SerialReceiver>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
