#include "switch.h"

// Instantiate buttons
DigitalIn button1(PC_10);
DigitalIn button2(PC_11);

// Simple debounce function: if press detected, delay 10ms and confirm again
static int debounce(DigitalIn &btn) {
    if (btn.read()) {
        wait_us(10000);            // Debounce delay 10 ms
        if (btn.read()) {
            return 1;
        }
    }
    return 0;
}

int getButton1() {
    return debounce(button1);
}

int getButton2() {
    return debounce(button2);
}
