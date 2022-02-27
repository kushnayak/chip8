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
        // 60 Hz
        std::this_thread::sleep_for(std::chrono::milliseconds(17));
    }
}

int main(int argc, char **argv){
    if (argc < 2){
        std::cout << "Enter a path to a Chip8 ROM" << std::endl;
        return 1;
    }
    Chip8 chip8(argv[1]);
    if (!chip8.loaded_rom){
        return 1;
    }

    SDL_Graphics screen(20);

    bool quit_thread = false;
    std::thread timer_thread(update_timer, 
                            std::ref(quit_thread),
                            std::ref(chip8.delay_timer),
                            std::ref(chip8.sound_timer));

    time_point last_draw = std::chrono::high_resolution_clock::now();
    time_point last_cycle = std::chrono::high_resolution_clock::now();

    while (1){
        if (screen.handle_keypress(chip8.pressed)){
            for(int i=0; i<16; ++i)
                if(chip8.pressed[i])
                    std::cout << i << " pressed" << std::endl;
            if (screen.quit){
                break;
            }
        }
        time_point now = std::chrono::high_resolution_clock::now();
        uint64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_cycle).count();
        if (elapsed < 5){
            std::this_thread::sleep_for(std::chrono::milliseconds(3 - elapsed));
        }

        last_cycle = std::chrono::high_resolution_clock::now();

        chip8.cycle();

        if (chip8.bad_opcode){
            std::cout << "ABORT: Bad opcode" << std::endl;
            break;
        }
        if (chip8.draw){
            time_point now = std::chrono::high_resolution_clock::now();
            uint64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_draw).count();
            // 60 FPS
            if (elapsed < 17){
                std::this_thread::sleep_for(std::chrono::milliseconds(17 - elapsed));
            }
            last_draw = std::chrono::high_resolution_clock::now();
            screen.update_screen(chip8.window_pixels);
        }
    }   

    quit_thread = true;
    timer_thread.join();

    screen.destroy_screen();
    SDL_Quit();
}
