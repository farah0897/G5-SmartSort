from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='arduinobot_vision',
            executable='realsense_node',
            name='realsense_node',
            output='screen'
        )
    ])