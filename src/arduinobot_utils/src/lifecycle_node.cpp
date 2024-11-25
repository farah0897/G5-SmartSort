#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <std_msgs/msg/string.hpp>
#include <lifecycle_msgs/msg/state.hpp>
using std::placeholders::_1;

class LifecycleSubscriber : public rclcpp_lifecycle::LifecycleNode
{
public:
    LifecycleSubscriber()
        : LifecycleNode("lifecycle_subscriber")
    {
        RCLCPP_INFO(get_logger(), "Constructing lifecycle subscriber node");
    }

    // Configure callback (from Unconfigured to Inactive)
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    on_configure(const rclcpp_lifecycle::State &)
    {
        subscription_ = create_subscription<std_msgs::msg::String>(
            "topic", 10,
            std::bind(&LifecycleSubscriber::topic_callback, this, _1));
        
        RCLCPP_INFO(get_logger(), "Lifecycle node configured() called");
        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    // Cleanup callback (from Inactive to Unconfigured)
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    on_cleanup(const rclcpp_lifecycle::State &)
    {
        subscription_.reset();
        RCLCPP_INFO(get_logger(), "on_cleanup() called");
        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    // Activate callback (from Inactive to Active)
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    on_activate(const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(get_logger(), "on_activate() called");
        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    // Deactivate callback (from Active to Inactive)
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
    on_deactivate(const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(get_logger(), "on_deactivate() called");
        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        // Only process messages if the node is in active state
        if (this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
        {
            RCLCPP_INFO(get_logger(), "Received message: '%s'", msg->data.c_str());
        }
    }

    rclcpp_lifecycle::LifecycleNode::SharedPtr node_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<LifecycleSubscriber>();
    
    rclcpp::spin(node->get_node_base_interface());
    rclcpp::shutdown();
    
    return 0;
} 