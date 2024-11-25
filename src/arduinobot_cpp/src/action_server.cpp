#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <arduinobot_msgs/action/fibonacci.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <memory>
#include <thread>
#include <functional>  // Needed for std::placeholders

namespace arduinobot_cpp
{
    class SimpleActionServer : public rclcpp::Node
    {
    public:
        explicit SimpleActionServer(const rclcpp::NodeOptions& options = rclcpp::NodeOptions())
            : Node("simple_action_server", options)
        {
            action_server_ = rclcpp_action::create_server<arduinobot_msgs::action::Fibonacci>(
                this,
                "fibonacci",
                std::bind(&SimpleActionServer::goalCallback, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&SimpleActionServer::cancelCallback, this, std::placeholders::_1),
                std::bind(&SimpleActionServer::acceptedCallback, this, std::placeholders::_1)
            );

            RCLCPP_INFO(this->get_logger(), "Action server has been started!");
        }

    private:
        rclcpp_action::Server<arduinobot_msgs::action::Fibonacci>::SharedPtr action_server_;

        rclcpp_action::GoalResponse goalCallback(
            const rclcpp_action::GoalUUID & uuid,
            std::shared_ptr<const arduinobot_msgs::action::Fibonacci::Goal> goal)
        {
            (void) uuid;  // Mark uuid as unused
            RCLCPP_INFO_STREAM(this->get_logger(), "Received goal request with order " << goal->order);
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }

        void acceptedCallback(
            const std::shared_ptr<rclcpp_action::ServerGoalHandle<arduinobot_msgs::action::Fibonacci>> goal_handle)
        {
            std::thread{std::bind(&SimpleActionServer::execute, this, goal_handle)}.detach();
        }

        void execute(
            const std::shared_ptr<rclcpp_action::ServerGoalHandle<arduinobot_msgs::action::Fibonacci>> goal_handle)
        {
            RCLCPP_INFO(this->get_logger(), "Executing goal");
            rclcpp::Rate loop_rate(1);

            const auto goal = goal_handle->get_goal();
            auto feedback = std::make_shared<arduinobot_msgs::action::Fibonacci::Feedback>();
            auto& partial_sequence= feedback->partial_sequence;  // Use the correct field name for Feedback

            partial_sequence.push_back(0);
            partial_sequence.push_back(1);

            auto result = std::make_shared<arduinobot_msgs::action::Fibonacci::Result>();

            for (int i = 1; (i < goal->order) && rclcpp::ok(); ++i)
            {
                if (goal_handle->is_canceling())  // Use the correct method
                {
                    result->partial_sequence= partial_sequence;
                    goal_handle->canceled(result);
                    RCLCPP_INFO(this->get_logger(), "Goal canceled");
                    return;
                }
                partial_sequence.push_back(partial_sequence[i] + partial_sequence[i - 1]);
                goal_handle->publish_feedback(feedback);
                RCLCPP_INFO(this->get_logger(), "Publish feedback: %d", feedback->partial_sequence.back());  // Update field name
                loop_rate.sleep();
            }

            if (rclcpp::ok())
            {
                result->partial_sequence= partial_sequence;
                goal_handle->succeed(result);
                RCLCPP_INFO(this->get_logger(), "Goal succeeded");
            }
        }

        rclcpp_action::CancelResponse cancelCallback(
            const std::shared_ptr<rclcpp_action::ServerGoalHandle<arduinobot_msgs::action::Fibonacci>> goal_handle)
        {
            RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
            return rclcpp_action::CancelResponse::ACCEPT;
        }
    };
}

RCLCPP_COMPONENTS_REGISTER_NODE(arduinobot_cpp::SimpleActionServer)
