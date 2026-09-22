#include <pigpiod_if2.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <csignal>

const int DIR1 = 5, EN1 = 12;
const int DIR2 = 6, EN2 = 13;
const int S1A = 23, S1B = 24;
const int S2A = 17, S2B = 27;

const int PWM_FREQ = 100000;

int pi;
bool running = true;

std::atomic<int> encoder1_count{0};
std::atomic<int> encoder2_count{0};
std::atomic<int> last_encoder1_state{0};
std::atomic<int> last_encoder2_state{0};

std::atomic<int> s1a_interrupts{0};
std::atomic<int> s2a_interrupts{0};

void signalHandler(int signum) {
    running = false;
}

void encoder1_callback(int pi, unsigned user_gpio, unsigned level, uint32_t tick) {
    s1a_interrupts++;
    
    int MSB = level;
    int LSB = gpio_read(pi, S1B);
    
    int encoded = (MSB << 1) | LSB;
    int sum = (last_encoder1_state << 2) | encoded;
    
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        encoder1_count++;
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        encoder1_count--;
    }
    
    last_encoder1_state = encoded;
}

void encoder2_callback(int pi, unsigned user_gpio, unsigned level, uint32_t tick) {
    s2a_interrupts++;
    
    int MSB = level;
    int LSB = gpio_read(pi, S2B);
    
    int encoded = (MSB << 1) | LSB;
    int sum = (last_encoder2_state << 2) | encoded;
    
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        encoder2_count++;
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        encoder2_count--;
    }
    
    last_encoder2_state = encoded;
}

void setup() {
    pi = pigpio_start(NULL, NULL);
    if (pi < 0) {
        std::cerr << "Erreur pigpio" << std::endl;
        exit(1);
    }
    
    set_mode(pi, DIR1, PI_OUTPUT);
    set_mode(pi, DIR2, PI_OUTPUT);
    
    set_mode(pi, S1A, PI_INPUT);
    set_mode(pi, S1B, PI_INPUT);
    set_mode(pi, S2A, PI_INPUT);
    set_mode(pi, S2B, PI_INPUT);
    
    set_pull_up_down(pi, S1A, PI_PUD_UP);
    set_pull_up_down(pi, S1B, PI_PUD_UP);
    set_pull_up_down(pi, S2A, PI_PUD_UP);
    set_pull_up_down(pi, S2B, PI_PUD_UP);
    
    callback(pi, S1A, EITHER_EDGE, encoder1_callback);
    callback(pi, S2A, EITHER_EDGE, encoder2_callback);
}

void stopAll() {
    hardware_PWM(pi, EN1, PWM_FREQ, 0);
    hardware_PWM(pi, EN2, PWM_FREQ, 0);
}

int main() {
    signal(SIGINT, signalHandler);
    setup();
    
    std::cout << "=== DIAGNOSTIC COMPLET ENCODEURS ===" << std::endl;
    
    // Test 1: État statique
    std::cout << "\n1. État statique des encodeurs:" << std::endl;
    std::cout << "   S1A: " << gpio_read(pi, S1A) << std::endl;
    std::cout << "   S1B: " << gpio_read(pi, S1B) << std::endl;
    std::cout << "   S2A: " << gpio_read(pi, S2A) << std::endl;
    std::cout << "   S2B: " << gpio_read(pi, S2B) << std::endl;
    
    // Test 2: Moteur 1 avec monitoring détaillé
    std::cout << "\n2. Test MOTEUR 1 à 80% PWM pendant 5 secondes" << std::endl;
    std::cout << "   Écoute si le moteur tourne...\n" << std::endl;
    
    encoder1_count = 0;
    s1a_interrupts = 0;
    
    gpio_write(pi, DIR1, 0);
    hardware_PWM(pi, EN1, PWM_FREQ, 800000);
    
    auto start = std::chrono::steady_clock::now();
    int last_s1a = gpio_read(pi, S1A);
    int last_s1b = gpio_read(pi, S1B);
    int manual_s1a_changes = 0;
    int manual_s1b_changes = 0;
    
    while (std::chrono::duration_cast<std::chrono::seconds>(
           std::chrono::steady_clock::now() - start).count() < 5) {
        
        int curr_s1a = gpio_read(pi, S1A);
        int curr_s1b = gpio_read(pi, S1B);
        
        if (curr_s1a != last_s1a) {
            manual_s1a_changes++;
            // std::cout << "   [" << std::chrono::duration_cast<std::chrono::milliseconds>(
            //     std::chrono::steady_clock::now() - start).count() 
            //     << "ms] S1A: " << last_s1a << "→" << curr_s1a << std::endl;
            last_s1a = curr_s1a;
        }
        
        if (curr_s1b != last_s1b) {
            manual_s1b_changes++;
            // std::cout << "   [" << std::chrono::duration_cast<std::chrono::milliseconds>(
            //     std::chrono::steady_clock::now() - start).count() 
            //     << "ms] S1B: " << last_s1b << "→" << curr_s1b << std::endl;
            last_s1b = curr_s1b;
        }
        
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    
    hardware_PWM(pi, EN1, PWM_FREQ, 0);
    
    std::cout << "\n   RÉSULTATS MOTEUR 1:" << std::endl;
    std::cout << "   S1A changements (polling):    " << manual_s1a_changes << std::endl;
    std::cout << "   S1B changements (polling):    " << manual_s1b_changes << std::endl;
    std::cout << "   S1A interruptions (callback): " << s1a_interrupts << std::endl;
    std::cout << "   Compteur encodeur (quadrature): " << encoder1_count << std::endl;
    
    if (manual_s1a_changes == 0 && manual_s1b_changes == 0) {
        std::cout << "\n   ❌ PROBLÈME CRITIQUE: Aucun signal encodeur détecté sur M1!" << std::endl;
        std::cout << "\n   Le moteur tourne-t-il? (o/n): ";
        char resp;
        std::cin >> resp;
        
        if (resp == 'o' || resp == 'O') {
            std::cout << "\n   → Le moteur tourne MAIS l'encodeur ne fonctionne PAS" << std::endl;
            std::cout << "\n   Causes possibles:" << std::endl;
            std::cout << "   1. Encodeur moteur 1 PAS CÂBLÉ aux pins S1A/S1B du PmodDHB1" << std::endl;
            std::cout << "   2. Mauvais GPIO: S1A devrait être GPIO 23, S1B devrait être GPIO 24" << std::endl;
            std::cout << "   3. Encodeur non alimenté (VCC 3.3V manquant)" << std::endl;
            std::cout << "   4. Ces moteurs N'ONT PAS d'encodeurs intégrés" << std::endl;
            std::cout << "\n   TON ANCIEN SCRIPT BASH:" << std::endl;
            std::cout << "   - Utilise-t-il vraiment les encodeurs?" << std::endl;
            std::cout << "   - Ou fait-il juste tourner les moteurs en boucle ouverte?" << std::endl;
        } else {
            std::cout << "\n   → Le moteur NE TOURNE PAS - problème de contrôle moteur" << std::endl;
        }
    } else if (manual_s1a_changes > 0 && encoder1_count == 0) {
        std::cout << "\n   ⚠️  Les signaux changent MAIS le compteur quadrature = 0" << std::endl;
        std::cout << "   → Problème dans le décodage quadrature" << std::endl;
    } else {
        std::cout << "\n   ✓ Encodeur moteur 1 fonctionne!" << std::endl;
        float ticks_per_sec = encoder1_count / 5.0f;
        std::cout << "   Vitesse: ~" << ticks_per_sec << " ticks/sec @ 80% PWM" << std::endl;
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Test 3: Moteur 2
    std::cout << "\n3. Test MOTEUR 2 à 80% PWM pendant 5 secondes\n" << std::endl;
    
    encoder2_count = 0;
    s2a_interrupts = 0;
    
    gpio_write(pi, DIR2, 0);
    hardware_PWM(pi, EN2, PWM_FREQ, 800000);
    
    start = std::chrono::steady_clock::now();
    int last_s2a = gpio_read(pi, S2A);
    int last_s2b = gpio_read(pi, S2B);
    int manual_s2a_changes = 0;
    int manual_s2b_changes = 0;
    
    while (std::chrono::duration_cast<std::chrono::seconds>(
           std::chrono::steady_clock::now() - start).count() < 5) {
        
        int curr_s2a = gpio_read(pi, S2A);
        int curr_s2b = gpio_read(pi, S2B);
        
        if (curr_s2a != last_s2a) {
            manual_s2a_changes++;
            // std::cout << "   [" << std::chrono::duration_cast<std::chrono::milliseconds>(
            //     std::chrono::steady_clock::now() - start).count() 
            //     << "ms] S2A: " << last_s2a << "→" << curr_s2a << std::endl;
            last_s2a = curr_s2a;
        }
        
        if (curr_s2b != last_s2b) {
            manual_s2b_changes++;
            // std::cout << "   [" << std::chrono::duration_cast<std::chrono::milliseconds>(
            //     std::chrono::steady_clock::now() - start).count() 
            //     << "ms] S2B: " << last_s2b << "→" << curr_s2b << std::endl;
            last_s2b = curr_s2b;
        }
        
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    
    hardware_PWM(pi, EN2, PWM_FREQ, 0);
    
    std::cout << "\n   RÉSULTATS MOTEUR 2:" << std::endl;
    std::cout << "   S2A changements (polling):    " << manual_s2a_changes << std::endl;
    std::cout << "   S2B changements (polling):    " << manual_s2b_changes << std::endl;
    std::cout << "   S2A interruptions (callback): " << s2a_interrupts << std::endl;
    std::cout << "   Compteur encodeur (quadrature): " << encoder2_count << std::endl;
    
    if (manual_s2a_changes == 0 && manual_s2b_changes == 0) {
        std::cout << "\n   ❌ Encodeur moteur 2 ne fonctionne pas!" << std::endl;
    } else {
        std::cout << "\n   ✓ Encodeur moteur 2 fonctionne!" << std::endl;
        float ticks_per_sec = encoder2_count / 5.0f;
        std::cout << "   Vitesse: ~" << ticks_per_sec << " ticks/sec @ 80% PWM" << std::endl;
    }
    
    std::cout << "\n=== FIN DIAGNOSTIC ===" << std::endl;
    std::cout << "\nQUESTIONS CRITIQUES:" << std::endl;
    std::cout << "1. Tes moteurs ont-ils VRAIMENT des encodeurs intégrés?" << std::endl;
    std::cout << "2. Les câbles encodeurs sont-ils branchés sur le PmodDHB1?" << std::endl;
    std::cout << "3. Ton ancien script bash utilise-t-il les encodeurs ou juste PWM?" << std::endl;
    
    stopAll();
    pigpio_stop(pi);
    return 0;
}