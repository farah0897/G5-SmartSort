from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    urdf_path = PathJoinSubstitution(
        [FindPackageShare("arduinobot_description"), "urdf", "test.urdf.xacro"]
    )

    robot_description = Command([
        FindExecutable(name="xacro"), " ", urdf_path
    ])
    
    debug_urdf = ExecuteProcess(
        cmd=['xacro', urdf_path],
        output='screen'
    )
    
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": robot_description}],
        output="screen"
    )

    joint_state_publisher = Node(
        package="joint_state_publisher",
        executable="joint_state_publisher",
        name="joint_state_publisher"
    )

    rviz_config = PathJoinSubstitution(
        [FindPackageShare("arduinobot_description"), "config", "test.rviz"]
    )
    
    rviz = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="screen",
        arguments=["-d", rviz_config],
    )

    return LaunchDescription([
        debug_urdf,
        robot_state_publisher,
        joint_state_publisher,
        rviz,
    ]) 