#include "graphics.hpp"
#include <iostream>


SDL_Graphics::SDL_Graphics(uint32_t scale) : 
    GFX_WIDTH(64),
    GFX_HEIGHT(32),
    WINDOW_WIDTH(GFX_WIDTH * scale),
    WINDOW_HEIGHT(GFX_HEIGHT * scale)
{
    if (init_screen()){
        std::cout << "Screen initialized!" << std::endl;
    }
}

bool SDL_Graphics::init_screen(){

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Unable to initialize video: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("CHIP8",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, 
                              WINDOW_HEIGHT, 
			      SDL_WINDOW_SHOWN);

    if (window == nullptr){
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return false;
    }


    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == nullptr){
        std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                GFX_WIDTH,
                                GFX_HEIGHT);
    if (texture == nullptr){
            std::cout << "Could not create texture: " << SDL_GetError() << std::endl;
            return false;
    }

    return true;
}

void SDL_Graphics::update_screen(uint32_t *pixels){
    SDL_UpdateTexture(texture, nullptr, pixels, GFX_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);	
}

void SDL_Graphics::clear_screen(){
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void SDL_Graphics::destroy_screen(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    texture = nullptr;
    renderer = nullptr;
    window = nullptr;
}
