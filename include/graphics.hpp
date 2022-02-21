#pragma once

#include "SDL.h"

class SDL_Graphics {
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    const uint8_t GFX_WIDTH;
    const uint8_t GFX_HEIGHT;
    const uint32_t WINDOW_WIDTH;
    const uint32_t WINDOW_HEIGHT;

    SDL_Graphics(uint32_t scale);

    bool init_screen();
    void update_screen(uint32_t *pixels);
    void clear_screen();
    void destroy_screen();

};
