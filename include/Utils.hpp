#pragma once

#include "pch.hpp"


#define CELL_SIZE 32
// #define MAP_WIDTH 20
// #define MAP_HEIGHT 15
#define SCREEN_WIDTH  900
#define SCREEN_HEIGHT 500

constexpr unsigned short FOV = 80;
constexpr unsigned short NB_RAY = SCREEN_WIDTH;
constexpr unsigned short MAX_RAY_LEGHT = 1000;
constexpr float DELTA_ANGLE = static_cast<float>(FOV) / static_cast<float>(NB_RAY);


float get_degrees(float i_degrees);
// double decToRad(short degrees);
double decToRad(double degrees);