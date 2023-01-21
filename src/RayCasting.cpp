#include "RayCasting.hpp"


// Constructor
RayCasting::RayCasting()
{
    this->fovVisualization = sf::VertexArray(sf::TriangleFan, NB_RAY + 1);
}

RayCasting::~RayCasting()
{
    
}

// Private functions
float RayCasting::RayDepthHorizontalCollison(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, float angle)
{
    float rayLenght = 0.f;
    sf::Vector2f step(0,0);
    sf::Vector2f rayEndPos(0,0);

    float cos_a = std::cos(angle);
    float sin_a = std::sin(angle);
    if (cos_a == 0) cos_a += 0.000001;
    if (sin_a == 0) sin_a += 0.000001;
    
    if (sin_a > 0)
    {
        step.y = CELL_SIZE;
        rayEndPos.y = std::ceil(pos.y / CELL_SIZE) * CELL_SIZE;
    }
    else
    {
        step.y = -1 * CELL_SIZE;
        rayEndPos.y = std::floor(pos.y / CELL_SIZE) * CELL_SIZE - 0.001;
    }

    rayLenght = (rayEndPos.y - pos.y) / sin_a;
    rayEndPos.x = pos.x + rayLenght * cos_a;

    float deltaRayLenght = ((step.y / CELL_SIZE) / sin_a) * (float)CELL_SIZE;
    step.x = deltaRayLenght * cos_a;

    while (rayLenght < MAX_RAY_LEGHT)
    {
        if (mapManager->chekPointCollision(rayEndPos)) break;
        rayEndPos.x += step.x;
        rayEndPos.y += step.y;
        rayLenght += deltaRayLenght;
    }

    if (rayLenght > MAX_RAY_LEGHT) return MAX_RAY_LEGHT;
    return rayLenght;    
}

float RayCasting::RayDepthVerticalCollison(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, float angle)
{
    float rayLenght = 0.f;
    sf::Vector2f step;
    sf::Vector2f rayEndPos(0,0);

    float cos_a = std::cos(angle);
    float sin_a = std::sin(angle);
    if (cos_a == 0) cos_a += 0.000001;
    if (sin_a == 0) sin_a += 0.000001;

    if (cos_a > 0)
    {
        step.x = CELL_SIZE;
        rayEndPos.x = std::ceil(pos.x / CELL_SIZE) * CELL_SIZE;
    }
    else
    {
        step.x = -1 * CELL_SIZE;
        rayEndPos.x = std::floor(pos.x / CELL_SIZE) * CELL_SIZE - 0.001;
    }

    rayLenght = (rayEndPos.x - pos.x) / cos_a;
    rayEndPos.y = pos.y + rayLenght * sin_a;

    float deltaRayLenght = ((step.x / CELL_SIZE) / cos_a) * (float)CELL_SIZE;
    step.y = deltaRayLenght * sin_a;

    while (rayLenght < MAX_RAY_LEGHT)
    {
        if (mapManager->chekPointCollision(rayEndPos)) break;
        rayEndPos.x += step.x;
        rayEndPos.y += step.y;
        rayLenght += deltaRayLenght;
    }

    if (rayLenght > MAX_RAY_LEGHT) return MAX_RAY_LEGHT;
    return rayLenght;
}

sf::Vector2f RayCasting::castRay(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, float angle)
{
    sf::Vector2f rayEndPos;

    float verticalRayLenght = this->RayDepthVerticalCollison(mapManager, pos, angle);
    float horizontalRayLenght = this->RayDepthHorizontalCollison(mapManager, pos, angle);

    float rayLenght = 0.f;
    if (verticalRayLenght < horizontalRayLenght) rayLenght = verticalRayLenght;
    else rayLenght = horizontalRayLenght;

    return sf::Vector2f(pos.x + rayLenght * std::cos(angle), pos.y + rayLenght * std::sin(angle));
}

// Accesors

// Public functions
void RayCasting::update(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, short baseAngle)
{
    this->fovVisualization[0].position = sf::Vector2f(pos.x, pos.y);

    float angle = get_degrees(baseAngle - FOV / 2);

    for (unsigned short i = 0; i < NB_RAY; i++)
    {
        this->fovVisualization[i + 1].position = this->castRay(mapManager, pos, static_cast<float>(decToRad(angle)));
        angle = get_degrees(angle + DELTA_ANGLE);
    }
}

void RayCasting::renderFovVisualisation(std::shared_ptr<sf::RenderTarget> renderTarget)
{    
    renderTarget->draw(this->fovVisualization);
}

void RayCasting::render(std::shared_ptr<sf::RenderTarget> renderTarget)
{    
    
}
