#!/bin/bash
echo -e "script test GPA778\c "
cd ~/GPA778/RED
gpio mode 25 out
gpio write 25 1 	#Alimente H-Drive
#gnome-terminal -- ./RED -a23 -b24      # Demarre un terminal pour lire encodeur Gauche
#gnome-terminal -- ./RED -a17 -b27	#demarre un terminal pour lire encodeur Droit
gpio mode 27 out
gpio write 27 1		#Enable Moteur 1
gpio mode 3 out
gpio write 3 1		#Enable Moteur 2
gpio mode 26 pwm	#Mode PWM sur DIR1 (moteur gauche)
gpio mode 23 pwm	#Mode PWM sur DIR2 (moteur droit)
gpio pwmc 192		# Set rep- Sacaler for 50 khz

for pin in 26 23 ;	# Loop for sur deux pin moteur 26 gauche 23 droit
do
num1=512		# Valeur Arret
gpio pwm 26 $num1   # met valeur 512 (valeur a l arret)
gpio pwm 23 $num1   # met valeur 512 (valeur a l arret)
clear


	echo -e "** GPIO:26 Roue Droite,   GPIO:23 Roue Gauche\n"
	for y in + -;  	# fais loop 2x (+) horraire  (-) anti-horaire
		do
		for i in {1..5}
		do

		vitesse=$((num1 $y (100 * $i))) #512 (+ou -) 100 x nombre de repetion
		gpio pwm $pin $vitesse

		echo -e " GPIO : $pin \n Sens: $y \n Vitesse: $vitesse\c"
		echo "    "
		echo $j
		sleep 1

done;
done;
done
gpio write 27 0 #diasble moteur gauche
gpio write 3 0 #disable moteur droit
cd ~
