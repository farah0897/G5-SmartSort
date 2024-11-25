from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    urdf_path = PathJoinSubstitution(
        [FindPackageShare("arduinobot_description"), "urdf", "arduinobot.urdf.xacro"]
    )
    
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": Command([FindExecutable(name="xacro"), " ", urdf_path])}],
    )
    
    joint_state_publisher_gui = Node(
        package="joint_state_publisher_gui",
        executable="joint_state_publisher_gui",
    )
    
    rviz_config = PathJoinSubstitution(
        [FindPackageShare("arduinobot_description"), "config", "display.rviz"]
    )
    
    rviz = Node(
        package="rviz2",
        executable="rviz2",
        arguments=["-d", rviz_config],
    )

    # Add a static transform publisher
    static_transform = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=['0', '0', '0', '0', '0', '0', 'world', 'base_link']
    )

    return LaunchDescription([
        robot_state_publisher,
        joint_state_publisher_gui,
        static_transform,
        rviz,
    ])

