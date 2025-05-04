#include "buzzer.h"

Buzzer::Buzzer(PinName pin)
    : _buzzer(pin)
{
    // Turn off buzzer initially
    _buzzer = 0.0f;
}

void Buzzer::play_note(int frequency, int duration_ms) {
    if (frequency <= 0 || duration_ms <= 0) {
        return;
    }
    // Set period: T = 1/f, converted to microseconds
    _buzzer.period_us(1000000 / frequency);
    // 50% duty cycle
    _buzzer.pulsewidth_us(_buzzer.read_period_us() / 2);
    // Play for specified duration
    ThisThread::sleep_for(chrono::milliseconds(duration_ms));
    // Turn off after playing
    _buzzer = 0.0f;
}

void Buzzer::stop() {
    _buzzer = 0.0f;
}




/* Example usage
#include "buzzer.h"
int main() {
    Buzzer buzzer;          // Uses PA_15 by default
    while (true) {
        // Play A4 (440Hz) for 200ms
        buzzer.play_note(440);
        ThisThread::sleep_for(500ms);

        // Play C5 (523Hz) for 300ms
        buzzer.play_note(523, 300);
        ThisThread::sleep_for(500ms);

        // Or stop directly
        buzzer.stop();
        ThisThread::sleep_for(1s);
    }
}
*/