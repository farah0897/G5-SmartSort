#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <functional>
#include <memory>

using std::placeholders::_1;

class SimpleSubscriber : public rclcpp::Node
{
public:
    SimpleSubscriber() : Node("simple_subscriber")
    {
        // Opprett subscriber med topic navn "chatter" og buffer størrelse 10
        subscription_ = create_subscription<std_msgs::msg::String>(
            "chatter", 
            10, 
            std::bind(&SimpleSubscriber::messageCallback, this, _1));
        
        RCLCPP_INFO(get_logger(), "SimpleSubscriber node har startet");
    }
    
    void messageCallback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO_STREAM(get_logger(), "Jeg mottok: " << msg->data);
    }


private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SimpleSubscriber>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
} 