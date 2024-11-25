import os
# Import necessary modules for working with packages and defining launch descriptions
from launch_ros.actions import Node
from launch import LaunchDescription
from launch_ros.parameter_descriptions import ParameterValue  # Corrected spelling
from launch.substitutions import Command, LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
from launch.actions import DeclareLaunchArgument
from launch.conditions import UnlessCondition

def generate_launch_description():

    is_simulated = LaunchConfiguration(
        "is_simulated", default="true"
    )


    robot_description = ParameterValue(Command(
        [
            "xacro ",
             os.path.join(get_package_share_directory("arduinobot_description"), "urdf", "arduinobot.urdf.xacro"),
             "is_simulated:=false",
        ]), value_type=str)

    robot_state_publisher = Node(
        package="robot_state_publisher",  # Package name
        executable="robot_state_publisher",  # Executable for publishing robot states
        parameters=[{"robot_description": robot_description, "use_sim_time": True}],
        condition=UnlessCondition(is_simulated)
    )

    controller_manager = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[{"robot_description": robot_description}, {"use_sim_time": is_simulated}, 
                    os.path.join(get_package_share_directory("arduinobot_controller"), 
                                 "config", 
                                "arduinobot_controllers.yaml")],
        condition=UnlessCondition(is_simulated)
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",  # Corrected package name
        executable="spawner",  # Corrected typo in "executable"
        arguments=[
            "joint_state_broadcaster", 
            "--controller-manager", 
            "/controller_manager"]
    )

    arm_controller_spawner = Node(
        package="controller_manager",  # Corrected package name
        executable="spawner",  # Corrected typo in "executable"
        arguments=[
            "arm_controller", 
            "--controller-manager", 
            "/controller_manager"]
    )

    gripper_controller_spawner = Node(
        package="controller_manager",  # Corrected package name
        executable="spawner",  # Corrected typo in "executable"
        arguments=[
            "gripper_controller", 
            "--controller-manager", 
            "/controller_manager"]
    )

    return LaunchDescription([
        is_simulated,
        robot_state_publisher,
        controller_manager,
        joint_state_broadcaster_spawner,
        arm_controller_spawner,
        gripper_controller_spawner
    ])
