#!/usr/bin/env python3
"""
Nœud de lancement de la perception Jerro

Ce fichier de lancement ROS2 orchestrate le démarrage de plusieurs nœuds et configurations
pour le pipeline de perception du robot Jerro, incluant la capture vidéo, le traitement
d'images et l'intégration LIDAR.

Auteur: Jalen Gauthier, Alexis Richjkaard
Date: 3 février 2026
"""

from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    """
    Generate the launch description for the Jerro perception system.

    This launch file orchestrates the startup of multiple ROS2 nodes and includes
    external launch configurations for a complete perception pipeline:

    - v4l2_camera_node: Captures raw images from a USB camera using the v4l2 driver
        and publishes them with support for both raw and compressed image transport formats.
    - cam_traitement_node: Processes camera images from the v4l2_camera_node.
    - lidar_traitement_node: Processes LIDAR sensor data.
    - sllidar_s2_launch: Includes the SLAMTEC LIDAR S2 launch configuration for
        LIDAR initialization and data publishing.

    Returns:
            LaunchDescription: A ROS2 launch description containing all nodes and
                                                configurations needed for the perception subsystem.
    """
    return LaunchDescription([
        Node(
            package='v4l2_camera',
            executable='v4l2_camera_node',
            name='v4l2_camera_node',
            parameters=[{
                'image_raw.enable_pub_plugins': ['image_transport/raw','image_transport/compressed']
            }]
        ),
        Node(
            package='jerro_perception',
            executable='cam_traitement',
            name='cam_traitement_node'
        ), 
        Node(
            package='jerro_perception',
            executable='lidar_traitement',
            name='lidar_traitement_node'
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([
                get_package_share_directory('sllidar_ros2'),
                '/launch/sllidar_s2_launch.py'
            ]),
            launch_arguments=[('frame_id', 'lidar')]
        ),
        Node(
            package='jerro_drivers',
            executable='servomotor',
            name='servomotor'
        ),
        ])