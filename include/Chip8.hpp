#pragma once

#include <cstdint>
#include <random>

class Chip8 {
public:
    static const uint8_t GFX_WIDTH = 64;
    static const uint8_t GFX_HEIGHT = 32;

    const uint16_t START_HEX_SPRITE_I = 0;
    const uint8_t hex_sprites[16][5] = {
        {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
        {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
        {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
        {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
        {0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
        {0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
        {0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
        {0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
        {0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
        {0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
        {0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
        {0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
        {0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
        {0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
        {0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
        {0xF0, 0x80, 0xF0, 0x80, 0x80}  // F
    };

    uint16_t opcode;
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t stack[16];
    uint16_t pc;
    uint8_t sp;
    uint16_t I;
    uint8_t delay_timer;
    uint8_t sound_timer;

    const uint32_t DISPLAY_ON = 0xFFFFFFFFU;
    const uint32_t DISPLAY_OFF = 0x00000000U;
    uint8_t gfx[GFX_HEIGHT * GFX_WIDTH];
    uint32_t window_pixels[GFX_HEIGHT * GFX_WIDTH];
    bool draw;
    bool bad_opcode;

    // random stuff
    std::mt19937 rand_num_gen;
    std::uniform_int_distribution<std::mt19937::result_type> rand_dist; 

    void update_window_pixels();
    void print_gfx();

    Chip8(const char *file);

    void init_chip8();
    void load_ROM(const char *file);

    void fetch();
    void execute();
    void cycle();       

    // operations
    void OP_00E0();
    void OP_00EE();
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XNN();
    void OP_4XNN();
    void OP_5XY0();
    void OP_6XNN();
    void OP_7XNN();
    void OP_8XY0();
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();
    void OP_9XY0();
    void OP_ANNN();
    void OP_BNNN();
    void OP_CXNN();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();
    void OP_FX07();
    void OP_FX0A();
    void OP_FX15();
    void OP_FX18();
    void OP_FX1E();
    void OP_FX29();
    void OP_FX33();
    void OP_FX55();
    void OP_FX65();

};
