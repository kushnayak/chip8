#!/bin/bash
g++-11 -std=c++17 -Wall -Wextra -O3 $(pkg-config sdl2 --cflags --libs) -I include/ src/*.cpp
