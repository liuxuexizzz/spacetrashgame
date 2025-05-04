#ifndef SPACETRASHENGINE_H
#define SPACETRASHENGINE_H

#include "Utils.h"   // Direction, UserInput structures
#include "N5110.h"   // Drawing on LCD
#include "buzzer.h"  // Buzzer control class



/**
 * @brief Object types in the game
 */
enum ObjectType {
    OBJ_NONE = 0,           
    OBJ_PLAYER,             // Player aircraft
    OBJ_BULLET,             // Bullet
    OBJ_SMALL_TRASH,        // Small trash
    OBJ_BIG_TRASH,          // Big trash
    OBJ_PLANET,             // Planet
    OBJ_PROPS,              // Props
    OBJ_DUST,               // Dust
};



/**
 * @brief Game state types
 */
enum GameState {
    STATE_MENU,     // Start menu
    STATE_PLAYING,  // Game in progress
    STATE_PAUSED,   // Paused
    STATE_GAMEOVER  // Game over
};



/**
 * @brief Basic representation of objects in the game (e.g., player, bullet, trash)
 */
struct GameObject {
    ObjectType type;
    float x;            // x position (in pixels)
    float y;            // y position (in pixels)
    float w;            // width
    float h;            // height
    float vx;           // velocity in x direction
    float vy;           // velocity in y direction
    bool active; 
};



/**
 * @brief Game engine class definition
 */
class SpaceTrashEngine {
public:
    SpaceTrashEngine();


    /**
     * @brief Initialize game engine screen
     * @param screen_width  LCD width (default 84)
     * @param screen_height LCD height (default 48)
     */
    void init(int screen_width, int screen_height);


    /**
     * @brief Update game state per frame: player movement, firing, spawning, collision, score/lives
     * @param input  User input (direction + magnitude)
     * @return       Remaining lives, used by main.cpp to check game over
     */
    int update(const UserInput &input);


    /**
     * @brief Draw game content to N5110 buffer
     * @param lcd  Nokia 5110 LCD object
     */
    void draw(N5110 &lcd);


    /**
     * @brief Get total spawned and missed trash statistics
     * @return Total number of trash spawned and missed
     */
    int getTotalTrashSpawned() const { return _totalTrashSpawned; }
    int getTotalTrashMissed()  const { return _totalTrashMissed;  }
    

    /**
     * @brief Get remaining lives and current score
     * @return Remaining lives and current score
     */
    int getLives() const { return _lives; }
    int getScore() const { return _score; }


    /**
     * @brief Set buzzer controller
     */
    void setBuzzer(Buzzer* buzzer);




private:
    // Functions
    void _init_player();                            // Initialize player
    void _spawn_trash();                            // Spawn trash
    void _fire_bullet();                            // Fire bullet
    void _update_player(const UserInput &input);    // Update player state based on input
    void _update_bullet(GameObject &obj);           // Update bullet position
    void _update_trash(GameObject &obj);            // Update trash position
    bool _check_collision(const GameObject &a, const GameObject &b);            // Collision detection
    void _handle_collisions();                      // Handle collisions




private:
    int _width;   // Screen width
    int _height;  // Screen height

    int _lives;   // Lives
    int _score;   // Score

    int _totalTrashSpawned = 0;         // Total trash count
    int _totalTrashMissed  = 0;         // Missed trash count

    // Object limits
    static const int MAX_BULLETS = 5;   // Max number of bullets
    static const int MAX_TRASH   = 8;   // Max number of trash
    static const int MAX_PLANETS = 2;   // Max 2 planets at the same time
    static const int MAX_PROPS   = 3;   // Max 3 props
    static const int MAX_DUST    = 20;  // Max 20 dust particles
    static const int FPS         = 10;  // Should match sleep_for in main.cpp
    static const int DUST_LIFETIME_FRAMES = 3 * FPS;            // 3 seconds of lifetime (in frames)
    static const int kBoundaryY = 9;    // Top boundary of game area: first usable pixel below score/life row
    Buzzer* _buzzer = nullptr;
    

    GameObject _player;                 // Player object  
    GameObject _bullets[MAX_BULLETS];   // Bullets
    GameObject _trash[MAX_TRASH];       // Trash
    GameObject _planets[MAX_PLANETS];
    GameObject _props[MAX_PROPS];
    GameObject _dust[MAX_DUST];
    int _dust_life[MAX_DUST];           // Remaining lifetime of each dust particle (in frames)
    int _planet_hp[MAX_PLANETS];        // Remaining hits for each planet

    void _spawn_planet();
    void _spawn_props();
    void _spawn_dust(float x, float y, int count);
    void _update_planet(GameObject &obj);
    void _update_props(GameObject &obj);
    void _update_dust(int i);
    void _spawn_small_trash_pair(float cx, float cy);
    
};








#endif
