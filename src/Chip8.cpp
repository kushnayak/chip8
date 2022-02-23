#include "Chip8.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>

#define    _NNN(opcode)    (opcode & 0x0FFF)
#define     _NN(opcode)    (opcode & 0x00FF)
#define      _N(opcode)    (opcode & 0x000F)
#define      _X(opcode)    ((opcode >> 8) & 0x000F)
#define      _Y(opcode)    ((opcode >> 4) & 0x000F)


Chip8::Chip8(const char *file){
    init_chip8();
    std::cout << "Initialized chip8!" << std::endl;
    loaded_rom = load_ROM(file); 
}

void Chip8::cycle(){
    fetch();
    execute();
}

void Chip8::init_chip8(){
    pc = 0x0200;
    opcode = 0;
    I = 0;
    sp = 0;
    
    memset(memory, 0, sizeof(memory));
    memset(V, 0, sizeof(V));
    memset(stack, 0, sizeof(stack));
    memset(gfx, 0, sizeof(gfx));
    memset(pressed, 0, sizeof(pressed));
    update_window_pixels();
    
    for(uint8_t hex = 0; hex < 0xF0; ++hex){
        memcpy(&memory[START_HEX_SPRITE_I + (hex * 5)], &hex_sprites[hex], 5);
    }

    std::random_device dev;
    rand_num_gen = std::mt19937(dev());
    rand_dist = std::uniform_int_distribution<std::mt19937::result_type>(0, 255);
}

bool Chip8::load_ROM(const char *file){
    std::ifstream rom_file(file, std::ios::binary | std::ios::ate);

    if (!rom_file.is_open()){
        std::cout << "Could not open ROM: " << file << std::endl;
        return false;
    }

    std::streampos len = rom_file.tellg();
    
    rom_file.seekg(0, std::ios::beg);

    char *buffer = new char[len];
    rom_file.read(buffer, len);
    rom_file.close();

    memcpy(&memory[0x0200], buffer, len); 

    delete[] buffer;

    return true;
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

void Chip8::fetch(){
    opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2; 
}

void Chip8::execute(){
    draw = 0;
    bad_opcode = 0;
    switch (opcode & 0xF000) {
        case 0x0000: {
            switch (opcode & 0x00FF){
                case 0x00E0:
                    draw = 1;
                    OP_00E0();
                    break;
                case 0x00EE:
                    OP_00EE();
                    break;
            }
            break;
        }

        case 0x1000:
            OP_1NNN();
            break;

        case 0x2000:
            OP_2NNN();
            break;

        case 0x3000:
            OP_3XNN();
            break;

        case 0x4000:
            OP_4XNN();
            break;

        case 0x5000:
            OP_5XY0();
            break;

        case 0x6000:
            OP_6XNN();
            break;

        case 0x7000:
            OP_7XNN();
            break;

        case 0x8000: {
            switch (opcode & 0x000F){
                case 0x0000:
                    OP_8XY0();
                    break;

                case 0x0001:
                    OP_8XY1();
                    break;

                case 0x0002:
                    OP_8XY2();
                    break;

                case 0x0003:
                    OP_8XY3();
                    break;

                case 0x0004:
                    OP_8XY4();
                    break;

                case 0x0005:
                    OP_8XY5();
                    break;

                case 0x0006:
                    OP_8XY6();
                    break;

                case 0x0007:
                    OP_8XY7();
                    break;

                case 0x000E:
                    OP_8XYE();
                    break;
            }
            break;
        }

        case 0x9000:
            OP_9XY0();
            break;

        case 0xA000:
            OP_ANNN();
            break;

        case 0xB000:
            OP_BNNN();
            break;

        case 0xC000:
            OP_CXNN();
            break;

        case 0xD000:
            draw = 1;
            OP_DXYN();
            break;

        case 0xE000: {
            switch (opcode & 0x00FF){
                case 0x009E:
                    OP_EX9E();
                    break;

                case 0x00A1:
                    OP_EXA1();
                    break;
            }
            break;
        }

        case 0xF000: {
            switch (opcode & 0x00FF){
                case 0x0007:
                    OP_FX07();
                    break;

                case 0x000A:
                    OP_FX0A();
                    break;

                case 0x0015:
                    OP_FX15();
                    break;

                case 0x0018:
                    OP_FX18();
                    break;

                case 0x001E:
                    OP_FX1E();
                    break;

                case 0x0029:
                    OP_FX29();
                    break;

                case 0x0033:
                    OP_FX33();
                    break;

                case 0x0055:
                    OP_FX55();
                    break;

                case 0x0065:
                    OP_FX65();
                    break;
            }
            break;
        }

        default:
            std::cout << "OPERATION 0x" << std::hex << opcode << " NOT FOUND." << std::dec;
            bad_opcode = 1;
    }
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

void Chip8::OP_00EE(){
    pc = stack[--sp];
}

void Chip8::OP_1NNN(){
    pc = _NNN(opcode);
}

void Chip8::OP_2NNN(){
    stack[sp++] = pc;
    pc = _NNN(opcode); 
}

void Chip8::OP_3XNN(){
    if (V[_X(opcode)] == _NN(opcode)){
        pc += 2;
    }
}

void Chip8::OP_4XNN(){
    if (V[_X(opcode)] != _NN(opcode)){
        pc += 2;
    }
}

void Chip8::OP_5XY0(){
    if (V[_X(opcode)] == V[_Y(opcode)]){
        pc += 2;
    }
}

void Chip8::OP_6XNN(){
    V[_X(opcode)] = _NN(opcode);
}

void Chip8::OP_7XNN(){
    V[_X(opcode)] += _NN(opcode);
}



void Chip8::OP_8XY0(){
    V[_X(opcode)] = V[_Y(opcode)];
}

void Chip8::OP_8XY1(){
    V[_X(opcode)] |= V[_Y(opcode)];
}

void Chip8::OP_8XY2(){
    V[_X(opcode)] &= V[_Y(opcode)];
}

void Chip8::OP_8XY3(){
    V[_X(opcode)] ^= V[_Y(opcode)];
}

void Chip8::OP_8XY4(){
    uint16_t sum = V[_X(opcode)] + V[_Y(opcode)];
    V[0xF] = sum > 0xFF;
    V[_X(opcode)] = sum;
}

void Chip8::OP_8XY5(){
    V[0xF] = V[_X(opcode)] > V[_Y(opcode)];
    V[_X(opcode)] -= V[_Y(opcode)];
}

void Chip8::OP_8XY6(){
    V[0xF] = V[_X(opcode)] & 0x1;
    V[_X(opcode)] >>= 1;
}

void Chip8::OP_8XY7(){
    V[0xF] = V[_Y(opcode)] > V[_X(opcode)];
    V[_X(opcode)] = V[_Y(opcode)] - V[_X(opcode)];
}

void Chip8::OP_8XYE(){
    V[0xF] = (V[_X(opcode)] & 0x80) >> 7;
    V[_X(opcode)] <<= 1;
}

void Chip8::OP_9XY0(){
    if (V[_X(opcode)] != V[_Y(opcode)]){
        pc += 2;
    }
}

void Chip8::OP_ANNN(){
    I = _NNN(opcode);
}

void Chip8::OP_BNNN(){
    pc = _NNN(opcode) + V[0x0];
}

void Chip8::OP_CXNN(){
    uint8_t rand_num = rand_dist(rand_num_gen);
    V[_X(opcode)] = rand_num & _NN(opcode);
}

void Chip8::OP_DXYN(){
    uint8_t start_row = V[_Y(opcode)] % (GFX_HEIGHT);
    uint8_t start_col = V[_X(opcode)] % (GFX_WIDTH);
    uint8_t n = _N(opcode);

    V[0xF] = 0;
    for(uint8_t row = 0; row < n && start_row + row < GFX_HEIGHT; ++row){
        uint8_t mem_byte = memory[I + row];
        for(uint8_t col = 0; col < 8 && start_col + col < GFX_WIDTH; ++col){
            uint8_t mem_pixel = (mem_byte >> (7 - col)) & 1; 
            uint8_t &gfx_pixel = gfx[(start_row + row) * GFX_WIDTH + start_col + col];
            if (mem_pixel & gfx_pixel){
                V[0xF] = 1;
            }
            gfx_pixel ^= mem_pixel;
        }
    }

    update_window_pixels();
}

void Chip8::OP_EX9E(){
    if (pressed[V[_X(opcode)]]){
        pc += 2;
    }
}

void Chip8::OP_EXA1(){
    if (!pressed[V[_X(opcode)]]){
        pc += 2;
    }
}

void Chip8::OP_FX07(){
    V[_X(opcode)] = delay_timer;
}

void Chip8::OP_FX0A(){
    for(uint8_t hex = 0x0; hex < 0xF; ++hex){
        if (pressed[hex]){
            V[_X(opcode)] = hex;
            return;
        }
    }
    pc -= 2;
}

void Chip8::OP_FX15(){
    delay_timer = V[_X(opcode)];
}

void Chip8::OP_FX18(){
    sound_timer = V[_X(opcode)];
}

void Chip8::OP_FX1E(){
    I += V[_X(opcode)];
}

void Chip8::OP_FX29(){
    I = START_HEX_SPRITE_I + V[_X(opcode)] * 5;
}

void Chip8::OP_FX33(){
    uint8_t num = V[_X(opcode)];
    memory[I + 2] = num % 10;
    num /= 10;
    memory[I + 1] = num % 10;
    num /= 10;
    memory[I] = num;
}

void Chip8::OP_FX55(){
    memcpy(&memory[I], &V[0x0], _X(opcode) + 1);
}

void Chip8::OP_FX65(){
    memcpy(&V[0x0], &memory[I], _X(opcode) + 1);
}



