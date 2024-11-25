from launch import LaunchDescription
from ament_index_python import get_package_share_directory
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
import os
def generate_launch_description():

    gazebo = IncludeLaunchDescription(
        os.path.join(get_package_share_directory("arduinobot_description"), 
                     "launch", "gazebo.launch.py")
    )

    controller = IncludeLaunchDescription(
        os.path.join(get_package_share_directory("arduinobot_controller"), 
                     "launch", "controller.launch.py"),
        launch_arguments={"is_simulated": "true"}.items()
    )

    moveit = IncludeLaunchDescription(
        os.path.join(get_package_share_directory("arduinobot_moveit"), 
                     "launch", "moveit.launch.py"),
        launch_arguments={"is_simulated": "true"}.items()
    )

    remote_interface = IncludeLaunchDescription(
        os.path.join(get_package_share_directory("arduinobot_remote"), 
                     "launch", "remote_interface.launch.py")
    )

    return LaunchDescription([
        gazebo, 
        controller, 
        moveit, 
        remote_interface
    ])
