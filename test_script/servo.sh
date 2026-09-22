#!/bin/bash
echo "test servo motor" 
cd ~/GPA778/RED
gpio mode 1 pwm
gpio pwm-ms
gpio pwmc 192
gpio pwmr 2000
gpio pwm 1 150
sleep 1

pos=150


affiche () {
	echo -e " SERVO Position: $position\c"
		echo "    " 
		echo $j
		sleep 1
}


for i in {1..4}
		do
   
		position=$((pos + (10 * $i))) #512 (+ou -) 100 x nombre de repetion
		gpio pwm 1 $position

                affiche

                position=$((pos - (10 * $i))) #512 (+ou -) 100 x nombre de repetion	
		gpio pwm 1 $position

		affiche
		

done
gpio pwm 1 150
gpio mode 1 in


