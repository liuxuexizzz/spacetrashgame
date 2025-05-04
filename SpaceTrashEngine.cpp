#include "SpaceTrashEngine.h"
#include <cstdlib>  // srand, rand
#include "Sprites.h"
#include <cmath>    // sqrtf
#include "switch.h"
#include "buzzer.h"


/**
 * @brief Initialize game parameters
 */
SpaceTrashEngine::SpaceTrashEngine()
{
    // Set default values first
    _width  = 84;  // Nokia 5110
    _height = 48;  
    _lives  = 3;
    _score  = 0;
}



/**
 * @brief Initialize game objects
 */
void SpaceTrashEngine::init(int screen_width, int screen_height)
{
    _width  = screen_width;
    _height = screen_height;

    _lives = 3;  // Initial lives
    _score = 0;  // Initial score

    // Initialize player
    _init_player();

    // Initialize bullet array
    for (int i = 0; i < MAX_BULLETS; i++) {
        _bullets[i].type = OBJ_BULLET;
        _bullets[i].active = false;
    }

    // Initialize trash array
    for (int i = 0; i < MAX_TRASH; i++) {
        _trash[i].type   = OBJ_SMALL_TRASH;
        _trash[i].active = false;
    }

    // Initialize planets
    for (int i = 0; i < MAX_PLANETS; i++) {
        _planets[i].type   = OBJ_PLANET;
        _planets[i].active = false;
        _planet_hp[i]      = 0;           // Initial HP is 0
    }
    // Initialize props
    for (int i = 0; i < MAX_PROPS; i++) {
        _props[i].type   = OBJ_PROPS;
        _props[i].active = false;
    }
    // Initialize dust
    for (int i = 0; i < MAX_DUST; i++) {
        _dust[i].type   = OBJ_DUST;
        _dust[i].active = false;
        _dust_life[i]   = 0;   // Dust lifetime
    }
}



/**
 * @brief Sound control
 */ 
void SpaceTrashEngine::setBuzzer(Buzzer* buzzer) {
    _buzzer = buzzer;
}



/**
 * @brief Object status update
 */
int SpaceTrashEngine::update(const UserInput &input)
{
    // 1) Update player position
    _update_player(input);

    // 2) Randomly spawn trash (or periodically)
    //    Here we use a 1/40 chance to spawn one
    if ((rand() % 40) == 0) {
        _spawn_trash();
    }

    // 3) Fire bullet: example -> if direction is E and magnitude > 0.5, fire
    if (getButton2() == 1) {
        _fire_bullet();
    }

    // 4) Update bullet positions
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (_bullets[i].active) {
            _update_bullet(_bullets[i]);
        }
    }

    // 5) Update trash positions
    for (int i = 0; i < MAX_TRASH; i++) {
        if (_trash[i].active) {
            _update_trash(_trash[i]);
        }
    }

    // Randomly spawn props (about 1/150 chance)
    if ((rand() % 150) == 0) {
        _spawn_props();
    }
    // Randomly spawn planets (about 1/200 chance)
    if ((rand() % 200) == 0) {
        _spawn_planet();
    }

    // Update planets
    for (int i = 0; i < MAX_PLANETS; i++) {
        if (_planets[i].active) {
            _update_planet(_planets[i]);
        }
    }
    // Update props
    for (int i = 0; i < MAX_PROPS; i++) {
        if (_props[i].active) {
            _update_props(_props[i]);
        }
    }
    // Update dust
    for (int i = 0; i < MAX_DUST; i++) {
        if (_dust[i].active) {
            _update_dust(i);
        }
    }

    // 6) Collision detection
    _handle_collisions();

    // Return remaining lives
    return _lives;
}



/**
 * @brief Render game screen to LCD buffer
 */
void SpaceTrashEngine::draw(N5110 &lcd)
{
    lcd.clear();

    // Draw top boundary line (below text row)
    lcd.drawLine(0, 8, _width, 8, FILL_BLACK);
    
    // Draw player
    if (_player.active) {
        drawPlayerSprite(lcd, (int)_player.x, (int)_player.y);
    }
    
    // Draw bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (_bullets[i].active) {
            drawBulletSprite(lcd, (int)_bullets[i].x, (int)_bullets[i].y);
        }
    }
    
    // Draw trash, choose sprite based on size
    for (int i = 0; i < MAX_TRASH; i++) {
        if (_trash[i].active) {
            if (_trash[i].w == 5) {
                drawSmallTrashSprite(lcd, (int)_trash[i].x, (int)_trash[i].y);
            } else if (_trash[i].w == 7) {
                drawBigTrashSprite(lcd, (int)_trash[i].x, (int)_trash[i].y);
            }

        }
    }
    
    // Draw planets (radius 10 transparent circle)
    for (int i = 0; i < MAX_PLANETS; i++) {
        if (_planets[i].active) {
            // Draw using center coordinates with offset
            lcd.drawCircle(
                int(_planets[i].x + 10),
                int(_planets[i].y + 10),
                10,
                FILL_BLACK
            );
        }
    }

    // Draw props (5x5 pentagram)
    for (int i = 0; i < MAX_PROPS; i++) {
        if (_props[i].active) {
            drawPropsSprite(lcd, (int)_props[i].x, (int)_props[i].y);
        }
    }

    // Draw dust (1x1 small square)
    for (int i = 0; i < MAX_DUST; i++) {
        if (_dust[i].active) {
            drawDustSprite(lcd, (int)_dust[i].x, (int)_dust[i].y);
        }
    }

    // Show score and lives (as a string on row 0)
    char buf[20];
    sprintf(buf, "S:%d L:%d", _score, _lives);
    lcd.printString(buf, 0, 0);
    
    lcd.refresh();
}



/**
 * @brief Initialize player attributes
 */
void SpaceTrashEngine::_init_player()
{
    _player.type   = OBJ_PLAYER;
    _player.w      = 9;                             // 9x9
    _player.h      = 9;
    _player.x      = 1;
    _player.y      = (_height - _player.h) / 2;     // Vertically centered
    _player.vx     = 0;
    _player.vy     = 0;
    _player.active = true;
}



/**
 * @brief Randomly generate trash; randomly decide to generate small (70%) or big (30%) trash on the right
 */
void SpaceTrashEngine::_spawn_trash()
{
    float r = (float)rand() / (float)RAND_MAX; // Random number r ∈ [0,1)
    if (r < 0.7) {
        // 70% chance to generate small trash: 5x5 diamond
        for (int i = 0; i < MAX_TRASH; i++) {
            if (!_trash[i].active) {
                _trash[i].active = true;
                _totalTrashSpawned++;
                _trash[i].x = (float)(_width - 1);
                _trash[i].w = 5;
                _trash[i].h = 5;
                // **Restrict y between upper and lower boundaries**
                {
                    int minY = kBoundaryY;  // Upper boundary
                    int maxY = _height - kBoundaryY - _trash[i].h; 
                    int span = maxY - minY;
                    _trash[i].y = float(minY + (rand() % span));
                }
                _trash[i].vx = -1.0f - ((rand() % 10) / 10.0f);
                _trash[i].vy = 0;
                break;
            }
        }
    } else {
        // 30% chance to generate big trash: 7x7
        for (int i = 0; i < MAX_TRASH; i++) {
            if (!_trash[i].active) {
                _trash[i].active = true;
                _trash[i].x = (float)(_width - 1);
                _trash[i].w = 7;
                _trash[i].h = 7;
                // **Restrict y between upper and lower boundaries**
                {
                    int minY = kBoundaryY;
                    int maxY = _height - kBoundaryY - _trash[i].h;
                    int span = maxY - minY;
                    _trash[i].y = float(minY + (rand() % span));
                }
                _trash[i].vx = -0.5f - ((rand() % 10) / 20.0f);
                _trash[i].vy = 0;
                break;
            }
        }
    }
}



/**
 * @brief Generate props
 */ 
void SpaceTrashEngine::_spawn_props() {
    for (int i = 0; i < MAX_PROPS; i++) {
        if (!_props[i].active) {
            _props[i].active = true;
            _props[i].x = float(_width - 1);
            _props[i].w = 5;
            _props[i].h = 5;
            // Limit y to [kBoundaryY, _height - kBoundaryY - h)
            {
                int minY = kBoundaryY;
                int maxY = _height - kBoundaryY - _props[i].h;
                int span = maxY - minY;
                if (span > 0) {
                    _props[i].y = float(minY + (rand() % span));
                } else {
                    // If space insufficient, center it
                    _props[i].y = float((_height - _props[i].h) / 2);
                }
            }
            _props[i].vx = -1.0f;
            _props[i].vy = 0;
            break;
        }
    }
}



/**
 * @brief Generate planets
 */
void SpaceTrashEngine::_spawn_planet() {
    for (int i = 0; i < MAX_PLANETS; i++) {
        if (!_planets[i].active) {
            _planets[i].active = true;
            _planets[i].x  = (float)(_width - 20);
            const int minY = kBoundaryY;
            const int maxY = _height - kBoundaryY - _planets[i].h; // h == 20
            _planets[i].y = float(minY + (rand() % maxY));
            _planets[i].w  = 20; _planets[i].h = 20;
            _planets[i].vx = -0.3f - (rand()%10)/100.0f; _planets[i].vy = 0;
            _planet_hp[i]  = 3;           // Takes 3 hits to destroy
            break;
        }
    }
}



/**
 * @brief Generate dust particles
 */
void SpaceTrashEngine::_spawn_dust(float x, float y, int count) {
    for (int i = 0; i < MAX_DUST && count > 0; i++) {
        if (!_dust[i].active) {
            _dust[i].active = true;
            _dust[i].x  = x;
            _dust[i].y  = y;
            // Random direction
            _dust[i].vx = ((rand()%200)/100.0f - 1.0f) * 1.5f;
            _dust[i].vy = ((rand()%200)/100.0f - 1.0f) * 1.5f;
            _dust[i].w = _dust[i].h = 1;
            _dust_life[i]   = DUST_LIFETIME_FRAMES;    // 3 seconds = 30 frames
            count--;
        }
    }
}



/**
 * @brief Update method: deactivate if object moves out of screen
 */
void SpaceTrashEngine::_update_planet(GameObject &o) {
    o.x += o.vx;
    if (o.x + o.w < 0) o.active = false;
}



/**
 * @brief Update props
 */
void SpaceTrashEngine::_update_props(GameObject &o) {
    o.x += o.vx;
    if (o.x + o.w < 0) o.active = false;
}



/**
 * @brief Update dust state
 */
void SpaceTrashEngine::_update_dust(int i) {
    GameObject &o = _dust[i];

    // Move first
    o.x += o.vx;
    o.y += o.vy;

    // Countdown frames
    if (--_dust_life[i] <= 0) {
        o.active = false;
        return;
    }

    // Deactivate immediately if out of bounds
    if (o.x < 0 || o.x > _width || o.y < 0 || o.y > _height) {
        o.active = false;
    }
}



/**
 * @brief Fire bullet: fire from the right side of player, using a 2x2 bullet
 */
void SpaceTrashEngine::_fire_bullet()
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!_bullets[i].active) {
            _bullets[i].active = true;
            _bullets[i].x = _player.x + _player.w;
            _bullets[i].y = _player.y + _player.h / 2 - 1; // center align
            _bullets[i].vx = 2.0f;
            _bullets[i].vy = 0;
            _bullets[i].w  = 2;
            _bullets[i].h  = 2;
            break;
        }
    }
}



/**
 * @brief Update player position based on joystick input: move up/down, constrained by 9x9 player size
 */
void SpaceTrashEngine::_update_player(const UserInput &input)
{
    float speed = 1.5f;
    if (input.d == N) {           // Move up
        _player.y -= speed * input.mag;
    } else if (input.d == S) {    // Move down
        _player.y += speed * input.mag;
    }
    // Keep player inside boundary
    if (_player.y < kBoundaryY) {
        _player.y = kBoundaryY;
    }
    if (_player.y + _player.h > _height) {
        _player.y = _height - _player.h;
    }
}



/**
 * @brief Update bullet position, deactivate if it exits right boundary
 */
void SpaceTrashEngine::_update_bullet(GameObject &obj)
{
    obj.x += obj.vx;
    obj.y += obj.vy;
    if (obj.x > _width) {
        obj.active = false;
    }
}



/**
 * @brief Update trash position, deactivate and deduct 20 points if it exits left boundary
 */
void SpaceTrashEngine::_update_trash(GameObject &obj)
{
    obj.x += obj.vx;
    obj.y += obj.vy;
    if (obj.x + obj.w < 0) {
        obj.active = false;
        _score -= 20;
        _totalTrashMissed++;
    }
}



/**
 * @brief Simple AABB collision detection
 */
bool SpaceTrashEngine::_check_collision(const GameObject &a, const GameObject &b)
{
    if (!a.active || !b.active) return false;

    // Axis-aligned bounding box collision
    bool overlap_x = (a.x < (b.x + b.w)) && ((a.x + a.w) > b.x);
    bool overlap_y = (a.y < (b.y + b.h)) && ((a.y + a.h) > b.y);
    return (overlap_x && overlap_y);
}



/**
 * @brief Handle collisions between bullets & trash, player & trash, etc.
 */
void SpaceTrashEngine::_handle_collisions()
{
    // 1) Bullet - Trash collision
    for (int b = 0; b < MAX_BULLETS; ++b) {
        if (!_bullets[b].active) continue;
        for (int t = 0; t < MAX_TRASH; ++t) {
            if (_trash[t].active && _check_collision(_bullets[b], _trash[t])) {
                _bullets[b].active = false;
                // On hit: spawn dust if small trash, or split if big trash
                if (_trash[t].w == 5) {
                    _spawn_dust(_trash[t].x, _trash[t].y, 12);
                } else if (_trash[t].w == 7) { 
                    _spawn_small_trash_pair(_trash[t].x, _trash[t].y);
                }
                _trash[t].active = false;
                _score += 10;
                if (_buzzer) _buzzer->play_note(1000, 100);
                break;
            }
        }
    }

    // 2) Player - Trash collision
    for (int t = 0; t < MAX_TRASH; ++t) {
        if (_trash[t].active && _check_collision(_player, _trash[t])) {
            _trash[t].active = false;
            _lives--;
            _spawn_dust(_trash[t].x, _trash[t].y, 16);
        }
    }

    // 3) Player - Props collision
    for (int i = 0; i < MAX_PROPS; ++i) {
        if (_props[i].active && _check_collision(_player, _props[i])) {
            _props[i].active = false;
            _score += 20;
            if (_buzzer) _buzzer->play_note(1500, 150);
            _spawn_dust(_props[i].x, _props[i].y, 8);
        }
    }

    // 4) Bullet - Planet collision
    for (int b = 0; b < MAX_BULLETS; ++b) {
        if (!_bullets[b].active) continue;
        for (int p = 0; p < MAX_PLANETS; ++p) {
            if (_planets[p].active && _check_collision(_bullets[b], _planets[p])) {
                _bullets[b].active = false;
                _planet_hp[p]--;
                if (_planet_hp[p] <= 0) {
                    _planets[p].active = false;
                    _spawn_dust(_planets[p].x + 10, _planets[p].y + 10, 20);
                    _score += 50;
                    if (_buzzer) _buzzer->play_note(800, 200);
                } else {
                    _spawn_dust(_planets[p].x + 10, _planets[p].y + 10, 5);
                    if (_buzzer) _buzzer->play_note(1200, 80);
                }
                break;
            }
        }
    }

    // 5) Player - Planet collision
    for (int p = 0; p < MAX_PLANETS; ++p) {
        if (_planets[p].active && _check_collision(_player, _planets[p])) {
            _planets[p].active = false;
            _lives--;
            _spawn_dust(_planets[p].x + 10, _planets[p].y + 10, 20);
        }
    }

    // Ensure score is non-negative
    if (_score < 0) {
        _score = 0;
    }
}



/**
 * @brief Generate two small trash at (cx, cy) after big trash is destroyed
 */ 
void SpaceTrashEngine::_spawn_small_trash_pair(float cx, float cy) {
    // 1) Calculate player center
    float px = _player.x + _player.w * 0.5f;
    float py = _player.y + _player.h * 0.5f;

    // 2) Direction vector toward player (normalized)
    float dx = px - cx, dy = py - cy;
    float mag = sqrtf(dx*dx + dy*dy);
    float ux = 0, uy = 0;
    if (mag > 0.0f) {
        ux = dx / mag;
        uy = dy / mag;
    } else {
        ux = -1.0f; uy = 0.0f;
    }

    // 3) Perpendicular vector (ux, uy)
    float perp_x = -uy;
    float perp_y = ux;

    // 4) Set spacing and speed
    const float separation = 6.0f;
    const float speed      = 1.5f;
    int spawned = 0;

    // 5) Find 2 inactive trash slots
    for (int i = 0; i < MAX_TRASH && spawned < 2; i++) {
        if (!_trash[i].active) {
            _trash[i].active = true;
            _trash[i].type   = OBJ_SMALL_TRASH;
            _trash[i].w      = 5;
            _trash[i].h      = 5;

            float offset = (spawned == 0 ? 1.0f : -1.0f) * (separation * 0.5f);
            _trash[i].x = cx + perp_x * offset;
            _trash[i].y = cy + perp_y * offset;

            _trash[i].vx = ux * speed;
            _trash[i].vy = uy * speed;

            spawned++;
        }
    }
}
