#include "mbed.h"
#include "N5110.h"
#include "Joystick.h"
#include "SpaceTrashEngine.h"
#include "Utils.h"
#include <cstdio>
#include "switch.h"
#include "led.h"
#include "buzzer.h"

// Pin definitions
// Nucleo L476RG + Nokia 5110

N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10); 
Joystick joystick(PC_1, PC_0); 
//DigitalIn buttonA(BUTTON1);



// /*
int main() {
    // Initialize hardware
    lcd.init(LPH7366_1);  lcd.setContrast(0.5f);
    joystick.init();
    // Initialize life LED indicator
    LifeIndicator lifeLed;
    lifeLed.init();
    SpaceTrashEngine engine;
    Buzzer buzzer(PA_15);
    // Inject the buzzer into the game engine
    engine.setBuzzer(&buzzer);
    // Record button debounce state
    bool lastButton = getButton1();

    GameState state = STATE_MENU;

    while (true) {
        bool btn = (getButton1() == 1);
        bool btnPressed = btn && !lastButton;
        lastButton = btn;

        switch (state) {
        case STATE_MENU:
            // Draw start menu and mini animation
            lcd.clear();
            lcd.printString(" SPACE TRASH! ", 0, 1);
            // Simple animation: blinking "PRESS" text
            static bool show = true;
            if (show) lcd.printString(" PRESS BUTTON ", 0, 3);
            show = !show;
            lcd.refresh();
            if (getButton1() == 1) {
                // Button starts the game
                engine.init(84, 48);
                state = STATE_PLAYING;
            }
            ThisThread::sleep_for(200ms);
            break;

        case STATE_PLAYING: {
            // Main game loop
            UserInput input{ joystick.get_direction(), joystick.get_mag() };
            int lives = engine.update(input);
            // Update LED according to remaining lives
            lifeLed.setLives(lives);
            lcd.clear();
            engine.draw(lcd);
            lcd.refresh();
            // If lives are exhausted
            if (lives <= 0) {
                state = STATE_GAMEOVER;
            }
            // Check for pause
            if (getButton1() == 1) {
                state = STATE_PAUSED;
            }
            ThisThread::sleep_for(100ms);
            break;
        }

        case STATE_PAUSED:
            // Pause screen: show score and lives
            lcd.clear();
            {
                // First line: show PAUSED
                lcd.printString("   PAUSED!", 0, 1);

                // Second line: show score
                char buf[20];
                sprintf(buf, "   score:%d", engine.getScore());
                lcd.printString(buf, 0, 3);

                // Third line: show lives
                sprintf(buf, "   lives:%d", engine.getLives());
                lcd.printString(buf, 0, 4);
            }
            lcd.refresh();
            if (getButton1() == 1) {
                state = STATE_PLAYING;
            }
            ThisThread::sleep_for(200ms);
            break;

        case STATE_GAMEOVER:
            // Game over screen: score + missed trash percentage
            lcd.clear();
            {
                int score   = engine.getScore();
                int spawned = engine.getTotalTrashSpawned();
                int missed  = engine.getTotalTrashMissed();
                int pct     = spawned > 0 ? (missed * 100 / spawned) : 0;

                lcd.clear();
                lcd.printString("  GAME OVER", 0, 0);

                // Let snprintf directly return string length
                char info[17];
                int len = snprintf(info, sizeof(info), "  S:%d M:%d%%", score, pct);
                if (len < 0) len = 0;                 // Avoid snprintf error
                int col = (14 - len) / 2;             // Center the text
                lcd.printString(info, col, 2);
                lcd.refresh();
            }
            lcd.printString(" PRESS TO RESTART ", 0, 5);
            lcd.refresh();
            if (getButton1() == 1) {
                // Back to menu
                state = STATE_MENU;
            }
            ThisThread::sleep_for(200ms);
            break;
        }
    }
}
// */
