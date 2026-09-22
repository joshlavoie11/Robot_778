#include <pigpiod_if2.h>
#include <iostream>
#include <unistd.h>

// Méthode OFFICIELLE selon datasheet
const int DIR1 = 5;   // Direction moteur 1 (0=forward, 1=reverse)
const int EN1 = 12;   // Vitesse moteur 1 (Hardware PWM)
const int DIR2 = 6;   // Direction moteur 2
const int EN2 = 13;   // Vitesse moteur 2 (Hardware PWM)

const int S1A = 23, S1B = 24;  // Feedback moteur 1
const int S2A = 17, S2B = 27;  // Feedback moteur 2
const int SERVO = 19;          // Servo

int pi;

void setup() {
    pi = pigpio_start(NULL, NULL);
    if (pi < 0) {
        std::cerr << "Erreur pigpio" << std::endl;
        exit(1);
    }
    
    // Configure DIR en GPIO normal
    set_mode(pi, DIR1, PI_OUTPUT);
    set_mode(pi, DIR2, PI_OUTPUT);
    
    // CRITIQUE: Utilise hardware_PWM() pour activer le vrai hardware PWM
    // Fréquence 20kHz, duty cycle 0 (arrêt)
    int ret1 = hardware_PWM(pi, EN1, 20000, 0);
    int ret2 = hardware_PWM(pi, EN2, 20000, 0);
    
    std::cout << "Hardware PWM EN1: " << ret1 << std::endl;
    std::cout << "Hardware PWM EN2: " << ret2 << std::endl;
    
    if (ret1 != 0 || ret2 != 0) {
        std::cerr << "ERREUR: Hardware PWM non activé!" << std::endl;
        std::cerr << "GPIO 12 et 13 sont-ils disponibles?" << std::endl;
    }
    
    // Feedback
    set_mode(pi, S1A, PI_INPUT);
    set_mode(pi, S1B, PI_INPUT);
    set_mode(pi, S2A, PI_INPUT);
    set_mode(pi, S2B, PI_INPUT);
    
    // Servo
    set_mode(pi, SERVO, PI_OUTPUT);
    
    std::cout << "Init OK - Méthode officielle DIR+EN" << std::endl;
}

void setMotor1(int speed) {
    // speed: -255 à +255
    // Selon datasheet: DIR=0→Forward, DIR=1→Reverse
    
    if (speed > 0) {
        // Forward
        gpio_write(pi, DIR1, 0);
        // Convertit 0-255 en 0-1000000 (duty cycle en µs)
        hardware_PWM(pi, EN1, 20000, (speed * 1000000) / 255);
    } else if (speed < 0) {
        // Reverse
        gpio_write(pi, DIR1, 1);
        hardware_PWM(pi, EN1, 20000, (-speed * 1000000) / 255);
    } else {
        // Stop
        hardware_PWM(pi, EN1, 20000, 0);
    }
}

void setMotor2(int speed) {
    if (speed > 0) {
        gpio_write(pi, DIR2, 0);
        hardware_PWM(pi, EN2, 20000, (speed * 1000000) / 255);
    } else if (speed < 0) {
        gpio_write(pi, DIR2, 1);
        hardware_PWM(pi, EN2, 20000, (-speed * 1000000) / 255);
    } else {
        hardware_PWM(pi, EN2, 20000, 0);
    }
}

void setServo(int angle) {
    int pulseWidth = 500 + (angle * 2000 / 180);
    set_servo_pulsewidth(pi, SERVO, pulseWidth);
}

void stopAll() {
    hardware_PWM(pi, EN1, 20000, 0);
    hardware_PWM(pi, EN2, 20000, 0);
    set_servo_pulsewidth(pi, SERVO, 0);
}

int main() {
    setup();
    
    std::cout << "\n=== TEST MOTEUR 1 ===" << std::endl;
    std::cout << "Avant vitesse 128/255" << std::endl;
    setMotor1(128);
    sleep(3);
    
    std::cout << "Arrière vitesse 128/255" << std::endl;
    setMotor1(-128);
    sleep(3);
    
    std::cout << "Stop" << std::endl;
    setMotor1(0);
    sleep(1);
    
    std::cout << "\n=== TEST MOTEUR 2 ===" << std::endl;
    setMotor2(128);
    sleep(3);
    setMotor2(-128);
    sleep(3);
    setMotor2(0);
    
    std::cout << "\n=== TEST SERVO ===" << std::endl;
    setServo(0);
    sleep(1);
    setServo(90);
    sleep(1);
    setServo(180);
    sleep(1);
    
    stopAll();
    pigpio_stop(pi);
    return 0;
}