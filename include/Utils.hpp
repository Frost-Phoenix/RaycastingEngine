#pragma once

#include "pch.hpp"


typedef sf::Vector2<double> Vector2d;

#define CELL_SIZE 32
#define TEXTURE_SIZE 64
#define SCREEN_WIDTH  960
#define SCREEN_HEIGHT 540
#define NB_LAYERS 4

constexpr unsigned short FOV = 70;
constexpr unsigned short MAX_RAY_LEGHT = 100;

#define MAP_COLLISION_LAYER 0
#define MAP_WALL_LAYER 1
#define MAP_FLOOR_LAYER 2
#define MAP_CEILING_LAYER 3
#define MAP_PLAYER_LAYER 4

#define MAP_PLAYER_ID 1
#define MAP_WALL_ID 2
#define MAP_HORIZONTAL_DOOR_ID 3
#define MAP_VERTICAL_DOOR_ID 4

float get_degrees(float i_degrees);
double decToRad(double degrees);
