#include <rclcpp/rclcpp.hpp>
#include <memory>
#include <arduinobot_msgs/srv/add_two_ints.hpp>

class SimbleServiceServer : public rclcpp::Node
{
public:
    SimbleServiceServer() : Node("simble_service_server") 
    {
        service_ = create_service<arduinobot_msgs::srv::AddTwoInts>("add_two_ints", std::bind(&SimbleServiceServer::serviceCallback, this, std::placeholders::_1, std::placeholders::_2));
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Service add_two_ints iscreated");
    }


private:
    rclcpp::Service<arduinobot_msgs::srv::AddTwoInts>::SharedPtr service_;

    void serviceCallback(const std::shared_ptr<arduinobot_msgs::srv::AddTwoInts::Request> request, const std::shared_ptr<arduinobot_msgs::srv::AddTwoInts::Response> response)
    {
        
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Incoming request Received: a: " << request->a << " b: " << request->b);
        response->sum = request->a + request->b;
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Sending back response: " << response->sum);
    }
};

int main(int argc, char*argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SimbleServiceServer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
