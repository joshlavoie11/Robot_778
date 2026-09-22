#include <pigpiod_if2.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>

// CONFIGURATION
const int SERVO_PIN = 25; // GPIO 25 (Pin physique 22)

// Largeurs d'impulsion (en microsecondes)
// Pour la plupart des servos : 500 = 0°, 1500 = 90°, 2500 = 180°
const int PULSE_MIN = 1000;
const int PULSE_MAX = 2000;
const int PULSE_MID = 1500;
const int PULSE_OFF = 0;    // Éteint le signal (relâche le servo)

int pi;
bool running = true;

// Gestionnaire pour Ctrl+C
void signalHandler(int signum) {
    std::cout << "\nArrêt demandé..." << std::endl;
    running = false;
}

void setup() {
    pi = pigpio_start(NULL, NULL);
    if (pi < 0) {
        std::cerr << "Erreur : Impossible de se connecter au démon pigpio." << std::endl;
        exit(1);
    }
    
    // On s'assure que le mode est OUTPUT
    set_mode(pi, SERVO_PIN, PI_OUTPUT);
    std::cout << "Connexion pigpio établie. Servo sur GPIO " << SERVO_PIN << std::endl;
}

int main() {
    signal(SIGINT, signalHandler);
    setup();

    std::cout << "=== TEST SERVO MOTEUR ===" << std::endl;
    std::cout << "1. Centrage (90°)..." << std::endl;
    
    // Commande directe à une position
    set_servo_pulsewidth(pi, SERVO_PIN, PULSE_MID);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    while(running) {
        std::cout << "2. Balayage lent (Sweep) 0° -> 180°..." << std::endl;
        
        // Boucle pour aller de MIN à MAX doucement
        for (int pulse = PULSE_MIN; pulse <= PULSE_MAX; pulse += 10) {
            if (!running) break;
            set_servo_pulsewidth(pi, SERVO_PIN, pulse);
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Vitesse du mouvement
        }

        if (!running) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "3. Retour rapide à 0°..." << std::endl;
        set_servo_pulsewidth(pi, SERVO_PIN, PULSE_MIN);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "4. Aller à 180°..." << std::endl;
        set_servo_pulsewidth(pi, SERVO_PIN, PULSE_MAX);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Décommentez la ligne ci-dessous pour arrêter après un seul cycle
        // running = false; 
    }

    // Nettoyage final
    std::cout << "Désactivation du servo..." << std::endl;
    set_servo_pulsewidth(pi, SERVO_PIN, PULSE_OFF); // Arrête d'envoyer le signal
    pigpio_stop(pi);
    
    return 0;
}