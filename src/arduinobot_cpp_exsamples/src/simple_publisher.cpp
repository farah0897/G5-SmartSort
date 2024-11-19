#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <string>

using namespace std::chrono_literals;

class SimplePublisher : public rclcpp::Node
{
public:
    SimplePublisher() : Node("simple_publisher"), counter_(0)
    {
        // Opprett publisher med topic navn "chatter" og buffer størrelse 10
        publisher_ = create_publisher<std_msgs::msg::String>("chatter", 10);
        
        // Sett opp timer for å publisere meldinger hvert sekund
        timer_ = create_wall_timer(1000ms, 
            std::bind(&SimplePublisher::timerCallback, this));
        
        RCLCPP_INFO(get_logger(), "SimplePublisher node har startet");
    }
    
    void timerCallback()
    {
        auto message = std_msgs::msg::String();
        message.data = "Hei ROS 2! Melding nr: " + std::to_string(counter_++);
        
        RCLCPP_INFO_STREAM(get_logger(), "Publiserer: " << message.data);
        publisher_->publish(message);
    }
private:
    

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    size_t counter_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SimplePublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

