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
    quit = 0;
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

void SDL_Graphics::destroy_screen(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    texture = nullptr;
    renderer = nullptr;
    window = nullptr;
}


bool SDL_Graphics::handle_keypress(bool *pressed){
    bool updated = false;
    while (SDL_PollEvent(&event)){
        updated = true;
        switch (event.type){
            case SDL_QUIT:
                quit = 1;
                break;

            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym){
                    case SDLK_x:
                        pressed[KEY_X] = 1;
                        break;

                    case SDLK_1:
                        pressed[KEY_1] = 1;
                        break;

                    case SDLK_2:
                        pressed[KEY_2] = 1;
                        break;

                    case SDLK_3:
                        pressed[KEY_3] = 1;
                        break;

                    case SDLK_q:
                        pressed[KEY_Q] = 1;
                        break;

                    case SDLK_w:
                        pressed[KEY_W] = 1;
                        break;

                    case SDLK_e:
                        pressed[KEY_E] = 1;
                        break;

                    case SDLK_a:
                        pressed[KEY_A] = 1;
                        break;

                    case SDLK_s:
                        pressed[KEY_S] = 1;
                        break;

                    case SDLK_d:
                        pressed[KEY_D] = 1;
                        break;

                    case SDLK_z:
                        pressed[KEY_Z] = 1;
                        break;

                    case SDLK_c:
                        pressed[KEY_C] = 1;
                        break;

                    case SDLK_4:
                        pressed[KEY_4] = 1;
                        break;

                    case SDLK_r:
                        pressed[KEY_R] = 1;
                        break;

                    case SDLK_f:
                        pressed[KEY_F] = 1;
                        break;

                    case SDLK_v:
                        pressed[KEY_V] = 1;
                        break;

                    default:
                        std::cout << "Key press not identified" << std::endl;
                }
                break;
            }


            case SDL_KEYUP: {
                switch (event.key.keysym.sym){
                    case SDLK_x:
                        pressed[KEY_X] = 0;
                        break;

                    case SDLK_1:
                        pressed[KEY_1] = 0;
                        break;

                    case SDLK_2:
                        pressed[KEY_2] = 0;
                        break;

                    case SDLK_3:
                        pressed[KEY_3] = 0;
                        break;

                    case SDLK_q:
                        pressed[KEY_Q] = 0;
                        break;

                    case SDLK_w:
                        pressed[KEY_W] = 0;
                        break;

                    case SDLK_e:
                        pressed[KEY_E] = 0;
                        break;

                    case SDLK_a:
                        pressed[KEY_A] = 0;
                        break;

                    case SDLK_s:
                        pressed[KEY_S] = 0;
                        break;

                    case SDLK_d:
                        pressed[KEY_D] = 0;
                        break;

                    case SDLK_z:
                        pressed[KEY_Z] = 0;
                        break;

                    case SDLK_c:
                        pressed[KEY_C] = 0;
                        break;

                    case SDLK_4:
                        pressed[KEY_4] = 0;
                        break;

                    case SDLK_r:
                        pressed[KEY_R] = 0;
                        break;

                    case SDLK_f:
                        pressed[KEY_F] = 0;
                        break;

                    case SDLK_v:
                        pressed[KEY_V] = 0;
                        break;
                }
                break;
            }
        }
    }
    return updated;
}
