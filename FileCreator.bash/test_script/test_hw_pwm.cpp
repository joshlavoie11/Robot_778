#include <pigpiod_if2.h>
#include <iostream>
#include <unistd.h>

int pi;

void testHardwarePWM() {
    std::cout << "=== TEST HARDWARE PWM ===" << std::endl;
    
    // Utilise hardware_PWM au lieu de set_PWM_dutycycle
    int result1 = hardware_PWM(pi, 12, 1000, 500000);  // GPIO 12, 1kHz, 50% duty
    std::cout << "hardware_PWM GPIO 12: " << result1 << " (0=success)" << std::endl;
    
    int result2 = hardware_PWM(pi, 13, 1000, 500000);  // GPIO 13, 1kHz, 50% duty
    std::cout << "hardware_PWM GPIO 13: " << result2 << " (0=success)" << std::endl;
    
    std::cout << "\nMesure GPIO 12 et 13 maintenant (devraient osciller)" << std::endl;
    std::cout << "Attends 30 secondes..." << std::endl;
    sleep(30);
    
    // Stop PWM
    hardware_PWM(pi, 12, 0, 0);
    hardware_PWM(pi, 13, 0, 0);
}

int main() {
    pi = pigpio_start(NULL, NULL);
    if (pi < 0) {
        std::cerr << "Erreur pigpio: " << pi << std::endl;
        return 1;
    }
    
    std::cout << "Connecté au daemon pigpio" << std::endl;
    
    testHardwarePWM();
    
    pigpio_stop(pi);
    return 0;
}