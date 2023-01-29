#pragma once

#include "pch.hpp"


#define CELL_SIZE 32
#define SCREEN_WIDTH  900
#define SCREEN_HEIGHT 500

constexpr unsigned short FOV = 80;
constexpr unsigned short NB_RAY = SCREEN_WIDTH;
constexpr unsigned short MAX_RAY_LEGHT = 1000;
constexpr float DELTA_ANGLE = static_cast<float>(FOV) / static_cast<float>(NB_RAY);

#define MAP_WALL_ID 1
#define MAP_HORIZONTAL_DOOR_ID 2
#define MAP_VERTICAL_DOOR_ID 3

float get_degrees(float i_degrees);
double decToRad(double degrees);