#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <arduinobot_msgs/action/fibonacci.hpp>
#include <memory>

class SimpleActionClient : public rclcpp::Node
{
public:
    explicit SimpleActionClient() : Node("simple_action_client")
    {
        client_ptr_ = rclcpp_action::create_client<arduinobot_msgs::action::Fibonacci>(
            this,
            "fibonacci");

        timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&SimpleActionClient::send_goal, this));
    }

    void send_goal()
    {
        using namespace std::placeholders;
        timer_->cancel();

        if (!client_ptr_->wait_for_action_server(std::chrono::seconds(10))) {
            RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
            rclcpp::shutdown();
            return;
        }

        auto goal_msg = arduinobot_msgs::action::Fibonacci::Goal();
        goal_msg.order = 10;

        RCLCPP_INFO(this->get_logger(), "Sending goal");

        auto send_goal_options = rclcpp_action::Client<arduinobot_msgs::action::Fibonacci>::SendGoalOptions();
        send_goal_options.feedback_callback =
            std::bind(&SimpleActionClient::feedback_callback, this, _1, _2);
        send_goal_options.goal_response_callback =
            std::bind(&SimpleActionClient::goal_response_callback, this, _1);
        send_goal_options.result_callback =
            std::bind(&SimpleActionClient::result_callback, this, _1);

        client_ptr_->async_send_goal(goal_msg, send_goal_options);
    }

private:
    rclcpp_action::Client<arduinobot_msgs::action::Fibonacci>::SharedPtr client_ptr_;
    rclcpp::TimerBase::SharedPtr timer_;

    void goal_response_callback(const rclcpp_action::ClientGoalHandle<arduinobot_msgs::action::Fibonacci>::SharedPtr& goal_handle)
    {
        if (!goal_handle) {
            RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
            return;
        }
        RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
    }

    void feedback_callback(
        rclcpp_action::ClientGoalHandle<arduinobot_msgs::action::Fibonacci>::SharedPtr,
        const std::shared_ptr<const arduinobot_msgs::action::Fibonacci::Feedback> feedback)
    {
        RCLCPP_INFO(this->get_logger(), "Next number in partial_sequence: %d", feedback->partial_sequence.back());
    }

    void result_callback(
        const rclcpp_action::ClientGoalHandle<arduinobot_msgs::action::Fibonacci>::WrappedResult& result)
    {
        switch (result.code) {
            case rclcpp_action::ResultCode::SUCCEEDED:
                RCLCPP_INFO(this->get_logger(), "Goal succeeded!");
                break;
            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(), "Goal was aborted");
                return;
            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_ERROR(this->get_logger(), "Goal was canceled");
                return;
            default:
                RCLCPP_ERROR(this->get_logger(), "Unknown result code");
                return;
        }

        RCLCPP_INFO(this->get_logger(), "Result received");
        rclcpp::shutdown();
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto action_client = std::make_shared<SimpleActionClient>();   
    rclcpp::spin(action_client);
    rclcpp::shutdown();
    return 0;
} 