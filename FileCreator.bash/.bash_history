source /opt/ros/humble/setup.bash 
ros2 run v4l2_camera v4l2_camera_node
source install/setup.bash 
ros2 launch jerro_navigation navigation_launch.py
ros2 launch foxglove_bridge foxglove_bridge_launch.xml
source install/setup.bash 
ros2 launch foxglove_bridge foxglove_bridge_launch.xml
source install/setup.bash 
ros2 launch jerro_navigation navigation_launch.py
source install/setup.bash 
ros2 run teleop_twist_keyboard teleop_twist_keyboard
colcon build
ros2 launch jerro_perception perception_launch.py
colcon build 
ros2 launch jerro_perception perception_launch.py
sudo pigpiod 
ros2 launch jerro_perception perception_launch.py
ros2 launch sllidar_ros2 sllidar_s2_launch.py
ros2 launch jerro_perception perception_launch.py
ros2 launch sllidar_ros2 sllidar_s2_launch.py
ros2 launch jerro_perception perception_launch.py
colcon build
ros2 launch jerro_perception perception_launch.py
ros2 launch sllidar_ros2 sllidar_s2_launch.py
ros2 launch jerro_perception perception_launch.py
ros2 launch sllidar_ros2 sllidar_s2_launch.py
ros2 launch jerro_perception perception_launch.py
sudo reboot
sudo pigpiod
ros2 launch jerro_navigation navigation_launch.py
source install/setup.bash 
ros2 launch jerro_navigation navigation_launch.py
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
source install/setup.bash 
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
source install/setup.bash 
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
sudo pigpiod
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ros2 action send_goal /perception/start_scan jerro_msgs/action/StartScan   "{
    start: true,
    enable_filtering: true,
    enable_crop: true,
    crop_x_min: -2.5, crop_x_max: 2.5,
    crop_y_min: -2.5, crop_y_max: 2.5,
    crop_z_min: -0.5, crop_z_max: 1.0,
    enable_statistical_filter: true,
    statistical_nb_neighbors: 20,
    statistical_std_ratio: 2.0,
    enable_voxel_downsampling: true,
    voxel_size: 0.02
  }"
ls
cd Desktop
cd /Desktop
cd ..
pwd
mkdir /lab1
mkdir/home/lab1
mkdir lab1
pwd
cd..
cd ..
mkdir lab1
man mkdir
mkdir -p home/lab1
mkdir -p work/lab1
pwd
cd lab1
cd /lab1
ls
cd /home
ls
cd lab1
ls
cd
ls
pwd
cd
pwd
cd home
cd /home
pwd
ls
cd
ls
cd /
ls
cd /home
ls
cd /ubuntu
cd /home/ubuntu
ls
cd /home
ls
cd ubuntu
cd /home
pwd
cd ubuntu
pwd
nano .bashrc 
cat  .bashrc  |grep ubunt
env |grep /ubuntu
pwd
mkdir my_scripts
ls
cd my_scripts
touch FileCreator.bash
chmod 744 FileCreator.bash
nano FileCreator.bash
./FileCreator.bash
nano FileCreator.bash
./FileCreator.bash
ls
cd fichiers
ls
rm Josh_toecap
ls
rm Josh_Toecap
ls
cd
pwd
cd my_scripts
./FileCreator.bash
cd fichiers
pwd
ls
rm LAB1_EX1
ls
cd ..
ls
scp ubuntu@10.192.8.18:/home/ubuntu/my_scripts/FileCreator.bash .
ls
man scp
ifconfig
ls
pwd
history
history | grep "ros2"
ifconfig
cd Deskstop
cd
cd desktop
cd Desktop
cd ..
pwd
ls
man ls
sudo mkdir lab1
mkdif lab1
mkdir lab1
pwd
ls
ls-p
ls
cd ..
ls
cd /homme
cd /home
ls
mkdir lab2
sudo
sudo mkdir lab2
ls
cd lab2
man cd
man ps
clear
ps-a
ps -A
ps -a
ps -A
cd
ls
mkdir -p /home/ubuntu/ros2_ws/src
source /opt/ros/humble/setup.bash
cd /home/ubuntu/ros2_ws/
colcon build
echo "source /home/ubuntu/ros2_ws/install/setup.bash" >> ~/.bashrc
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
echo "export ROS_DOMAIN_ID=<numero_robot>" >> ~/.bashrc
source ~/.bashrc
echo "export ROS_DOMAIN_ID=4" >> ~/.bashrc
source ~/.bashrc
echo "source /home/ubuntu/ros2_ws/install/setup.bash" >> ~/.bashrc
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
echo "export ROS_DOMAIN_ID=4" >> ~/.bashrc
source ~/.bashrc
ls
cd ..
ls
cd ros2_ws
source ~/.bashrc
cd /home/ubuntu/ros2_ws/src
ros2 pkg create --build-type ament_python bavardage
ls
bavardage
cd bavardage
cd /home/ubuntu/ros2_ws/src/bavardage/bavardage/
touch parleur.py auditeur.py
ls
cd ..
cd /home/ubuntu/ros2_ws
colcon build
cd /home/ubuntu
ls
cd ros2_ws
run bavardage parleur
ros2 run bavardage parleur
cd Desktop
cd ..
my_scripts
cd my_scripts
ls
cd ..
cd ros2_ws
ls
cd build
ls
cd ..
cd my_scripts
ls
scp ./FileCreator.Bash lrk@192.168.8.128:/home/Documents
ls
scp ./FileCreator.bash ubuntu@192.168.8.128:home/lrk/Documents/GPA778-01/LAB_1 
pwd
ls
cd
cd ..
ls
cd ..
ld
ls
cd home
ls
cd ubuntu
ls
cd ros2_ws
ls
cd my_script
cd ..
ls
cd my_scripts
ls
cd..
cd ..
cd ros2_ws
run bavardage auditeur
ros2 run bavardage auditeur
clear
ls
cd ..
ls
cd my_scripts
ls
cd fichiers
ls
cd ..
ls
cd ..
ls
cd ros2_ws
ls
cd build
ls
cd 
ls
cd my_sripts
cd my_scripts
ls
pwd
cd
ls
cd my_scripts
ls
pwd
ls
cd ros2_ws
ros2 run Parleur
ros2 run bavardage parleur
ls
cd src
ls
cd bavardage
ls
cd /ros2_ws
cd ros2_ws
pwd
ls
ros2 pkg create --build-type ament_python jerro_navigation --dependencies rclpy geometry_msgs std_msgs  jerro_msgs
ls
gpio readall
[200~ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 100.0, motor_speed_b: 100.0}"
~ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 100.0, motor_speed_b: 100.0}"
ros2 topic echo /encoder_a
ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 100.0, motor_speed_b: 100.0}"
ros2 topic echo /encoder_a
[200~ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 100.0, motor_speed_b: 100.0}"
[200~ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: .0, motor_speed_b: 0}"
[200~ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 100.0, motor_speed_b: 100.0}"
[200~ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 0, motor_speed_b: 0}"
ros2 topic echo /encoder_b
clear
[200~ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 0, motor_speed_b: 0}"
ros2 topic echo /encoder_a~
[200~ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 0, motor_speed_b: 0}"
ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 0, motor_speed_b: 0}"
ros2 topic echo /encoder_a
clear
pwd
cd home
ls
cd ros2_ws
ls
[200~ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 0, motor_speed_b: 0}"
ls
mkdir jerro_navigation
ls
rm jerro_navigation
sudo rm jerro_navigation
rm jerro_navigation
rmdir jerro_navigation
ls
gpio readall
sudo pigpiod
pwd
cd ~/ros2_ws/src
git clone https://github.com/Lab-CORO/jerro
colcon build -h
colcon build packages-up-to jerro_msgs
colcon build packages-up-to [jerro_msgs]
colcon build --packages-up-to jerro_msgs
ros2 launch jerro_drivers motor_control.launch.py
cd jerro_navigation
cd ..
ros2 run jerro_navigation twist_to_motors
ls
ros2 run jerro_navigation twist_to_motors
colcon build
ros2 run jerro_navigation twist_to_motors
colcon build
ros2 run jerro_navigation twist_to_motors
ros2 run teleop_twist_keyboard teleop_twist_keyboard
ls
cd ros2_ws
ls
cd jerro_navigation
ls
cd jerro_navigation
ls
run twist_to_motor
ros2 run jerro_navigation
cd ..
ls
ls
ros2_ws run build jerro_navigation
ros
ros2 run build jerro_navigation
ros2 run ros2_ws jerro_navigation
ls
cd jerro_navigation
ls
ros2 run jerro_navigation
ros2 run jerro_navigation twist_to_motors
colcon build ros2_ws
colcon build ros2_ws jerro_navigation
colcon build ros2_ws/jerro_navigation
colcon ros2_ws
colcon build
ros2 run jerro_navigation twist_to_motors
cd ..
ls
cd ..
ls
colcon build
ros2 run jerro_navigation twist_to_motors
cd ~/ros2_ws
pwd
ls
rm -rf build install log
ls
cd ~/ros2_ws
colcon Build
colcon build
clear
ros2 run jerro_navigation twist_to_motors
source ~/ros2_ws/install/setup.bash
ros2 run jerro_navigation twist_to_motors
ros2 run jerro_navigation jerro_navigation
ros2 run jerro_navigation twist_to_motors
ros2 run jerro_drivers motor_pid_controller
ros2 node list
ubuntu@ubuntu:~/ros2_ws$ ros2 node list
/twist_to_motor
ros2 node info /twist_to_motor
ros2 topic info /cmd_vel -v
ros2 topic info /motor/set_speed -v
ros2 run teleop_twist_keyboard teleop_twist_keyboard
ros2 run teleop_twist_keyboard teleop_twist_keyboard
source install/setup.bash
ros2 run jerro_navigation twist_to_motors
ros2 pkg executables jerro_drivers
[200~cd ~/ros2_ws
source install/setup.bash
cd ~/ros2_ws
source install/setup.bash
ros2 run jerro_drivers motor_pid_controller
cd ~/ros2_ws
pwd
source install/setup.bash
ros2 run jerro_drivers motor_pid_controller
which pigpiod
pgrep -a pigpiod
sudo pigpiod
pgrep -a pigpiod
ros2 run jerro_drivers motor_pid_controller
clear
ros2 topic info /motor/set_speed -v
ros2 node info /motor_pid_controller
ros2 topic info /encoder_a -v
ls
ls ros2_ws
ls ros2_ws jerro_navigation
cd ros2_ws
jerro_navigation
cd jerro_navigation
ls
cd jerro_navigation
ls
cd ..
cd ros2_ws
ros2 run teleop_twist_keyboard teleop_twist_keyboard
ls
cd jerro_navigation
ls
cd..
cd ..
ls
cd pigpio
ls
cd ..
cd ros2_ws
ls
cd log
ls
cd ..
cd..
cd ..
cd ros2_ws
ls
cd log
ls
cd ..
cd log
ls
cd ..
ls
cd jerro_navigation
ls
cd log
ls
cd..
cd ..
ls
cd jerro_navigation
ls
cd ..
ls
cd pigpio
ls
cd build
ls
cd lib
ls
cd ..
cd..
cd ..
ls
cd install
ls
cd jerro_drivers
ls
cd lib
ls
cd jerro_drivers
ls
cd ..
ls
cd ros2_ws
ls
cd build
ls
cd jerro_drivers
ls
cd ..
ls
cd jerro_navigation
ls
ls
cd jerro_navigat
ls
cd jerro_navigation
ls
cd ..
ls
cd.
ls
cd src
ls
cd jerro
ls
cd jerro_driver
cd jerro_drivers
ls
cd src
ls
clear
~/ros2_ws
cd .. .. ..
cd ~
ls
pwd
ls
cd ..
ls
cd lab2
ls
cd ubuntu
cd ~
cd ubuntu
ls
~/ros2_ws
cd ros2_ws
ls
ros2 run jerro_navigation twist_to_motors
cd ~
ls
ros2 run jerro_navigation twist_to_motors
ros2 run teleop_twist_keyboard teleop_twist_keyboard
cd ~
ros2 run teleop_twist_keyboard teleop_twist_keyboard
pwd
ls
cd ..
ls
os2 run teleop_twist_keyboard teleop_twist_keyboard
ros2 run teleop_twist_keyboard teleop_twist_keyboard
ls
ro2 topic
ros2 node list
ros2 node info /twist_to_motor
ros2 node info 
ros2 topic list
ros2 topic list -t
ros2 topic info /encoder_a
ros2 topic info /encoder_b
ros2 topic info /cmd_vel
ros2 topic info /motor/set_speed
ls
cd ..
ls
cd unbuntu
cd ubuntu
ls
pwd
ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 100.0, motor_speed_b: 100.0}"
ros2 topic echo /encoder_a
ros2 topic pub --once /motor/motor_RT_cmd jerro_msgs/msg/MotorSpeed "{motor_speed_a: 0.0, motor_speed_b: 0.0}"
ros2 topic echo /encoder_a
