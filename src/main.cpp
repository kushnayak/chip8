#include "Chip8.hpp"
#include "graphics.hpp"
#include "SDL.h"
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>


using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

void update_timer(bool &quit, uint8_t &delay_timer, uint8_t &sound_timer){
    while (!quit){
        if (delay_timer){
            --delay_timer;
        }
        if (sound_timer){
            --sound_timer;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(17));
    }
}

int main(int argc, char **argv){

    bool quit_thread = false;
    SDL_Graphics screen(20);
    Chip8 chip8(argv[1]);

    std::thread timer_thread(update_timer, 
                            std::ref(quit_thread),
                            std::ref(chip8.delay_timer),
                            std::ref(chip8.sound_timer));

    time_point last_draw = std::chrono::high_resolution_clock::now();

    if (!chip8.loaded_rom){
        goto exit;
    }

    while (1){
        if (screen.handle_keypress(chip8.pressed)){
            for(int i=0; i<16; ++i)
                if(chip8.pressed[i])
                    std::cout << i << " pressed" << std::endl;
            if (screen.quit){
                break;
            }
        }
        chip8.cycle();
        if (chip8.bad_opcode){
            std::cout << "ABORT: Bad opcode" << std::endl;
            break;
        }
        if (chip8.draw){
            time_point now = std::chrono::high_resolution_clock::now();
            uint64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_draw).count();
            if (elapsed < 17){
                std::this_thread::sleep_for(std::chrono::milliseconds(17 - elapsed));
            }
            last_draw = std::chrono::high_resolution_clock::now();
            screen.update_screen(chip8.window_pixels);
        }
    }   

exit:
    quit_thread = true;
    timer_thread.join();

    screen.destroy_screen();
    SDL_Quit();
}
