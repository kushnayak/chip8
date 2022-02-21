#include "Chip8.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>

#define _NNN(opcode)   (opcode & 0x0FFF)
#define _NN(opcode)    (opcode & 0x00FF)
#define _N(opcode)     (opcode & 0x000F)
#define _X(opcode)     ((opcode >> 8) & 0x000F)
#define _Y(opcode)     ((opcode >> 4) & 0x000F)


Chip8::Chip8(const char *file){
    init_system();
    std::cout << "Initialized chip8!" << std::endl;
    load_ROM(file); 
    std::cout << "Loaded " << file << "!" << std::endl;
}

void Chip8::init_system(){
    pc = 0x0200;
    opcode = 0;
    I = 0;
    sp = 0;
    
    memset(memory, 0, sizeof(memory));
    memset(V, 0, 16);
    memset(stack, 0, sizeof(stack));
    memset(gfx, 0, sizeof(gfx));
    update_window_pixels();
    
    for(uint16_t hex = 0; hex < 15; ++hex){
        memcpy(&memory[font_I + (hex * 5)], &hex_sprites[hex], 5);
    }
}

void Chip8::load_ROM(const char *file){
    std::ifstream rom_file(file, std::ios::binary | std::ios::ate);

    if (!rom_file.is_open()){
        std::cout << "Could not open ROM: " << file << std::endl;
        return;
    }

    std::streampos len = rom_file.tellg();
    
    rom_file.seekg(0, std::ios::beg);

    char *buffer = new char[len];
    rom_file.read(buffer, len);
    rom_file.close();

    memcpy(&memory[0x0200], buffer, len); 

    delete[] buffer;
}

void Chip8::fetch(){
    opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2; 
    //std::cout << "Fetched opcode: 0x" << std::hex << opcode << std::dec << std::endl;
}

void Chip8::execute(){
    draw = 0;
    bad_opcode = 0;
    switch (opcode & 0xF000) {
        case 0x0000: {
            switch (opcode & 0x0FFF){
                case 0x00E0:
                    draw = 1;
                    OP_00E0();
                    break;
                case 0x00EE:
                    break;
            }
            break;
        }

        case 0x1000:
            OP_1NNN();
            break;

        case 0x6000:
            OP_6XNN();
            break;

        case 0x7000:
            OP_7XNN();
            break;

        case 0xA000:
            OP_ANNN();
            break;

        case 0xD000:
            draw = 1;
            OP_DXYN();
            break;

        default:
            std::cout << "OPERATION 0x" << std::hex << opcode << " NOT FOUND." << std::dec;
            bad_opcode = 1;
    }
}

void Chip8::cycle(){
    fetch();
    execute();
}

void Chip8::update_window_pixels(){
    for(uint16_t pos = 0; pos < GFX_HEIGHT * GFX_WIDTH; ++pos){
        window_pixels[pos] = gfx[pos] ? DISPLAY_ON : DISPLAY_OFF;
    }
}

void Chip8::OP_00E0(){
    memset(gfx, 0, sizeof(gfx));
    update_window_pixels();
}

void Chip8::OP_1NNN(){
    pc = _NNN(opcode);
}

void Chip8::OP_6XNN(){
    V[_X(opcode)] = _NN(opcode);
    std::cout << std::hex << "fetched opcode " << opcode << "; putting NN: " << +_NN(opcode) << " into register X: " << _X(opcode) << std::endl;
}

void Chip8::OP_7XNN(){
    V[_X(opcode)] += _NN(opcode);
}

void Chip8::OP_ANNN(){
    I = _NNN(opcode);
}

void Chip8::OP_DXYN(){
    uint8_t start_row = V[_Y(opcode)] % (GFX_HEIGHT);
    uint8_t start_col = V[_X(opcode)] % (GFX_WIDTH);
    uint8_t n = _N(opcode);

    std::cout << std::hex << "Opcode: 0x" << opcode << std::endl;
    std::cout << std::dec << "Vx: " << +V[_X(opcode)] << " Vy: " << +V[_Y(opcode)] << " n: " << +n << std::endl;
    std::cout << std::hex << "I: 0x" << I << std::endl;
    V[0xF] = 0;
    for(uint8_t row = 0; row < n && start_row + row < GFX_HEIGHT; ++row){
        uint8_t mem_byte = memory[I + row];
        std::cout << std::hex << "Mem byte: 0x" << +mem_byte << " @ index: 0x" << +(I + row) << std::endl;
        for(uint8_t col = 0; col < 8 && start_col + col < GFX_WIDTH; ++col){
            std::cout << std::dec << " row: " << +row+start_row << " col: " << +col+start_col;
            uint8_t mem_pixel = (mem_byte >> (7 - col)) & 1; 
            uint8_t &gfx_pixel = gfx[(start_row + row) * GFX_WIDTH + start_col + col];
            std::cout << std::hex << " mem byte pixel: 0x" << +mem_pixel << " gfx pixel: 0x" << +gfx_pixel;
            if (mem_pixel & gfx_pixel){
                V[0xF] = 1;
            }
            gfx_pixel ^= mem_pixel;
            std::cout << std::hex << "(after XOR) gfx pixel: 0x" << +gfx_pixel << std::endl;
        }
    }

    update_window_pixels();
}

void Chip8::print_gfx(){
    for(int i = 0; i < GFX_HEIGHT; ++i){
        for(int j = 0; j < GFX_WIDTH; ++j){
            uint32_t val = window_pixels[i * GFX_WIDTH + j];
            if (val == DISPLAY_ON) std::cout << '*';
            else if (val == DISPLAY_OFF) std::cout << '.';
            else std::cout << std::hex << "Unkown pixel: 0x" << val;
            std::cout << ' ';
        }
        std::cout << std::endl;
    }
}