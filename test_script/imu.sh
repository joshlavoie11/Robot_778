#!/bin/bash
echo "test servo imu"
clear


gpio mode 4 out				# met GPIO 4  au GND pour alimenter le IMU
gpio write 4 0
sudo ~/GPA778/MPU6050/./mpu6050		# Lance le scrpit
 
