#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <memory>
#include <thread>

class SimpleMoveitInterface : public rclcpp::Node
{
public:
    SimpleMoveitInterface() : Node("simple_moveit_interface")
    {
        // Initialiser move groups for arm og gripper
        arm_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
            std::shared_ptr<rclcpp::Node>(std::move(this)), "arduinobot_arm");
        gripper_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
            std::shared_ptr<rclcpp::Node>(std::move(this)), "arduinobot_hand");

        // Sett planleggingstid
        arm_group_->setPlanningTime(15.0);
        gripper_group_->setPlanningTime(15.0);
        
        // Start en timer for å kjøre bevegelsen
        timer_ = this->create_wall_timer(
            std::chrono::seconds(2),
            std::bind(&SimpleMoveitInterface::execute_movement, this));
    }

private:
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> arm_group_;
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> gripper_group_;
    rclcpp::TimerBase::SharedPtr timer_;

    bool moveGripper(const double target_width)
    {
        std::vector<double> gripper_joints = {target_width, target_width};
        gripper_group_->setJointValueTarget(gripper_joints);

        moveit::planning_interface::MoveGroupInterface::Plan gripper_plan;
        bool success = (gripper_group_->plan(gripper_plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if (success) {
            RCLCPP_INFO(this->get_logger(), "Utfører gripper bevegelse...");
            return gripper_group_->execute(gripper_plan) == moveit::core::MoveItErrorCode::SUCCESS;
        }
        return false;
    }

    bool moveBase(const double target_angle)
    {
        std::vector<double> joint_values = arm_group_->getCurrentJointValues();
        joint_values[0] = target_angle;  // Base joint er vanligvis den første
        arm_group_->setJointValueTarget(joint_values);

        moveit::planning_interface::MoveGroupInterface::Plan base_plan;
        bool success = (arm_group_->plan(base_plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if (success) {
            RCLCPP_INFO(this->get_logger(), "Utfører base rotasjon...");
            return arm_group_->execute(base_plan) == moveit::core::MoveItErrorCode::SUCCESS;
        }
        return false;
    }

    void execute_movement()
    {
        timer_->cancel();

        // 1. Åpne gripper
        RCLCPP_INFO(this->get_logger(), "Åpner gripper...");
        moveGripper(0.03);  // Åpen posisjon
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // 2. Flytt til target posisjon
        geometry_msgs::msg::Pose target_pose;
        target_pose.orientation.w = 1.0;
        target_pose.position.x = 0.2;
        target_pose.position.y = 0.0;
        target_pose.position.z = 0.2;

        arm_group_->setPoseTarget(target_pose);
        
        moveit::planning_interface::MoveGroupInterface::Plan my_plan;
        bool success = (arm_group_->plan(my_plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if (success) {
            RCLCPP_INFO(this->get_logger(), "Flytter til target posisjon...");
            arm_group_->execute(my_plan);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // 3. Lukk gripper
        RCLCPP_INFO(this->get_logger(), "Lukker gripper...");
        moveGripper(0.0);  // Lukket posisjon
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // 4. Roter base 90 grader
        RCLCPP_INFO(this->get_logger(), "Roterer base...");
        moveBase(1.57);  // 90 grader i radianer
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // 5. Gå tilbake til home posisjon
        arm_group_->setNamedTarget("home");
        success = (arm_group_->plan(my_plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if (success) {
            RCLCPP_INFO(this->get_logger(), "Går tilbake til home posisjon...");
            arm_group_->execute(my_plan);
        }

        // 6. Åpne gripper igjen
        RCLCPP_INFO(this->get_logger(), "Åpner gripper igjen...");
        moveGripper(0.03);

        rclcpp::shutdown();
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    
    auto moveit_interface = std::make_shared<SimpleMoveitInterface>();
    
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(moveit_interface);
    executor.spin();
    
    rclcpp::shutdown();
    return 0;
} 