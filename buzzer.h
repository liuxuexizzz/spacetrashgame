// buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

#include "mbed.h"

/**
 * @brief PWM buzzer control class
 * 
 * Produces sound by setting PWM period and duty cycle.
 * Default pin is PA_15, but can be changed via constructor to any PWM output pin.
 */
class Buzzer {
public:
    /**
     * @brief Construct a Buzzer instance
     * @param pin PWM pin connected to the buzzer (default PA_15)
     */
    explicit Buzzer(PinName pin = PA_15);

    /**
     * @brief Play a tone of given frequency
     * @param frequency  Tone frequency in Hz
     * @param duration_ms Duration in milliseconds (default 200 ms)
     */
    void play_note(int frequency, int duration_ms = 200);

    /**
     * @brief Stop sound (set duty cycle to 0)
     */
    void stop();

private:
    PwmOut _buzzer;
};

#endif // BUZZER_H
