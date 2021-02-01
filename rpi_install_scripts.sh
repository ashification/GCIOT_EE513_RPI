 ##############################################
## 		         Set up file		                ##
## ------------------------------------------ ## 
##| Editor | Version |    Date    | Comments |## 
## ------------------------------------------ ##    
##   ALee      1.0      10/05/18    Created   ##
##   ALee      1.1      21/05/18    bluepy    ##
##   ALee	     1.2		  01/02/21    add vim   ##
## 					                                  ##
 ##############################################


#!/bin/bash

#Start Up
echo "Welcome to system set up" 
echo "HOST: " $HOSTNAME
echo "USER: " $USER

#Configure internet connection
echo "Configure internet connection"
#echo "Are you using WiFi ? [Y = 1 /N = 0] "   
#read wirelessconnection #
#
#if[wirelessconnection=1]
#then
#https://unix.stackexchange.com/questions/92799/connecting-to-wifi-network-through-command-line
#	sudo dhclient wlan0
#	##nmcli dev wifi connect $ssid password $password
#
#fi

hostname -I # Display IP address

# Install
echo "Installing software and updates"
sudo apt-get update
sudo apt-get upgrade

sudo apt-get install vim
sudo apt-get install  nmap  # SSH
sudo apt-get install  xrdp  # Remote PC
sudo apt-get install  nmcli #
sudo apt-get install  python
sudo apt-get install  mysql-server
sudo apt-get install  npm i bluetoothctl
sudo apt-get install  bluez					#Bluetooth libraries
sudo apt-get install  bluez-utils
				#Bluetooth controls
 sudo apt-get install python-pip
 sudo apt-get install libglib2.0-dev
 sudo pip install bluepy

sudo pip     install  pygatt
sudo pip     install  "pygatt[GATTTOOL]"		 	 #Library relative to script being ran
sudo pip     install  git+https://github.com/peplin/pygatt  	 #Library relative to script being ran
sudo -E pip  install  pexpect					 #Library relative to script being ran


#Configure SSH
echo "activating SSH functionality"
raspi-config nonint # Runs RPI config as non nteractive
do_ssh 0	    # Activates SSH if not already done so
