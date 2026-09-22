#include <pigpiod_if2.h>
#include <iostream>
#include <unistd.h>

int main() {
    int pi = pigpio_start(NULL, NULL);
    if (pi < 0) {
        std::cerr << "Erreur pigpio" << std::endl;
        return 1;
    }
    
    const int DIR1 = 5;
    const int EN1 = 18;
    
    set_mode(pi, DIR1, PI_OUTPUT);
    set_mode(pi, EN1, PI_OUTPUT);
    set_PWM_frequency(pi, EN1, 1000);
    set_PWM_range(pi, EN1, 255);
    
    std::cout << "=== MESURE LES TENSIONS MAINTENANT ===" << std::endl;
    std::cout << "\nActivation DIR1 (GPIO 5)..." << std::endl;
    gpio_write(pi, DIR1, 1);
    std::cout << "→ Mesure GPIO 5 : devrait être 3.3V" << std::endl;
    std::cout << "Attends 30 secondes pour mesurer..." << std::endl;
    sleep(30);
    
    std::cout << "\nActivation EN1 PWM 100% (GPIO 18)..." << std::endl;
    set_PWM_dutycycle(pi, EN1, 255);
    std::cout << "→ Mesure GPIO 18 : devrait osciller autour de 3.3V" << std::endl;
    std::cout << "Attends 30 secondes pour mesurer..." << std::endl;
    sleep(30);
    
    std::cout << "\nFin du test" << std::endl;
    
    set_PWM_dutycycle(pi, EN1, 0);
    gpio_write(pi, DIR1, 0);
    pigpio_stop(pi);
    return 0;
}