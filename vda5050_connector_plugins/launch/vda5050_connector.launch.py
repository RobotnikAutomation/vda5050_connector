from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch.actions import GroupAction
from launch_ros.actions import Node, PushRosNamespace
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():

	node_ns = LaunchConfiguration("node_ns", default="robot")
	use_sim = LaunchConfiguration("use_sim", default="true")

	config = PathJoinSubstitution([
		FindPackageShare('vda5050_connector_ros2'),
		'config',
		'config.yaml'
	])

	laser_scan = Node(
		package="vda5050_connector_ros2",
		executable="vda5050_connector_node",
		name="vda5050_connector",
		parameters=[config, {'use_sim_time': use_sim}],
	)

	group = GroupAction([
		PushRosNamespace(node_ns),
		laser_scan,
	])

	return LaunchDescription([group])
