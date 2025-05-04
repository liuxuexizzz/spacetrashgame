#include "Sprites.h"

void drawPlayerSprite(N5110 &lcd, int x, int y) {
    lcd.drawSprite(x, y, 9, 9, (int*)PLAYER_SPRITE);
}

void drawBulletSprite(N5110 &lcd, int x, int y) {
    lcd.drawSprite(x, y, 2, 2, (int*)BULLET_SPRITE);
}

void drawSmallTrashSprite(N5110 &lcd, int x, int y) {
    lcd.drawSprite(x, y, 5, 5, (int*)SMALL_TRASH_SPRITE);
}

void drawBigTrashSprite(N5110 &lcd, int x, int y) {
    lcd.drawSprite(x, y, 7, 7, (int*)BIG_TRASH_SPRITE);
}

/*
void drawPlanetSprite(N5110 &lcd, int x, int y) {
    //lcd.drawSprite(x, y, 21, 21, (int*)PLANET_SPRITE);
    lcd.drawCircle(x, y, 10, FILL_BLACK);
}
*/

void drawPropsSprite(N5110 &lcd, int x, int y) {
    lcd.drawSprite(x, y, 5, 5, (int*)PROPS_SPRITE);
}

void drawDustSprite(N5110 &lcd, int x, int y) {
    lcd.drawSprite(x, y, 1, 1, (int*)DUST_SPRITE);
}
