#include "Utils.hpp"


float get_degrees(float degrees)
{
    return static_cast<float>(std::fmod(360 + std::fmod(degrees, 360), 360));
}

double decToRad(double degrees)
{
    return static_cast<double>((M_PI * degrees) / 180);
}