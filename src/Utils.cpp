#include "Utils.hpp"


float get_degrees(float degrees)
{
    return static_cast<float>(fmod(360 + fmod(degrees, 360), 360));
}

// double decToRad(short degrees)
// {
//     return static_cast<double>((M_PI * degrees) / 180);
// }

double decToRad(double degrees)
{
    return static_cast<double>((M_PI * degrees) / 180);
}