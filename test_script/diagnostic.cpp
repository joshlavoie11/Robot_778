#include <pigpiod_if2.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <iomanip>

// GPIO pins (PmodDHB1 - configuration actuelle)
const int DIR1 = 5, EN1 = 12;
const int DIR2 = 6, EN2 = 13;
const int S1A = 23, S1B = 24;
const int S2A = 17, S2B = 27;

// H-Bridge enable pins (CRITIQUES pour faire fonctionner le PmodDHB1)
const int H_BRIDGE_POWER = 26;  // Alimente le H-Drive
const int ENABLE_MOTOR1 = 16;   // Enable Moteur 1
const int ENABLE_MOTOR2 = 22;   // Enable Moteur 2

const int PWM_FREQ = 1000;  // 1kHz - fréquence standard utilisée

int pi;
bool running = true;

struct MotorTestResult {
    bool gpio_ok;
    bool pwm_ok;
    bool motor_spins;
    int encoder_a_changes;
    int encoder_b_changes;
    bool encoder_ok;
};

void signalHandler(int signum) {
    std::cout << "\nCtrl+C - Arrêt..." << std::endl;
    running = false;
}

void stopAll() {
    set_PWM_dutycycle(pi, EN1, 0);
    set_PWM_dutycycle(pi, EN2, 0);
}

void setup() {
    pi = pigpio_start(NULL, NULL);
    if (pi < 0) {
        std::cerr << "Erreur pigpio" << std::endl;
        exit(1);
    }

    // CRITIQUE: Configurer et activer les pins H-Bridge
    std::cout << "Configuration H-Bridge..." << std::endl;
    set_mode(pi, H_BRIDGE_POWER, PI_OUTPUT);
    set_mode(pi, ENABLE_MOTOR1, PI_OUTPUT);
    set_mode(pi, ENABLE_MOTOR2, PI_OUTPUT);

    gpio_write(pi, H_BRIDGE_POWER, 1);  // Power ON H-Bridge
    gpio_write(pi, ENABLE_MOTOR1, 1);   // Enable Motor 1
    gpio_write(pi, ENABLE_MOTOR2, 1);   // Enable Motor 2
    std::cout << "H-Bridge activé (GPIO 26=HIGH, 16=HIGH, 22=HIGH)" << std::endl;

    // Configuration des pins de direction
    set_mode(pi, DIR1, PI_OUTPUT);
    set_mode(pi, DIR2, PI_OUTPUT);
    gpio_write(pi, DIR1, 0);
    gpio_write(pi, DIR2, 0);

    // Configuration des pins PWM
    set_mode(pi, EN1, PI_OUTPUT);
    set_mode(pi, EN2, PI_OUTPUT);
    set_PWM_frequency(pi, EN1, PWM_FREQ);
    set_PWM_frequency(pi, EN2, PWM_FREQ);
    set_PWM_dutycycle(pi, EN1, 0);
    set_PWM_dutycycle(pi, EN2, 0);
    std::cout << "PWM configuré à " << PWM_FREQ << " Hz" << std::endl;

    // Configuration des encodeurs
    set_mode(pi, S1A, PI_INPUT);
    set_mode(pi, S1B, PI_INPUT);
    set_mode(pi, S2A, PI_INPUT);
    set_mode(pi, S2B, PI_INPUT);

    set_pull_up_down(pi, S1A, PI_PUD_UP);
    set_pull_up_down(pi, S1B, PI_PUD_UP);
    set_pull_up_down(pi, S2A, PI_PUD_UP);
    set_pull_up_down(pi, S2B, PI_PUD_UP);

    std::cout << "Setup terminé!\n" << std::endl;
}

MotorTestResult testMotor(int motor_num, int dir_pin, int en_pin,
                          int encoder_a_pin, int encoder_b_pin,
                          const char* motor_name) {
    MotorTestResult result = {false, false, false, 0, 0, false};

    std::cout << "\n========================================" << std::endl;
    std::cout << "   TEST " << motor_name << std::endl;
    std::cout << "========================================\n" << std::endl;

    // 1. Test GPIO de contrôle
    std::cout << "1. Test GPIO direction (" << motor_name << ")" << std::endl;
    gpio_write(pi, dir_pin, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    int dir_read = gpio_read(pi, dir_pin);
    result.gpio_ok = (dir_read == 1);
    std::cout << "   DIR (GPIO " << dir_pin << "): "
              << (result.gpio_ok ? "✓ OK" : "✗ ERREUR") << std::endl;
    gpio_write(pi, dir_pin, 0);

    // 2. Test PWM
    std::cout << "\n2. Test PWM " << motor_name << " (GPIO " << en_pin << ")" << std::endl;
    std::cout << "   Activation PWM 50%..." << std::endl;
    int pwm_result = set_PWM_dutycycle(pi, en_pin, 128);  // 50% de 255
    result.pwm_ok = (pwm_result == 0);
    std::cout << "   Résultat: " << (result.pwm_ok ? "✓ OK" : "✗ ERREUR") << std::endl;

    std::cout << "\n   Le " << motor_name << " tourne-t-il? (tu dois l'ENTENDRE)" << std::endl;
    std::cout << "   Attends 3 secondes..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    set_PWM_dutycycle(pi, en_pin, 0);

    std::cout << "\n   Le moteur a-t-il tourné? (o/n): ";
    char response;
    std::cin >> response;
    result.motor_spins = (response == 'o' || response == 'O');

    if (!result.motor_spins) {
        std::cout << "\n   ❌ PROBLÈME: " << motor_name << " ne tourne pas!" << std::endl;
        return result;
    }

    std::cout << "\n   ✓ " << motor_name << " tourne!\n" << std::endl;

    // 3. Test encodeur statique
    std::cout << "3. Test encodeur " << motor_name << " (statique)" << std::endl;
    int sa = gpio_read(pi, encoder_a_pin);
    int sb = gpio_read(pi, encoder_b_pin);
    std::cout << "   Encoder A (GPIO " << encoder_a_pin << "): " << sa << std::endl;
    std::cout << "   Encoder B (GPIO " << encoder_b_pin << "): " << sb << std::endl;

    if (sa == 0 && sb == 0) {
        std::cout << "\n   ⚠️  Les deux signaux à 0 - possible problème" << std::endl;
    }

    // 4. Test encodeur dynamique
    std::cout << "\n4. Test encodeur " << motor_name << " (dynamique)" << std::endl;
    std::cout << "   " << motor_name << " à 60% PWM pendant 3 secondes" << std::endl;
    std::cout << "   Comptage des changements...\n" << std::endl;

    gpio_write(pi, dir_pin, 0);
    set_PWM_dutycycle(pi, en_pin, 153);  // 60% de 255

    int last_sa = gpio_read(pi, encoder_a_pin);
    int last_sb = gpio_read(pi, encoder_b_pin);
    result.encoder_a_changes = 0;
    result.encoder_b_changes = 0;

    auto start = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::seconds>(
           std::chrono::steady_clock::now() - start).count() < 3) {

        int curr_sa = gpio_read(pi, encoder_a_pin);
        int curr_sb = gpio_read(pi, encoder_b_pin);

        if (curr_sa != last_sa) {
            result.encoder_a_changes++;
            last_sa = curr_sa;
        }

        if (curr_sb != last_sb) {
            result.encoder_b_changes++;
            last_sb = curr_sb;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    set_PWM_dutycycle(pi, en_pin, 0);

    std::cout << "   Résultats:" << std::endl;
    std::cout << "   Encoder A changements: " << result.encoder_a_changes << std::endl;
    std::cout << "   Encoder B changements: " << result.encoder_b_changes << std::endl;

    result.encoder_ok = (result.encoder_a_changes > 10 || result.encoder_b_changes > 10);

    if (!result.encoder_ok) {
        std::cout << "\n   ❌ PROBLÈME: Encodeur " << motor_name << " ne fonctionne PAS!" << std::endl;
        std::cout << "\n   Le moteur tourne mais l'encodeur ne compte pas." << std::endl;
    } else {
        std::cout << "\n   ✓ Encodeur " << motor_name << " fonctionne!" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    return result;
}

void printFinalReport(const MotorTestResult& motor1, const MotorTestResult& motor2) {
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "   RAPPORT FINAL - COMPARAISON" << std::endl;
    std::cout << "========================================\n" << std::endl;

    std::cout << std::left << std::setw(25) << "Test"
              << std::setw(15) << "Moteur 1"
              << std::setw(15) << "Moteur 2" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    std::cout << std::setw(25) << "GPIO Direction"
              << std::setw(15) << (motor1.gpio_ok ? "✓ OK" : "✗ ERREUR")
              << std::setw(15) << (motor2.gpio_ok ? "✓ OK" : "✗ ERREUR") << std::endl;

    std::cout << std::setw(25) << "PWM"
              << std::setw(15) << (motor1.pwm_ok ? "✓ OK" : "✗ ERREUR")
              << std::setw(15) << (motor2.pwm_ok ? "✓ OK" : "✗ ERREUR") << std::endl;

    std::cout << std::setw(25) << "Moteur tourne"
              << std::setw(15) << (motor1.motor_spins ? "✓ OUI" : "✗ NON")
              << std::setw(15) << (motor2.motor_spins ? "✓ OUI" : "✗ NON") << std::endl;

    std::cout << std::setw(25) << "Encodeur A (changes)"
              << std::setw(15) << std::to_string(motor1.encoder_a_changes)
              << std::setw(15) << std::to_string(motor2.encoder_a_changes) << std::endl;

    std::cout << std::setw(25) << "Encodeur B (changes)"
              << std::setw(15) << std::to_string(motor1.encoder_b_changes)
              << std::setw(15) << std::to_string(motor2.encoder_b_changes) << std::endl;

    std::cout << std::setw(25) << "Encodeur fonctionne"
              << std::setw(15) << (motor1.encoder_ok ? "✓ OUI" : "✗ NON")
              << std::setw(15) << (motor2.encoder_ok ? "✓ OUI" : "✗ NON") << std::endl;

    std::cout << std::string(55, '-') << std::endl;

    // Analyse globale
    std::cout << "\n=== ANALYSE GLOBALE ===" << std::endl;

    bool motor1_ok = motor1.gpio_ok && motor1.pwm_ok && motor1.motor_spins && motor1.encoder_ok;
    bool motor2_ok = motor2.gpio_ok && motor2.pwm_ok && motor2.motor_spins && motor2.encoder_ok;

    if (motor1_ok && motor2_ok) {
        std::cout << "✓✓✓ EXCELLENT! Les deux moteurs et encodeurs fonctionnent parfaitement!" << std::endl;
    } else if (motor1_ok && !motor2_ok) {
        std::cout << "⚠️  Moteur 1: OK | Moteur 2: PROBLÈME" << std::endl;
        std::cout << "→ Vérifiez le câblage du Moteur 2" << std::endl;
    } else if (!motor1_ok && motor2_ok) {
        std::cout << "⚠️  Moteur 1: PROBLÈME | Moteur 2: OK" << std::endl;
        std::cout << "→ Vérifiez le câblage du Moteur 1" << std::endl;
    } else {
        std::cout << "❌ Les deux moteurs ont des problèmes!" << std::endl;
        std::cout << "→ Vérifiez l'alimentation (VM) et le câblage général" << std::endl;
    }

    // Problèmes spécifiques d'encodeurs
    if (motor1.motor_spins && !motor1.encoder_ok) {
        std::cout << "\n⚠️  Moteur 1 tourne mais encodeur ne fonctionne pas" << std::endl;
        std::cout << "   Câblage encodeur 1: S1A→GPIO23, S1B→GPIO24" << std::endl;
    }

    if (motor2.motor_spins && !motor2.encoder_ok) {
        std::cout << "\n⚠️  Moteur 2 tourne mais encodeur ne fonctionne pas" << std::endl;
        std::cout << "   Câblage encodeur 2: S2A→GPIO17, S2B→GPIO27" << std::endl;
    }

    std::cout << "\n========================================" << std::endl;
}

int main() {
    signal(SIGINT, signalHandler);
    setup();

    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║  DIAGNOSTIC COMPLET - DEUX MOTEURS     ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    // Test Moteur 1
    MotorTestResult motor1_result = testMotor(
        1, DIR1, EN1, S1A, S1B, "MOTEUR 1"
    );

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Moteur 2
    MotorTestResult motor2_result = testMotor(
        2, DIR2, EN2, S2A, S2B, "MOTEUR 2"
    );

    // Rapport final
    printFinalReport(motor1_result, motor2_result);

    // Arrêt propre
    std::cout << "\nArrêt propre..." << std::endl;
    stopAll();

    // Désactiver le H-Bridge
    gpio_write(pi, H_BRIDGE_POWER, 0);
    gpio_write(pi, ENABLE_MOTOR1, 0);
    gpio_write(pi, ENABLE_MOTOR2, 0);
    std::cout << "H-Bridge désactivé" << std::endl;

    pigpio_stop(pi);
    return 0;
}
