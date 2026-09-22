#include <pigpiod_if2.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
#include <cmath>
#include <csignal>
#include <vector>

// ROS2 includes
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>

// Configuration pins
const int DIR1 = 5, EN1 = 12;
const int DIR2 = 6, EN2 = 13;
const int S1A = 23, S1B = 24;
const int S2A = 17, S2B = 27;
const int SERVO = 19;

const int PWM_FREQ = 100000;

int pi;
std::atomic<bool> running{true};

// Variables encodeur
std::atomic<int> encoder1_count{0};
std::atomic<int> encoder2_count{0};
std::atomic<int> lev1A{0}, lev1B{0};
std::atomic<int> lev2A{0}, lev2B{0};
std::atomic<int> oldState1{0};
std::atomic<int> oldState2{0};

// Inversion encodeur moteur 2 (il compte à l'envers)
const int ENCODER2_INVERT = -1;  // Change à 1 si mauvais sens

static int transits[16] = {
    0,  -1,   1,   0,   1,   0,   0,  -1,
   -1,   0,   0,   1,   0,   1,  -1,   0
};

// PID avec meilleurs gains
struct PIDController {
    float Kp = 0.1f;      // Augmenté pour réduire erreur statique
    float Ki = 0.0f;      // Ajouté pour éliminer erreur statique
    float Kd = 0.0f;      // Ajouté pour stabilité
    
    float error = 0;
    float raw_error = 0;       // Erreur brute avant deadzone (pour détection oscillation)
    float last_error = 0;
    float integral = 0;
    float derivative = 0;
    
    float integral_max = 100.0f;
    float output_max = 200.0f;
    float deadband_pwm = 15.0f;
    float error_deadzone = 5.0f;
    
    float compute(float setpoint, float measured, float dt) {
        raw_error = setpoint - measured;  // Stocker erreur brute AVANT deadzone
        error = raw_error;                // Copie pour traitement

        if (std::abs(error) < error_deadzone) {
            error = 0;  // Deadzone appliquée seulement à 'error', pas 'raw_error'
        }
        
        integral += error * dt;
        if (integral > integral_max) integral = integral_max;
        if (integral < -integral_max) integral = -integral_max;
        
        derivative = (error - last_error) / dt;
        
        float output = Kp * error + Ki * integral + Kd * derivative;
        
        if (output > output_max) output = output_max;
        if (output < -output_max) output = -output_max;
        
        // Compensation zone morte
        if (output > 0) {
            output = output + deadband_pwm;
            if (output > 255) output = 255;
        } else if (output < 0) {
            output = output - deadband_pwm;
            if (output < -255) output = -255;
        }
        
        last_error = error;
        return output;
    }
    
    void reset() {
        error = 0;
        last_error = 0;
        integral = 0;
        derivative = 0;
    }
    
    void setDeadband(float pwm) {
        deadband_pwm = pwm;
    }
    
    void setGains(float kp, float ki, float kd) {
        Kp = kp;
        Ki = ki;
        Kd = kd;
        std::cout << "Gains PID: Kp=" << Kp << " Ki=" << Ki << " Kd=" << Kd << std::endl;
    }

    // Déclarations des méthodes d'auto-tuning (implémentations après setMotorPWM)
    void calculateZieglerNicholsGains(float Ku, float Tu);
    bool autoTune(int motor_num, float target_velocity, float max_duration_sec,
                  float& Ku_out, float& Tu_out);
};

PIDController pid_motor1;
PIDController pid_motor2;

void signalHandler(int signum) {
    std::cout << "\n\nCtrl+C - Arrêt..." << std::endl;
    running = false;
}

void encoder1_callback_A(int pi, unsigned gpio, unsigned level, uint32_t tick) {
    lev1A = level;
    int newState = (lev1A << 1) | lev1B;
    int inc = transits[(oldState1 << 2) | newState];
    if (inc) {
        oldState1 = newState;
        encoder1_count += inc;
    }
}

void encoder1_callback_B(int pi, unsigned gpio, unsigned level, uint32_t tick) {
    lev1B = level;
    int newState = (lev1A << 1) | lev1B;
    int inc = transits[(oldState1 << 2) | newState];
    if (inc) {
        oldState1 = newState;
        encoder1_count += inc;
    }
}

void encoder2_callback_A(int pi, unsigned gpio, unsigned level, uint32_t tick) {
    lev2A = level;
    int newState = (lev2A << 1) | lev2B;
    int inc = transits[(oldState2 << 2) | newState];
    if (inc) {
        oldState2 = newState;
        encoder2_count += inc * ENCODER2_INVERT;  // Inversion si nécessaire
    }
}

void encoder2_callback_B(int pi, unsigned gpio, unsigned level, uint32_t tick) {
    lev2B = level;
    int newState = (lev2A << 1) | lev2B;
    int inc = transits[(oldState2 << 2) | newState];
    if (inc) {
        oldState2 = newState;
        encoder2_count += inc * ENCODER2_INVERT;
    }
}

void setup() {
    pi = pigpio_start(NULL, NULL);
    if (pi < 0) {
        std::cerr << "Erreur pigpio" << std::endl;
        exit(1);
    }
    
    set_mode(pi, DIR1, PI_OUTPUT);
    set_mode(pi, DIR2, PI_OUTPUT);
    hardware_PWM(pi, EN1, PWM_FREQ, 0);
    hardware_PWM(pi, EN2, PWM_FREQ, 0);
    
    set_mode(pi, S1A, PI_INPUT);
    set_mode(pi, S1B, PI_INPUT);
    set_mode(pi, S2A, PI_INPUT);
    set_mode(pi, S2B, PI_INPUT);
    
    set_pull_up_down(pi, S1A, PI_PUD_UP);
    set_pull_up_down(pi, S1B, PI_PUD_UP);
    set_pull_up_down(pi, S2A, PI_PUD_UP);
    set_pull_up_down(pi, S2B, PI_PUD_UP);
    
    set_glitch_filter(pi, S1A, 1000);
    set_glitch_filter(pi, S1B, 1000);
    set_glitch_filter(pi, S2A, 1000);
    set_glitch_filter(pi, S2B, 1000);
    
    lev1A = gpio_read(pi, S1A);
    lev1B = gpio_read(pi, S1B);
    lev2A = gpio_read(pi, S2A);
    lev2B = gpio_read(pi, S2B);
    oldState1 = (lev1A << 1) | lev1B;
    oldState2 = (lev2A << 1) | lev2B;
    
    callback(pi, S1A, EITHER_EDGE, encoder1_callback_A);
    callback(pi, S1B, EITHER_EDGE, encoder1_callback_B);
    callback(pi, S2A, EITHER_EDGE, encoder2_callback_A);
    callback(pi, S2B, EITHER_EDGE, encoder2_callback_B);
    
    set_mode(pi, SERVO, PI_OUTPUT);
    
    std::cout << "Initialisation OK!" << std::endl;
    std::cout << "PWM: " << PWM_FREQ << " Hz" << std::endl;
    std::cout << "Encodeur 2 inversion: " << (ENCODER2_INVERT == -1 ? "OUI" : "NON") << std::endl;
}

void setMotorPWM(int motor, float pwm_value) {
    if (pwm_value > 255) pwm_value = 255;
    if (pwm_value < -255) pwm_value = -255;
    
    int duty_cycle = (std::abs(pwm_value) * 1000000) / 255;
    
    if (motor == 1) {
        if (pwm_value >= 0) {
            gpio_write(pi, DIR1, 0);
            hardware_PWM(pi, EN1, PWM_FREQ, duty_cycle);
        } else {
            gpio_write(pi, DIR1, 1);
            hardware_PWM(pi, EN1, PWM_FREQ, duty_cycle);
        }
    } else if (motor == 2) {
        if (pwm_value >= 0) {
            gpio_write(pi, DIR2, 0);
            hardware_PWM(pi, EN2, PWM_FREQ, duty_cycle);
        } else {
            gpio_write(pi, DIR2, 1);
            hardware_PWM(pi, EN2, PWM_FREQ, duty_cycle);
        }
    }
}

void stopAll() {
    hardware_PWM(pi, EN1, PWM_FREQ, 0);
    hardware_PWM(pi, EN2, PWM_FREQ, 0);
}

// Classes utilitaires pour le contrôle et l'auto-tuning

class ExponentialFilter {
private:
    float alpha = 0.2f;
    float filtered_value = 0;
    bool initialized = false;

public:
    float update(float new_value) {
        if (!initialized) {
            filtered_value = new_value;
            initialized = true;
        } else {
            filtered_value = alpha * new_value + (1.0f - alpha) * filtered_value;
        }
        return filtered_value;
    }

    void reset() {
        filtered_value = 0;
        initialized = false;
    }
};

// Détecteur d'oscillations pour auto-tuning Ziegler-Nichols
// Détecte les pics et vallées (extrema locaux) au lieu des passages par zéro
// Permet de détecter les oscillations même avec un offset DC non nul
struct OscillationDetector {
    // Configuration
    int min_cycles_required = 4;
    float period_tolerance = 0.30f;         // Tolérance de 30% sur la période
    float min_extremum_amplitude = 20.0f;   // Amplitude minimale pour détecter un extremum (ticks)
    float min_period = 0.2f;                // Période minimale acceptée (secondes) - évite bruit HF

    // État
    std::vector<float> extremum_times;    // Temps des pics/vallées
    std::vector<float> extremum_values;   // Valeurs des pics/vallées
    float last_error = 0;
    float second_last_error = 0;          // Pour détecter tendance (besoin de 3 points)
    float filtered_error = 0;             // Erreur filtrée (pour réduire bruit)
    float time_accumulator = 0;
    bool last_was_peak = false;           // Pour alterner pics/vallées
    bool has_extremum = false;            // Pour initialisation

    bool detectOscillation(float error, float dt, float& Tu_out) {
        time_accumulator += dt;

        // Besoin de 3 points pour détecter un extremum local
        if (!has_extremum && second_last_error != 0) {
            // Détecte pic (maximum local): erreur monte puis descend
            if (second_last_error < last_error && last_error > error) {
                extremum_times.push_back(time_accumulator - dt);
                extremum_values.push_back(last_error);
                last_was_peak = true;
                has_extremum = true;
            }
            // Détecte vallée (minimum local): erreur descend puis monte
            else if (second_last_error > last_error && last_error < error) {
                extremum_times.push_back(time_accumulator - dt);
                extremum_values.push_back(last_error);
                last_was_peak = false;
                has_extremum = true;
            }
        }
        // Détecte extrema suivants (en alternance pic/vallée)
        else if (has_extremum) {
            bool is_peak = (second_last_error < last_error && last_error > error);
            bool is_valley = (second_last_error > last_error && last_error < error);

            // Accepte pic après vallée, ou vallée après pic
            if ((is_peak && !last_was_peak) || (is_valley && last_was_peak)) {
                // Vérifie l'amplitude par rapport au dernier extremum
                float amplitude = std::abs(last_error - extremum_values.back());

                // Filtre: rejette les extrema avec amplitude trop faible (bruit)
                if (amplitude >= min_extremum_amplitude) {
                    extremum_times.push_back(time_accumulator - dt);
                    extremum_values.push_back(last_error);
                    last_was_peak = is_peak;

                    // Besoin d'au moins 9 extrema pour 4 cycles complets
                    // 1 cycle = pic → vallée → pic (3 extrema)
                    // 4 cycles = 9 extrema
                    if (extremum_times.size() >= 9) {
                        if (isOscillationSustained(Tu_out)) {
                            return true;
                        }
                    }
                }
            }
        }

        // Mise à jour historique
        second_last_error = last_error;
        last_error = error;
        return false;
    }

    bool isOscillationSustained(float& Tu_out) {
        // Calcule les périodes complètes entre extrema de même type
        // (pic à pic ou vallée à vallée)
        std::vector<float> periods;
        for (size_t i = 2; i < extremum_times.size(); i += 2) {
            periods.push_back(extremum_times[i] - extremum_times[i-2]);
        }

        if (periods.size() < 4) return false;

        // Prend les 4 dernières périodes
        std::vector<float> recent_periods(
            periods.end() - 4,
            periods.end()
        );

        // Calcule moyenne et vérifie cohérence
        float mean_period = 0;
        for (float p : recent_periods) {
            mean_period += p;
        }
        mean_period /= recent_periods.size();

        // Filtre 1: Rejette si période moyenne trop courte (bruit HF)
        if (mean_period < min_period) {
            return false;  // Période trop courte = bruit haute fréquence
        }

        // Filtre 2: Vérifie que toutes les périodes sont cohérentes
        for (float p : recent_periods) {
            if (std::abs(p - mean_period) > mean_period * period_tolerance) {
                return false;  // Trop de variation
            }
        }

        Tu_out = mean_period;
        return true;
    }

    void reset() {
        extremum_times.clear();
        extremum_values.clear();
        last_error = 0;
        second_last_error = 0;
        time_accumulator = 0;
        last_was_peak = false;
        has_extremum = false;
    }
};

// Implémentation des méthodes d'auto-tuning PID

void PIDController::calculateZieglerNicholsGains(float Ku, float Tu) {
    // Formules Ziegler-Nichols classiques pour PID
    float new_Kp = 0.6f * Ku;
    float new_Ki = 1.2f * Ku / Tu;
    float new_Kd = 0.075f * Ku * Tu;

    std::cout << "\n=== Gains Ziegler-Nichols Calculés ===" << std::endl;
    std::cout << "Kp = 0.6 * " << Ku << " = " << new_Kp << std::endl;
    std::cout << "Ki = 1.2 * " << Ku << " / " << Tu << " = " << new_Ki << std::endl;
    std::cout << "Kd = 0.075 * " << Ku << " * " << Tu << " = " << new_Kd << std::endl;

    // Validation des gains (affiche avertissements si hors plages)
    if (new_Kp < 0 || new_Kp > 5.0f) {
        std::cerr << "AVERTISSEMENT: Kp hors plage attendue!" << std::endl;
    }
    if (new_Ki < 0 || new_Ki > 20.0f) {
        std::cerr << "AVERTISSEMENT: Ki hors plage attendue!" << std::endl;
    }
    if (new_Kd < 0 || new_Kd > 1.0f) {
        std::cerr << "AVERTISSEMENT: Kd hors plage attendue!" << std::endl;
    }

    setGains(new_Kp, new_Ki, new_Kd);
}

bool PIDController::autoTune(int motor_num, float target_velocity, float max_duration_sec,
                              float& Ku_out, float& Tu_out) {
    using namespace std::chrono;

    // Paramètres de sécurité
    const float Kp_start = 0.05f;
    const float Kp_increment = 0.05f;
    const float Kp_max = 2.0f;
    const float test_duration_per_Kp = 8.0f;  // Augmenté de 8s à 12s pour plus de temps
    const float settling_time = 2.0f;

    // Sauvegarde des gains actuels
    float original_Kp = Kp;
    float original_Ki = Ki;
    float original_Kd = Kd;

    // Initialisation pour contrôle proportionnel pur
    Ki = 0.0f;
    Kd = 0.0f;
    Kp = Kp_start;
    reset();

    std::cout << "\n=== AUTO-TUNING MOTEUR " << motor_num << " ===" << std::endl;
    std::cout << "Vitesse cible: " << target_velocity << " ticks/sec" << std::endl;
    std::cout << "Balayage Kp de " << Kp_start << " à " << Kp_max << std::endl;

    auto start_time = steady_clock::now();

    // Boucle de balayage Kp
    while (Kp <= Kp_max) {
        std::cout << "\nTest Kp = " << Kp << "..." << std::endl;

        OscillationDetector detector;
        reset();

        auto test_start = steady_clock::now();
        auto last_time = test_start;

        int last_count = (motor_num == 1) ? encoder1_count.load() : encoder2_count.load();
        ExponentialFilter filter;

        bool oscillation_found = false;
        float Tu_detected = 0;

        // Boucle de contrôle pour cette valeur de Kp
        while (running) {
            auto current_time = steady_clock::now();
            float elapsed = duration_cast<milliseconds>(
                current_time - test_start).count() / 1000.0f;

            // Vérification timeout
            if (elapsed > test_duration_per_Kp) {
                std::cout << "  Pas d'oscillation détectée à Kp=" << Kp << std::endl;
                break;
            }

            // Calcul dt
            float dt = duration_cast<milliseconds>(
                current_time - last_time).count() / 1000.0f;
            last_time = current_time;

            if (dt < 0.001f || dt > 1.0f) {
                std::this_thread::sleep_for(milliseconds(10));
                continue;
            }

            // Mesure de la vitesse
            int current_count = (motor_num == 1) ?
                encoder1_count.load() : encoder2_count.load();
            float velocity_raw = (current_count - last_count) / dt;
            last_count = current_count;
            float velocity = filter.update(velocity_raw);

            // Calcul sortie PID
            float pwm = compute(target_velocity, velocity, dt);
            setMotorPWM(motor_num, pwm);

            // Détection d'oscillation (après stabilisation)
            if (elapsed > settling_time) {
                if (detector.detectOscillation(raw_error, dt, Tu_detected)) {  // Utilise raw_error SANS deadzone
                    oscillation_found = true;
                    Ku_out = Kp;
                    Tu_out = Tu_detected;

                    std::cout << "  OSCILLATION DÉTECTÉE!" << std::endl;
                    std::cout << "  Gain Ultime (Ku) = " << Ku_out << std::endl;
                    std::cout << "  Période Oscillation (Tu) = " << Tu_out << " sec" << std::endl;
                    break;
                }
            }

            std::this_thread::sleep_for(milliseconds(20));
        }

        // Arrêt moteur
        setMotorPWM(motor_num, 0);
        std::this_thread::sleep_for(milliseconds(500));

        if (oscillation_found) {
            // Succès! Calcul des gains PID
            calculateZieglerNicholsGains(Ku_out, Tu_out);
            return true;
        }

        // Si Ctrl+C pressé, abandonner
        if (!running) {
            std::cerr << "Auto-tuning interrompu par l'utilisateur" << std::endl;
            Kp = original_Kp;
            Ki = original_Ki;
            Kd = original_Kd;
            return false;
        }

        // Incrémenter Kp et réessayer
        Kp += Kp_increment;

        // Vérification timeout global
        float total_elapsed = duration_cast<seconds>(
            steady_clock::now() - start_time).count();
        if (total_elapsed > max_duration_sec) {
            std::cerr << "Timeout auto-tuning! Oscillation non trouvée." << std::endl;
            Kp = original_Kp;
            Ki = original_Ki;
            Kd = original_Kd;
            return false;
        }
    }

    std::cerr << "Auto-tuning échoué: Kp dépasse limite sécurité sans oscillation" << std::endl;
    Kp = original_Kp;
    Ki = original_Ki;
    Kd = original_Kd;
    return false;
}

void measureDeadband() {
    std::cout << "\n=== MESURE ZONE MORTE ===" << std::endl;
    
    std::cout << "Moteur 1..." << std::endl;
    encoder1_count = 0;
    
    for (int pwm = 0; pwm <= 100; pwm += 5) {
        hardware_PWM(pi, EN1, PWM_FREQ, (pwm * 1000000) / 255);
        gpio_write(pi, DIR1, 0);
        
        int start_count = encoder1_count;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        int end_count = encoder1_count;
        
        if (std::abs(end_count - start_count) > 5) {
            std::cout << "Zone morte M1: " << pwm << " PWM" << std::endl;
            hardware_PWM(pi, EN1, PWM_FREQ, 0);
            pid_motor1.setDeadband(pwm);
            break;
        }
    }
    
    hardware_PWM(pi, EN1, PWM_FREQ, 0);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "Moteur 2..." << std::endl;
    encoder2_count = 0;
    
    for (int pwm = 0; pwm <= 100; pwm += 5) {
        hardware_PWM(pi, EN2, PWM_FREQ, (pwm * 1000000) / 255);
        gpio_write(pi, DIR2, 0);
        
        int start_count = encoder2_count;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        int end_count = encoder2_count;
        
        if (std::abs(end_count - start_count) > 5) {
            std::cout << "Zone morte M2: " << pwm << " PWM" << std::endl;
            hardware_PWM(pi, EN2, PWM_FREQ, 0);
            pid_motor2.setDeadband(pwm);
            break;
        }
    }
    
    hardware_PWM(pi, EN2, PWM_FREQ, 0);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void velocityControl(float target_velocity_motor1, float target_velocity_motor2, float duration_sec) {
    using namespace std::chrono;
    
    auto start_time = steady_clock::now();
    auto last_time = start_time;
    
    int last_count1 = encoder1_count;
    int last_count2 = encoder2_count;
    
    ExponentialFilter filter1, filter2;
    
    std::cout << "\n=== Asservissement vitesse ===" << std::endl;
    std::cout << "Cible M1: " << target_velocity_motor1 << " ticks/sec | "
              << "Cible M2: " << target_velocity_motor2 << " ticks/sec" << std::endl;
    std::cout << "Ctrl+C pour arrêter\n" << std::endl;
    
    float ramp_duration = 2.0f;
    
    while (running) {
        auto current_time = steady_clock::now();
        float elapsed = duration_cast<milliseconds>(current_time - start_time).count() / 1000.0f;
        
        if (elapsed >= duration_sec) break;
        
        float dt = duration_cast<milliseconds>(current_time - last_time).count() / 1000.0f;
        last_time = current_time;
        
        if (dt < 0.001f || dt > 1.0f) {
            std::this_thread::sleep_for(milliseconds(10));
            continue;
        }
        
        float ramp_factor = (elapsed < ramp_duration) ? (elapsed / ramp_duration) : 1.0f;
        
        float target1 = target_velocity_motor1 * ramp_factor;
        float target2 = target_velocity_motor2 * ramp_factor;
        
        int current_count1 = encoder1_count;
        int current_count2 = encoder2_count;
        
        float velocity1_raw = (current_count1 - last_count1) / dt;
        float velocity2_raw = (current_count2 - last_count2) / dt;
        
        last_count1 = current_count1;
        last_count2 = current_count2;
        
        float velocity1 = filter1.update(velocity1_raw);
        float velocity2 = filter2.update(velocity2_raw);
        
        float pwm1 = pid_motor1.compute(target1, velocity1, dt);
        float pwm2 = pid_motor2.compute(target2, velocity2, dt);
        
        setMotorPWM(1, pwm1);
        setMotorPWM(2, pwm2);
        
        static auto last_print = steady_clock::now();
        if (duration_cast<milliseconds>(current_time - last_print).count() > 300) {
            std::cout << std::fixed << std::setprecision(1);
            std::cout << "t=" << std::setw(4) << elapsed << "s | "
                      << "M1: " << std::setw(6) << target1 
                      << "→" << std::setw(7) << velocity1 
                      << " (err=" << std::setw(6) << pid_motor1.error
                      << " int=" << std::setw(6) << pid_motor1.integral
                      << " pwm=" << std::setw(6) << pwm1 << ") | "
                      << "M2: " << std::setw(6) << target2 
                      << "→" << std::setw(7) << velocity2 
                      << " (err=" << std::setw(6) << pid_motor2.error
                      << " pwm=" << std::setw(6) << pwm2 << ")" << std::endl;
            last_print = current_time;
        }
        
        std::this_thread::sleep_for(milliseconds(20));
    }
}

int main() {
    signal(SIGINT, signalHandler);
    setup();
    
    measureDeadband();
    
    if (!running) { stopAll(); pigpio_stop(pi); return 0; }
    
    // Calibration
    std::cout << "\n=== Calibration ===" << std::endl;
    encoder1_count = 0;
    encoder2_count = 0;
    
    hardware_PWM(pi, EN1, PWM_FREQ, 700000);
    gpio_write(pi, DIR1, 0);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    hardware_PWM(pi, EN1, PWM_FREQ, 0);
    
    float vel_m1 = encoder1_count / 3.0f;
    std::cout << "M1: " << encoder1_count << " ticks → " << vel_m1 << " ticks/sec" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    encoder2_count = 0;
    
    hardware_PWM(pi, EN2, PWM_FREQ, 700000);
    gpio_write(pi, DIR2, 0);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    hardware_PWM(pi, EN2, PWM_FREQ, 0);
    
    float vel_m2 = encoder2_count / 3.0f;
    std::cout << "M2: " << encoder2_count << " ticks → " << vel_m2 << " ticks/sec" << std::endl;
    
    float test_velocity = (std::abs(vel_m1) + std::abs(vel_m2)) / 4.0f;
    std::cout << "\nVitesse de test: " << test_velocity << " ticks/sec" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Auto-tuning PID
    std::cout << "\n=== AUTO-TUNING PID CONTROLLERS ===" << std::endl;

    // Tuning Moteur 1
    float Ku1, Tu1;
    encoder1_count = 0;
    if (pid_motor1.autoTune(1, test_velocity, 120.0f, Ku1, Tu1)) {
        std::cout << "\nMoteur 1: Tuning réussi!\n" << std::endl;
    } else {
        std::cout << "\nMoteur 1: Échec - gains par défaut utilisés\n" << std::endl;
        pid_motor1.setGains(0.1f, 0.0f, 0.0f);
    }

    if (!running) { stopAll(); pigpio_stop(pi); return 0; }
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Tuning Moteur 2
    float Ku2, Tu2;
    encoder2_count = 0;
    if (pid_motor2.autoTune(2, test_velocity, 120.0f, Ku2, Tu2)) {
        std::cout << "\nMoteur 2: Tuning réussi!\n" << std::endl;
    } else {
        std::cout << "\nMoteur 2: Échec - gains par défaut utilisés\n" << std::endl;
        pid_motor2.setGains(0.1f, 0.0f, 0.0f);
    }

    if (!running) { stopAll(); pigpio_stop(pi); return 0; }
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Tests
    encoder1_count = 0;
    encoder2_count = 0;
    pid_motor1.reset();
    pid_motor2.reset();
    
    std::cout << "\n=== Test 1: " << test_velocity << " ticks/sec (10s) ===" << std::endl;
    velocityControl(test_velocity, test_velocity, 10.0f);
    stopAll();
    
    std::cout << "\n=== Tests terminés ===" << std::endl;
    std::cout << "Position M1: " << encoder1_count << " ticks" << std::endl;
    std::cout << "Position M2: " << encoder2_count << " ticks" << std::endl;
    
    pigpio_stop(pi);
    return 0;
}