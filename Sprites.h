#ifndef SPRITES_H
#define SPRITES_H

#include "N5110.h"


// Player spaceship (OBJ_PLAYER): 9x9 matrix
static const int PLAYER_SPRITE[9][9] = {
    {0,0,1,0,0,0,0,0,0},
    {0,1,1,1,0,0,0,0,0},
    {1,1,1,1,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,0,0,0},
    {0,1,1,1,0,0,0,0,0},
    {0,0,1,0,0,0,0,0,0}
};

// Player bullet (OBJ_BULLET): 2x2 matrix
static const int BULLET_SPRITE[2][2] = {
    {1,1},
    {1,1}
};

// Small trash (OBJ_SMALL_TRASH): 5x5 diamond
static const int SMALL_TRASH_SPRITE[5][5] = {
    {0,0,1,0,0},
    {0,1,1,1,0},
    {1,1,1,1,1},
    {0,1,1,1,0},
    {0,0,1,0,0}
};

// Big trash (OBJ_BIG_TRASH): 7x7 hexagon (approximate)
static const int BIG_TRASH_SPRITE[7][7] = {
    {0,0,1,1,1,0,0},
    {0,1,1,1,1,1,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
    {0,1,1,1,1,1,0},
    {0,0,1,1,1,0,0}
};

// Planet (OBJ_PLANET): 10-pixel diameter circle (drawn in SpaceTrashEngine.cpp)

// Power-up prop (OBJ_PROPS): 5x5 star
static const int PROPS_SPRITE[5][5] = {
    {0,0,1,0,0},
    {0,1,1,1,0},
    {1,1,1,1,1},
    {0,1,1,1,0},
    {0,1,0,1,0}
};

// Dust particle (OBJ_DUST_*): 1x1
static const int DUST_SPRITE[1][1] = {
    {1}
};

void drawPlayerSprite(N5110 &lcd, int x, int y);
void drawBulletSprite(N5110 &lcd, int x, int y);
void drawSmallTrashSprite(N5110 &lcd, int x, int y);
void drawBigTrashSprite(N5110 &lcd, int x, int y);
void drawPlanetSprite(N5110 &lcd, int x, int y);
void drawPropsSprite(N5110 &lcd, int x, int y);
void drawDustSprite(N5110 &lcd, int x, int y);

#endif
