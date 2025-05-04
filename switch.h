#ifndef SWITCH_H
#define SWITCH_H

#include "mbed.h"

// Create two button inputs on PC_10 and PC_11 pins
extern DigitalIn button1;
extern DigitalIn button2;

// Return 1 if pressed, 0 if not pressed, with simple internal debounce delay
int getButton1();
int getButton2();

#endif // SWITCH_H
