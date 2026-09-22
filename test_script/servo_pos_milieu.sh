#!/bin/bash
echo "Positionnement servo à 512"

# Configurer GPIO 1 pour le PWM
gpio mode 1 pwm
gpio pwm-ms
gpio pwmc 192
gpio pwmr 2000
gpio pwm 1 150
sleep 1

# Placer le servo à la position 512
gpio pwm 1 512

# Le servo reste à cette position tant que le script tourne
# sleep infini pour laisser le servo en place
while true; do
    sleep 60
done

