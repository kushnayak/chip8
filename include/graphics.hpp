#pragma once

#include "SDL.h"

class SDL_Graphics {
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;

    const uint8_t GFX_WIDTH;
    const uint8_t GFX_HEIGHT;
    const uint32_t WINDOW_WIDTH;
    const uint32_t WINDOW_HEIGHT;
    bool quit;

    SDL_Graphics(uint32_t scale);

    bool init_screen();
    void update_screen(uint32_t *pixels);
    void destroy_screen();

/*    original            QWERTY
 *     layout             layout 
 *
 *   1  2  3  C         1  2  3  4
 *   4  5  6  D   -->   Q  W  E  R
 *   7  8  9  E   -->   A  S  D  F
 *   A  0  B  F         Z  X  C  V
 */
    enum Keys {
        KEY_X, // 0
        KEY_1, // 1
        KEY_2, // 2
        KEY_3, // 3
        KEY_Q, // 4
        KEY_W, // 5
        KEY_E, // 6
        KEY_A, // 7
        KEY_S, // 8
        KEY_D, // 9
        KEY_Z, // A
        KEY_C, // B
        KEY_4, // C
        KEY_R, // D
        KEY_F, // E
        KEY_V, // F
    };

    bool handle_keypress(bool *pressed);
};
