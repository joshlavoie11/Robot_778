#include <pigpiod_if2.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <iomanip>

// GPIO pins (PmodDHB1 - configuration actuelle)
const int DIR1 = 5, EN1 = 12;   // EN1 = GPIO 12 (Hardware PWM0)
const int DIR2 = 6, EN2 = 13;   // EN2 = GPIO 13 (Hardware PWM1)
const int S1A = 23, S1B = 24;
const int S2A = 17, S2B = 27;

// H-Bridge enable pins (CRITIQUES pour faire fonctionner le PmodDHB1)
const int H_BRIDGE_POWER = 26;  // Alimente le H-Drive
const int ENABLE_MOTOR1 = 16;   // Enable Moteur 1
const int ENABLE_MOTOR2 = 22;   // Enable Moteur 2

// PWM frequencies to test
const int PWM_FREQ_LOW = 1000;      // 1 kHz
const int PWM_FREQ_HIGH = 100000;   // 100 kHz

int pi;
bool running = true;

struct MotorTestResult {
    bool gpio_ok;
    bool pwm_ok;
    bool motor_spins;
    int encoder_a_changes;
    int encoder_b_changes;
    bool encoder_ok;
    int pwm_frequency_used;
};

void signalHandler(int signum) {
    std::cout << "\nCtrl+C - Arrêt..." << std::endl;
    running = false;
}

void stopAll() {
    // Hardware PWM: dutycycle=0 pour arrêter
    hardware_PWM(pi, EN1, 0, 0);
    hardware_PWM(pi, EN2, 0, 0);
}

void setup() {
    pi = pigpio_start(NULL, NULL);
    if (pi < 0) {
        std::cerr << "Erreur pigpio" << std::endl;
        exit(1);
    }

    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║  TEST HARDWARE PWM - DEUX MOTEURS      ║" << std::endl;
    std::cout << "║  GPIO 12 & 13 (Hardware PWM0/PWM1)     ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝\n" << std::endl;

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

    // Pas besoin de set_mode pour EN1/EN2 - hardware_PWM le fait automatiquement
    std::cout << "Hardware PWM sera utilisé sur GPIO 12 et GPIO 13" << std::endl;

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

// Convertir duty cycle de 0-255 (PWM classique) vers 0-1000000 (hardware PWM)
int convertDutyCycle(int pwm_0_255) {
    // 0-255 -> 0-1000000
    // dutycycle = (pwm_0_255 * 1000000) / 255
    return (pwm_0_255 * 1000000) / 255;
}

MotorTestResult testMotor(int motor_num, int dir_pin, int en_pin,
                          int encoder_a_pin, int encoder_b_pin,
                          const char* motor_name, int pwm_freq) {
    MotorTestResult result = {false, false, false, 0, 0, false, pwm_freq};

    std::cout << "\n========================================" << std::endl;
    std::cout << "   TEST " << motor_name << " @ " << pwm_freq << " Hz" << std::endl;
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

    // 2. Test Hardware PWM
    std::cout << "\n2. Test Hardware PWM " << motor_name << " (GPIO " << en_pin << ")" << std::endl;
    std::cout << "   Fréquence: " << pwm_freq << " Hz" << std::endl;
    std::cout << "   Activation PWM 50% (duty=500000)..." << std::endl;

    // 50% = 500000 sur 1000000
    int pwm_result = hardware_PWM(pi, en_pin, pwm_freq, 500000);
    result.pwm_ok = (pwm_result == 0);
    std::cout << "   Résultat: " << (result.pwm_ok ? "✓ OK" : "✗ ERREUR") << std::endl;

    std::cout << "\n   Le " << motor_name << " tourne-t-il? (tu dois l'ENTENDRE)" << std::endl;
    std::cout << "   Attends 3 secondes..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    hardware_PWM(pi, en_pin, 0, 0);  // Arrêt

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
    // 60% = 600000 sur 1000000
    hardware_PWM(pi, en_pin, pwm_freq, 600000);

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

    hardware_PWM(pi, en_pin, 0, 0);  // Arrêt

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

void printFinalReport(const MotorTestResult& m1_low, const MotorTestResult& m1_high,
                      const MotorTestResult& m2_low, const MotorTestResult& m2_high) {
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "   RAPPORT FINAL - HARDWARE PWM" << std::endl;
    std::cout << "========================================\n" << std::endl;

    std::cout << std::left << std::setw(25) << "Test"
              << std::setw(15) << "M1 @ 1kHz"
              << std::setw(15) << "M1 @ 100kHz"
              << std::setw(15) << "M2 @ 1kHz"
              << std::setw(15) << "M2 @ 100kHz" << std::endl;
    std::cout << std::string(85, '-') << std::endl;

    std::cout << std::setw(25) << "GPIO Direction"
              << std::setw(15) << (m1_low.gpio_ok ? "✓ OK" : "✗ ERR")
              << std::setw(15) << (m1_high.gpio_ok ? "✓ OK" : "✗ ERR")
              << std::setw(15) << (m2_low.gpio_ok ? "✓ OK" : "✗ ERR")
              << std::setw(15) << (m2_high.gpio_ok ? "✓ OK" : "✗ ERR") << std::endl;

    std::cout << std::setw(25) << "Hardware PWM"
              << std::setw(15) << (m1_low.pwm_ok ? "✓ OK" : "✗ ERR")
              << std::setw(15) << (m1_high.pwm_ok ? "✓ OK" : "✗ ERR")
              << std::setw(15) << (m2_low.pwm_ok ? "✓ OK" : "✗ ERR")
              << std::setw(15) << (m2_high.pwm_ok ? "✓ OK" : "✗ ERR") << std::endl;

    std::cout << std::setw(25) << "Moteur tourne"
              << std::setw(15) << (m1_low.motor_spins ? "✓ OUI" : "✗ NON")
              << std::setw(15) << (m1_high.motor_spins ? "✓ OUI" : "✗ NON")
              << std::setw(15) << (m2_low.motor_spins ? "✓ OUI" : "✗ NON")
              << std::setw(15) << (m2_high.motor_spins ? "✓ OUI" : "✗ NON") << std::endl;

    std::cout << std::setw(25) << "Encodeur A (changes)"
              << std::setw(15) << std::to_string(m1_low.encoder_a_changes)
              << std::setw(15) << std::to_string(m1_high.encoder_a_changes)
              << std::setw(15) << std::to_string(m2_low.encoder_a_changes)
              << std::setw(15) << std::to_string(m2_high.encoder_a_changes) << std::endl;

    std::cout << std::setw(25) << "Encodeur B (changes)"
              << std::setw(15) << std::to_string(m1_low.encoder_b_changes)
              << std::setw(15) << std::to_string(m1_high.encoder_b_changes)
              << std::setw(15) << std::to_string(m2_low.encoder_b_changes)
              << std::setw(15) << std::to_string(m2_high.encoder_b_changes) << std::endl;

    std::cout << std::setw(25) << "Encodeur fonctionne"
              << std::setw(15) << (m1_low.encoder_ok ? "✓ OUI" : "✗ NON")
              << std::setw(15) << (m1_high.encoder_ok ? "✓ OUI" : "✗ NON")
              << std::setw(15) << (m2_low.encoder_ok ? "✓ OUI" : "✗ NON")
              << std::setw(15) << (m2_high.encoder_ok ? "✓ OUI" : "✗ NON") << std::endl;

    std::cout << std::string(85, '-') << std::endl;

    // Analyse
    std::cout << "\n=== ANALYSE HARDWARE PWM ===\n" << std::endl;

    bool m1_low_ok = m1_low.gpio_ok && m1_low.pwm_ok && m1_low.motor_spins && m1_low.encoder_ok;
    bool m1_high_ok = m1_high.gpio_ok && m1_high.pwm_ok && m1_high.motor_spins && m1_high.encoder_ok;
    bool m2_low_ok = m2_low.gpio_ok && m2_low.pwm_ok && m2_low.motor_spins && m2_low.encoder_ok;
    bool m2_high_ok = m2_high.gpio_ok && m2_high.pwm_ok && m2_high.motor_spins && m2_high.encoder_ok;

    if (m1_low_ok && m1_high_ok) {
        std::cout << "✓ Moteur 1: Fonctionne parfaitement aux deux fréquences" << std::endl;
    } else if (m1_low_ok && !m1_high_ok) {
        std::cout << "⚠️  Moteur 1: OK à 1kHz mais problème à 100kHz" << std::endl;
    } else if (!m1_low_ok && m1_high_ok) {
        std::cout << "⚠️  Moteur 1: OK à 100kHz mais problème à 1kHz" << std::endl;
    } else {
        std::cout << "❌ Moteur 1: Problèmes aux deux fréquences" << std::endl;
    }

    if (m2_low_ok && m2_high_ok) {
        std::cout << "✓ Moteur 2: Fonctionne parfaitement aux deux fréquences" << std::endl;
    } else if (m2_low_ok && !m2_high_ok) {
        std::cout << "⚠️  Moteur 2: OK à 1kHz mais problème à 100kHz" << std::endl;
    } else if (!m2_low_ok && m2_high_ok) {
        std::cout << "⚠️  Moteur 2: OK à 100kHz mais problème à 1kHz" << std::endl;
    } else {
        std::cout << "❌ Moteur 2: Problèmes aux deux fréquences" << std::endl;
    }

    std::cout << "\n=== RECOMMANDATIONS ===\n" << std::endl;
    std::cout << "Hardware PWM (GPIO 12, 13) utilise le périphérique PWM matériel." << std::endl;
    std::cout << "Avantages: Plus précis, moins de charge CPU, fréquences plus élevées." << std::endl;

    if (m1_low_ok || m2_low_ok) {
        std::cout << "\n✓ 1 kHz fonctionne bien - fréquence recommandée pour moteurs DC" << std::endl;
    }

    if (m1_high_ok || m2_high_ok) {
        std::cout << "✓ 100 kHz fonctionne - peut réduire le bruit audible" << std::endl;
    }

    std::cout << "\n========================================" << std::endl;
}

int main() {
    signal(SIGINT, signalHandler);
    setup();

    std::cout << "\nCe test va comparer Hardware PWM à deux fréquences:" << std::endl;
    std::cout << "1. 1 kHz (fréquence basse, standard)" << std::endl;
    std::cout << "2. 100 kHz (fréquence haute, config actuelle)\n" << std::endl;

    std::cout << "Appuyez sur ENTRÉE pour commencer...";
    std::cin.ignore();
    std::cin.get();

    // Test Moteur 1 @ 1kHz
    MotorTestResult m1_low = testMotor(1, DIR1, EN1, S1A, S1B, "MOTEUR 1", PWM_FREQ_LOW);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Moteur 1 @ 100kHz
    MotorTestResult m1_high = testMotor(1, DIR1, EN1, S1A, S1B, "MOTEUR 1", PWM_FREQ_HIGH);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Moteur 2 @ 1kHz
    MotorTestResult m2_low = testMotor(2, DIR2, EN2, S2A, S2B, "MOTEUR 2", PWM_FREQ_LOW);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Test Moteur 2 @ 100kHz
    MotorTestResult m2_high = testMotor(2, DIR2, EN2, S2A, S2B, "MOTEUR 2", PWM_FREQ_HIGH);

    // Rapport final
    printFinalReport(m1_low, m1_high, m2_low, m2_high);

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
