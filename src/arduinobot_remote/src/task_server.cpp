#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <arduinobot_msgs/action/arduinobot_task.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <memory>
#include <thread>

namespace arduinobot_remote
{
    class TaskServer : public rclcpp::Node
    {
    public:
        explicit TaskServer(const rclcpp::NodeOptions& options = rclcpp::NodeOptions())
            : Node("task_server", options)
        {
            using namespace std::placeholders;

            action_server_ = rclcpp_action::create_server<arduinobot_msgs::action::ArduinobotTask>(
                this,
                "arduinobot_task",
                std::bind(&TaskServer::handleGoal, this, _1, _2),
                std::bind(&TaskServer::handleCancel, this, _1),
                std::bind(&TaskServer::handleAccepted, this, _1)
            );

            RCLCPP_INFO(this->get_logger(), "Task server er startet!");
        }

    private:
        rclcpp_action::Server<arduinobot_msgs::action::ArduinobotTask>::SharedPtr action_server_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> arm_group_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> gripper_group_;

        void initializeMoveGroups()
        {
            arm_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
                shared_from_this(), "arduinobot_arm");
            gripper_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(
                shared_from_this(), "arduinobot_hand");

            arm_group_->setPlanningTime(15.0);
            gripper_group_->setPlanningTime(5.0);
        }

        rclcpp_action::GoalResponse handleGoal(
            const rclcpp_action::GoalUUID&,
            std::shared_ptr<const arduinobot_msgs::action::ArduinobotTask::Goal> goal)
        {
            RCLCPP_INFO(this->get_logger(), "Mottok forespørsel om task: %s", goal->task_number.c_str());
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }

        rclcpp_action::CancelResponse handleCancel(
            const std::shared_ptr<rclcpp_action::ServerGoalHandle<arduinobot_msgs::action::ArduinobotTask>>)
        {
            RCLCPP_INFO(this->get_logger(), "Mottok kansellering av task");
            return rclcpp_action::CancelResponse::ACCEPT;
        }

        void handleAccepted(
            const std::shared_ptr<rclcpp_action::ServerGoalHandle<arduinobot_msgs::action::ArduinobotTask>> goal_handle)
        {
            std::thread{std::bind(&TaskServer::executeTask, this, goal_handle)}.detach();
        }

        bool checkGripperBounds(const double target_width)
        {
            if (!gripper_group_) {
                initializeMoveGroups();
            }

            const double MIN_WIDTH = 0.0;    // Helt lukket
            const double MAX_WIDTH = 0.03;   // Helt åpen

            if (target_width < MIN_WIDTH || target_width > MAX_WIDTH) {
                RCLCPP_ERROR(this->get_logger(),
                    "Gripper bredde er utenfor grensene: %.3f (min: %.3f, max: %.3f)",
                    target_width, MIN_WIDTH, MAX_WIDTH);
                return false;
            }
            return true;
        }

        bool moveGripper(const bool open_gripper)
        {
            if (!gripper_group_) {
                initializeMoveGroups();
            }

            double target_width = open_gripper ? 0.03 : 0.0;
            
            if (!checkGripperBounds(target_width)) {
                return false;
            }

            std::vector<double> gripper_joints = {target_width, target_width};
            gripper_group_->setJointValueTarget(gripper_joints);

            moveit::planning_interface::MoveGroupInterface::Plan gripper_plan;
            bool success = (gripper_group_->plan(gripper_plan) == moveit::core::MoveItErrorCode::SUCCESS);

            if (success) {
                return gripper_group_->execute(gripper_plan) == moveit::core::MoveItErrorCode::SUCCESS;
            }
            return false;
        }

        bool checkJointLimits(const std::vector<double>& joint_positions)
        {
            if (!arm_group_) {
                initializeMoveGroups();
            }

            // Få gjeldende robot state
            moveit::core::RobotStatePtr current_state = arm_group_->getCurrentState();
            if (!current_state) {
                RCLCPP_ERROR(this->get_logger(), "Kunne ikke få current robot state");
                return false;
            }

            // Få joint model group
            const moveit::core::JointModelGroup* joint_model_group = 
                arm_group_->getRobotModel()->getJointModelGroup(arm_group_->getName());
            
            if (!joint_model_group) {
                RCLCPP_ERROR(this->get_logger(), "Kunne ikke få joint model group");
                return false;
            }

            // Sjekk antall ledd
            if (joint_positions.size() != static_cast<size_t>(joint_model_group->getVariableCount())) {
                RCLCPP_ERROR(this->get_logger(), 
                    "Feil antall ledd-posisjoner: %lu (forventet %lu)", 
                    joint_positions.size(), 
                    static_cast<size_t>(joint_model_group->getVariableCount()));
                return false;
            }

            // Sett joint verdier i current state
            current_state->setJointGroupPositions(joint_model_group, joint_positions);

            // Sjekk om posisjonene er innenfor grensene
            bool within_bounds = current_state->satisfiesBounds(joint_model_group);
            
            if (!within_bounds) {
                // Logg hvilke ledd som er utenfor grensene
                std::vector<std::string> joint_names = joint_model_group->getVariableNames();
                for (size_t i = 0; i < joint_positions.size(); ++i) {
                    const moveit::core::JointModel* joint_model = 
                        joint_model_group->getJointModel(joint_names[i]);
                    if (joint_model && !current_state->satisfiesBounds(joint_model)) {
                        RCLCPP_ERROR(this->get_logger(),
                            "Ledd '%s' er utenfor grensene: %.3f",
                            joint_names[i].c_str(), joint_positions[i]);
                    }
                }
            }

            return within_bounds;
        }

        void executeTask(
            const std::shared_ptr<rclcpp_action::ServerGoalHandle<arduinobot_msgs::action::ArduinobotTask>> goal_handle)
        {
            if (!arm_group_) {
                initializeMoveGroups();
            }

            const auto goal = goal_handle->get_goal();
            auto feedback = std::make_shared<arduinobot_msgs::action::ArduinobotTask::Feedback>();
            auto result = std::make_shared<arduinobot_msgs::action::ArduinobotTask::Result>();

            // Sett joint positions med sjekk av grenser
            arm_group_->setJointValueTarget(goal->joint_positions);
            
            if (!checkJointLimits(goal->joint_positions)) {
                result->success = false;
                result->message = "Joint positions er utenfor tillatte grenser";
                goal_handle->abort(result);
                return;
            }

            feedback->completion_percentage = 0.0;
            feedback->current_state = "Planlegger bevegelse";
            goal_handle->publish_feedback(feedback);

            moveit::planning_interface::MoveGroupInterface::Plan arm_plan;
            bool success = (arm_group_->plan(arm_plan) == moveit::core::MoveItErrorCode::SUCCESS);

            if (success) {
                feedback->completion_percentage = 30.0;
                feedback->current_state = "Utfører armbevegelse";
                goal_handle->publish_feedback(feedback);

                success = arm_group_->execute(arm_plan) == moveit::core::MoveItErrorCode::SUCCESS;
            }

            if (success) {
                feedback->completion_percentage = 60.0;
                feedback->current_state = "Kontrollerer gripper";
                goal_handle->publish_feedback(feedback);

                success = moveGripper(goal->gripper_state);
            }

            feedback->completion_percentage = 100.0;
            feedback->current_state = success ? "Task fullført" : "Task feilet";
            goal_handle->publish_feedback(feedback);

            result->success = success;
            result->message = success ? "Task fullført vellykket" : "Task feilet under utførelse";

            if (success) {
                goal_handle->succeed(result);
            } else {
                goal_handle->abort(result);
            }
        }
    };
}

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(arduinobot_remote::TaskServer) 