#include "Chip8.hpp"
#include "graphics.hpp"
#include "SDL.h"
#include <iostream>
#include <stdexcept>

Chip8 chip8("ibm.ch8");
SDL_Graphics screen(10);

int main(){
    SDL_Event event;
     while (1) {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
                break;
        chip8.cycle();
        if (chip8.bad_opcode){
            screen.destroy_screen();
            SDL_Quit();
            throw std::runtime_error("Bad opcode");
        }
        if (chip8.draw){
            screen.update_screen(chip8.window_pixels);
            chip8.print_gfx();
        }
    }   
}
