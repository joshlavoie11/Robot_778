#!/bin/bash
echo -e "script test GPA778\c "
#cd ~/GPA778/RED
gpio mode 25 out
gpio write 25 1         #Alimente H-Drive
#gnome-terminal -- ./RED -a23 -b24      # Demarre un terminal pour lire encodeur Gauche
#gnome-terminal -- ./RED -a17 -b27      #demarre un terminal pour lire encodeur Droit
gpio mode 27 out
gpio write 27 1         #Enable Moteur 1
gpio mode 3 out
gpio write 3 1          #Enable Moteur 2
gpio mode 26 pwm        #Mode PWM sur DIR1 (moteur gauche)
gpio mode 23 pwm        #Mode PWM sur DIR2 (moteur droit)
gpio pwmc 192           # Set rep- Sacaler for 50 khz


num1=1023               # Valeur Arret
gpio pwm 26 $num1   # met valeur 512 (valeur a l arret)
gpio pwm 23 $num1   # met valeur 512 (valeur a l arret)



cd ~
