#include <rclcpp/rclcpp.hpp>
#include <memory>
#include <arduinobot_msgs/srv/add_two_ints.hpp>
#include <chrono>

class SimpleServiceClient : public rclcpp::Node
{
public:
    SimpleServiceClient(int first, int second) : Node("simple_service_client")
    {
        client_ = create_client<arduinobot_msgs::srv::AddTwoInts>("add_two_ints");
     auto request = std::make_shared<arduinobot_msgs::srv::AddTwoInts::Request>();
        request->first = first;
        request->second = second;

        while (!client_->wait_for_service(std::chrono::seconds(1)))
        {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Venter på server...");
            if (!rclcpp::ok())
            {
                RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Avbrutt mens vi ventet på tjenesten.");
                return;
            }
        }

        auto result = client_->async_send_request(request, std::bind(&SimpleServiceClient::responseCallback, this, std::placeholders::_1));
    }

private:
    rclcpp::Client<arduinobot_msgs::srv::AddTwoInts>::SharedPtr client_;

    void responseCallback(rclcpp::Client<arduinobot_msgs::srv::AddTwoInts>::SharedFuture future)
    {
        if (future.valid())
        {
            RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Service response received: " << future.get()->sum);
        }
        else
        {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Service response failure");
        }
    }
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    if (argc != 3)  
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Bruk: ros2 run arduinobot_cpp_examples simple_service_client X Y");
        return 1;
    }
    auto node = std::make_shared<SimpleServiceClient>(atoi(argv[1]), atoi(argv[2]));
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
} 
