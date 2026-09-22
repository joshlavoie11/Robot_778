#include <pigpiod_if2.h>
#include <iostream>
#include <unistd.h>
#include <csignal>

const int DIR1 = 5, EN1 = 12;
const int DIR2 = 6, EN2 = 13;

int pi;
bool running = true;

void signalHandler(int signum) {
    std::cout << "\nCtrl+C - Arrêt..." << std::endl;
    running = false;
}

void stopAll() {
    hardware_PWM(pi, EN1, 20000, 0);
    hardware_PWM(pi, EN2, 20000, 0);
}

void testFrequency(int frequency, const char* description) {
    std::cout << "\n=== Test " << description << " (" << frequency << " Hz) ===" << std::endl;
    std::cout << "Moteur 1 à 50% pendant 3 secondes" << std::endl;
    std::cout << "Écoute si c'est fluide ou haché..." << std::endl;
    
    gpio_write(pi, DIR1, 0);
    int result = hardware_PWM(pi, EN1, frequency, 500000);  // 50% duty
    
    if (result != 0) {
        std::cout << "ERREUR: Impossible de configurer " << frequency << " Hz (code: " << result << ")" << std::endl;
        return;
    }
    
    sleep(3);
    hardware_PWM(pi, EN1, frequency, 0);
    sleep(1);
}

int main() {
    signal(SIGINT, signalHandler);
    
    pi = pigpio_start(NULL, NULL);
    if (pi < 0) {
        std::cerr << "Erreur pigpio" << std::endl;
        return 1;
    }
    
    set_mode(pi, DIR1, PI_OUTPUT);
    set_mode(pi, DIR2, PI_OUTPUT);
    
    std::cout << "Test de différentes fréquences PWM" << std::endl;
    std::cout << "Note quelle fréquence est la plus fluide/silencieuse\n" << std::endl;
    
    // Test différentes fréquences
    testFrequency(10000, "10 kHz - Basse");
    if (!running) { stopAll(); pigpio_stop(pi); return 0; }
    
    testFrequency(20000, "20 kHz - Moyenne");
    if (!running) { stopAll(); pigpio_stop(pi); return 0; }
    
    testFrequency(30000, "30 kHz - Haute");
    if (!running) { stopAll(); pigpio_stop(pi); return 0; }
    
    testFrequency(40000, "40 kHz - Très haute");
    if (!running) { stopAll(); pigpio_stop(pi); return 0; }
    
    testFrequency(100000, "50 kHz - Ultra haute (comme script bash)");
    if (!running) { stopAll(); pigpio_stop(pi); return 0; }
    
    std::cout << "\n=== Tests terminés ===" << std::endl;
    std::cout << "Quelle fréquence était la plus fluide?" << std::endl;
    
    stopAll();
    pigpio_stop(pi);
    return 0;
}