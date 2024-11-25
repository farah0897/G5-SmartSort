import os
from ament_index_python.packages import get_package_share_directory
from launch.substitutions import Command
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    package_share_directory = get_package_share_directory("arduinobot_description")
    urdf_file_path = os.path.join(package_share_directory, "urdf", "arduinobot.urdf.xacro")

    return LaunchDescription([
        Node(
            package="robot_state_publisher",
            executable="robot_state_publisher",
            name="robot_state_publisher",
            output="screen",
            parameters=[{
                "robot_description": Command(["xacro ", urdf_file_path])
            }],
        ),
        Node(
            package="rviz2",
            executable="rviz2",
            name="rviz2",
            output="screen",
            arguments=["-d", os.path.join(package_share_directory, "rviz", "display_config.rviz")],
        ),
    ])
