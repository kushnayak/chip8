#!/bin/bash
g++-11 -std=c++17 -Wall -Wextra -O3 -D_THREAD_SAFE -I/usr/local/include/SDL2 -L/usr/local/lib -lSDL2 -I include/ src/*.cpp
