import os
# Import necessary modules for working with packages and defining launch descriptions
from launch_ros.actions import Node
from launch import LaunchDescription
from launch_ros.parameter_descriptions import ParameterValue  # Corrected spelling
from launch.substitutions import Command, LaunchConfiguration
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    robot_description = ParameterValue(Command(
        [
            "xacro ",
             os.path.join(get_package_share_directory("arduinobot_description"), "urdf", "arduinobot.urdf.xacro")
            
        ]), value_type=str)

    robot_state_publisher = Node(
        package="robot_state_publisher",  # Package name
        executable="robot_state_publisher",  # Executable for publishing robot states
        parameters=[{"robot_description": robot_description, "use_sim_time": True}]  # Parameters include the robot description and sim time
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
        robot_state_publisher,
        joint_state_broadcaster_spawner,
        arm_controller_spawner,
        gripper_controller_spawner
    ])
