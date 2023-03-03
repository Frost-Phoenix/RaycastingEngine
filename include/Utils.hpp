#pragma once

#include "pch.hpp"


typedef sf::Vector2<double> Vector2d;

#define CELL_SIZE 32
#define TEXTURE_SIZE 64
#define SCREEN_WIDTH  900
#define SCREEN_HEIGHT 500

constexpr unsigned short FOV = 70;
constexpr unsigned short MAX_RAY_LEGHT = 100;

#define MAP_PLAYER_ID 1
#define MAP_WALL_ID 2
#define MAP_HORIZONTAL_DOOR_ID 3
#define MAP_VERTICAL_DOOR_ID 4

float get_degrees(float i_degrees);
double decToRad(double degrees);