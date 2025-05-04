#include "led.h"

LifeIndicator::LifeIndicator(PinName leftPin,
                             PinName midPin,
                             PinName rightPin)
    : _ledLeft(leftPin), _ledMid(midPin), _ledRight(rightPin)
{
    // Do not automatically change LED states on construction; must call init() manually
}

void LifeIndicator::init() {
    // Ensure all LEDs are off
    _ledLeft  = 0;
    _ledMid   = 0;
    _ledRight = 0;
}

void LifeIndicator::setLives(int lives) {
    // Clamp value to [0,3]
    if (lives < 0) lives = 0;
    if (lives > 3) lives = 3;

    // If lives >= 1, turn on left LED; otherwise off
    _ledLeft  = (lives >= 1);
    // If lives >= 2, turn on middle LED; otherwise off
    _ledMid   = (lives >= 2);
    // If lives >= 3, turn on right LED; otherwise off
    _ledRight = (lives >= 3);
}
