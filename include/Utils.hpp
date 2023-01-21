#pragma once

#include "pch.hpp"


#define CELL_SIZE 32
#define MAP_WIDTH 20
#define MAP_HEIGHT 15
#define SCREEN_WIDTH  MAP_WIDTH * CELL_SIZE
#define SCREEN_HEIGHT MAP_HEIGHT * CELL_SIZE

constexpr unsigned short FOV = 70;
constexpr unsigned short NB_RAY = 1024;
constexpr unsigned short MAX_RAY_LEGHT = 350;
constexpr float DELTA_ANGLE = static_cast<float>(FOV) / static_cast<float>(NB_RAY);


float get_degrees(float i_degrees);
double decToRad(short degrees);