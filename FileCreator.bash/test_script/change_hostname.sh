#!/bin/bash

# Define a table of MAC addresses and corresponding hostnames
declare -A mac_to_hostname=(
	["DC:A6:32:EE:78:A4"]="robot-rouleur-ros-1"
	["DC:A6:32:EE:87:E2"]="robot-rouleur-ros-2"
	["DC:A6:32:EE:88:D6"]="robot-rouleur-ros-3"
	["DC:A6:32:EE:79:4D"]="robot-rouleur-ros-4"
	["DC:A6:32:EE:88:9F"]="robot-rouleur-ros-5"
	["DC:A6:32:EE:55:84"]="robot-rouleur-ros-6"
	["E4:5F:01:44:DA:90"]="robot-rouleur-ros-7"
	["DC:A6:32:EE:64:AB"]="robot-rouleur-ros-8"
	["DC:A6:32:EE:88:24"]="robot-rouleur-ros-9"
	["DC:A6:32:EE:5C:9B"]="robot-rouleur-ros-10"
	["DC:A6:32:EE:79:1D"]="robot-rouleur-ros-11"
	["DC:A6:32:EE:59:CA"]="robot-rouleur-ros-12"
	["DC:A6:32:EE:88:4B"]="robot-rouleur-ros-13"
	["DC:A6:32:D5:0A:38"]="robot-rouleur-ros-14"
	["DC:A6:32:EE:88:69"]="robot-rouleur-ros-15"
)

# Get the MAC address of the wireless network interface
mac_address=$(cat /sys/class/net/wlan0/address | tr '[:lower:]' '[:upper:]')

# Check if the MAC address exists in the table
if [[ -v mac_to_hostname["$mac_address"] ]]; then
	new_hostname="${mac_to_hostname[$mac_address]}"
	echo "Setting hostname to $new_hostname"

	# Change the hostname temporarily
	sudo hostnamectl set-hostname "$new_hostname"

	# Update /etc/hostname file
	echo "$new_hostname" | sudo tee /etc/hostname > /dev/null

	# Update /etc/hosts to reflect the new hostname
	sudo sed -i "s/127.0.0.1[[:space:]]*$(hostname)/127.0.0.1 $new_hostname/" /etc/hosts
else
	echo "No hostname mapping found for MAC address $mac_address"
fi
