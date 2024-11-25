from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration
from launch.conditions import UnlessCondition, IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from moveit_configs_utils import MoveItConfigsBuilder
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # Declare arguments
    is_simulated = DeclareLaunchArgument(
        "is_simulated",
        default_value="true",
        description="Use simulation (Gazebo) clock if true"
    )

    use_python_arg = DeclareLaunchArgument(
        "use_python",
        default_value="False",
        description="Use Python implementation if true"
    )

    # Get configurations
    use_python = LaunchConfiguration("use_python")
    is_simulated = LaunchConfiguration("is_simulated")

    # Configure MoveIt
    moveit_config = MoveItConfigsBuilder("arduinobot", 
        package_name="arduinobot_moveit"
    ).robot_description_file(
        os.path.join(
            get_package_share_directory('arduinobot_description'),
            "urdf",
            "arduinobot.urdf.xacro"
        )
    ).robot_description_semantic_file(
        file_path='config/arduinobot.srdf'
    ).trajectory_execution(
        file_path="config/moveit_controller.yaml"
    ).moveit_cpp(
        file_path="config/planning_python_api.yaml"
    ).to_moveit_configs()

    # C++ Task Server Node
    task_server_node = Node(
        package="arduinobot_remote",
        executable="task_server_node",
        condition=UnlessCondition(use_python),
        parameters=[{'use_sim_time': is_simulated}],
        output='screen'
    )

    # Python Task Server Node
    task_server_node_py = Node(
        package="arduinobot_remote",
        executable="task_server.py",
        condition=IfCondition(use_python),
        parameters=[
            moveit_config.to_dict(), 
            {"use_sim_time": is_simulated}
        ],
        output='screen'
    )

    # Alexa Interface Node
    alexa_interface_node = Node(
        package='arduinobot_remote',
        executable='alexa_interface',
        parameters=[{'use_sim_time': is_simulated}],
        output='screen'
    )

    return LaunchDescription([
        is_simulated,
        use_python_arg,
        task_server_node,
        task_server_node_py,
        alexa_interface_node
    ])
