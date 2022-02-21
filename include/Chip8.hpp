#pragma once

#include <cstdint>

class Chip8 {
public:
    static const uint8_t GFX_WIDTH = 64;
    static const uint8_t GFX_HEIGHT = 32;

    const uint16_t font_I = 0;
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

    void update_window_pixels();
    void print_gfx();

    Chip8(const char *file);

    void init_system();
    void load_ROM(const char *file);

    void fetch();
    void execute();
    void cycle();       

    // operations
    void OP_00E0();
    void OP_1NNN();
    void OP_6XNN();
    void OP_7XNN();
    void OP_ANNN();
    void OP_DXYN();

};
