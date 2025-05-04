#ifndef LED_H
#define LED_H

#include "mbed.h"

/**
 * @brief LED indicator used to display remaining lives
 */
class LifeIndicator {
public:
    /** Constructor: uses PA_1, PA_4, PB_0 by default */
    LifeIndicator(PinName leftPin = PA_1,
                  PinName midPin  = PA_4,
                  PinName rightPin= PB_0);

    /** Initialization: turn off all LEDs */
    void init();

    /**
     * @brief Set LED states based on remaining lives
     * @param lives Number of lives (0–3)
     */
    void setLives(int lives);

private:
    DigitalOut _ledLeft;   // PA_1
    DigitalOut _ledMid;    // PA_4
    DigitalOut _ledRight;  // PB_0
};

#endif // LED_H
