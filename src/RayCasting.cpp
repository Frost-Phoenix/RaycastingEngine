#include "RayCasting.hpp"


// Constructor
RayCasting::RayCasting(bool drawMap)
{
    this->drawMap = drawMap;
    this->fovVisualization = sf::VertexArray(sf::TriangleFan, NB_RAY + 1);

    this->textureManager = std::unique_ptr<TextureManager>(new TextureManager());
}

RayCasting::~RayCasting()
{
    
}

// Private functions
RayHitInfo RayCasting::castHorizontalRay(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, double angle)
{
    double rayLength = 0.f;
    // Delta x and y to move one tile on the y axis
    sf::Vector2f step;
    // Ray end pos used for checking the colision with a wall
    sf::Vector2f rayEndPos(0, 0);
    RayHitInfo rayHitInfo;

    double cos_a = std::cos(angle);
    double sin_a = std::sin(angle);
    // To avoid division by zero error later
    if (cos_a == 0) cos_a += 0.000001;
    if (sin_a == 0) sin_a += 0.000001;

    // If the angle dirrection face up, move one cell up 
    if (sin_a > 0)
    {
        step.y = CELL_SIZE;
        rayEndPos.y = std::ceil(pos.y / CELL_SIZE) * CELL_SIZE;
    }
    // Else, move one cell down 
    else
    {
        step.y = -1 * CELL_SIZE;
        rayEndPos.y = std::floor(pos.y / CELL_SIZE) * CELL_SIZE - 0.001;
    }

    // Ray length for ritching the first intersection on the y axis
    rayLength = (rayEndPos.y - pos.y) / sin_a;
    rayEndPos.x = pos.x + rayLength * cos_a;

    // Delta ray length to move one cell on the y axis
    double deltaRayLength = ((step.y / CELL_SIZE) / sin_a) * (double)CELL_SIZE;
    step.x = deltaRayLength * cos_a;

    // While the ray length is smaller then the max ray length, we extend the ray one cell on the y axis
    while (rayLength < MAX_RAY_LEGHT)
    {
        // If the ray hit a wall we stop expending the ray
        if (mapManager->chekPointCollision(rayEndPos))
        {
            short colideCellId = mapManager->getCellId("collision", rayEndPos);
            
            if (colideCellId == MAP_WALL_ID)
            {
                rayHitInfo.textureCol = static_cast<short>(std::floor(std::fmod(rayEndPos.x, CELL_SIZE) * 2));
                break;   
            } 
            else if (colideCellId == MAP_HORIZONTAL_DOOR_ID)
            {
                sf::Vector2f tmpRayEndPos(rayEndPos.x + step.x / 2, rayEndPos.y + step.y / 2);

                if (mapManager->chekPointCollision(tmpRayEndPos) && mapManager->getCellId("collision", tmpRayEndPos) == MAP_HORIZONTAL_DOOR_ID)
                {
                    rayLength += deltaRayLength / 2;
                    rayHitInfo.textureCol = static_cast<short>(std::floor(std::fmod(tmpRayEndPos.x, CELL_SIZE) * 2));
                    break;
                }
            }
        }
        
        // Expending the ray one cell on the y axis
        rayEndPos.x += step.x;
        rayEndPos.y += step.y;
        rayLength += deltaRayLength;
    }

    // We return the MAX_RAY_LEGHT if the ray length is biger, otherwise the ray length
    if (rayLength > MAX_RAY_LEGHT) rayHitInfo.length = MAX_RAY_LEGHT;
    else rayHitInfo.length = rayLength;

    rayHitInfo.textureId = mapManager->getCellId("walls", rayEndPos);

    if (sin_a > 0) rayHitInfo.textureCol = TEXTURE_SIZE - 1 - rayHitInfo.textureCol;
    
    return rayHitInfo;
}

RayHitInfo RayCasting::castVerticalRay(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, double angle)
{
    double rayLength = 0.f;
    // Delta x and y to move one tile on the x axis
    sf::Vector2f step;
    // Ray end pos used for checking the colision with a wall
    sf::Vector2f rayEndPos(0, 0);
    RayHitInfo rayHitInfo;

    double cos_a = std::cos(angle);
    double sin_a = std::sin(angle);
    // To avoid division by zero error later
    if (cos_a == 0) cos_a += 0.000001;
    if (sin_a == 0) sin_a += 0.000001;

    // If the angle dirrection face right, move one cell to the right 
    if (cos_a > 0)
    {
        step.x = CELL_SIZE;
        rayEndPos.x = std::ceil(pos.x / CELL_SIZE) * CELL_SIZE;
    }
    // Else, move one cell to the left 
    else
    {
        step.x = -1 * CELL_SIZE;
        rayEndPos.x = std::floor(pos.x / CELL_SIZE) * CELL_SIZE - 0.001;
    }

    // Ray length for ritching the first intersection on the x axis
    rayLength = (rayEndPos.x - pos.x) / cos_a;
    rayEndPos.y = pos.y + rayLength * sin_a;

    // Delta ray length to move one cell on the x axis
    double deltaRayLength = ((step.x / CELL_SIZE) / cos_a) * (double)CELL_SIZE;
    step.y = deltaRayLength * sin_a;

    // While the ray length is smaller then the max ray length, we extend the ray one cell on the x axis
    while (rayLength < MAX_RAY_LEGHT)
    {
        // If the ray hit a wall we stop expending the ray
        if (mapManager->chekPointCollision(rayEndPos))
        {
            short colideCellId = mapManager->getCellId("collision", rayEndPos);
            
            if (colideCellId == MAP_WALL_ID)
            {
                rayHitInfo.textureCol = static_cast<short>(std::floor(std::fmod(rayEndPos.y, CELL_SIZE) * 2));
                break;   
            } 
            else if (colideCellId == MAP_VERTICAL_DOOR_ID)
            {
                sf::Vector2f tmpRayEndPos(rayEndPos.x + step.x / 2, rayEndPos.y + step.y / 2);

                if (mapManager->chekPointCollision(tmpRayEndPos) && mapManager->getCellId("collision", tmpRayEndPos) == MAP_VERTICAL_DOOR_ID)
                {
                    rayLength += deltaRayLength / 2;
                    rayHitInfo.textureCol = static_cast<short>(std::floor(std::fmod(tmpRayEndPos.y, CELL_SIZE) * 2));
                    break;
                }
            }
        }
        // Expending the ray one cell on the x axis
        rayEndPos.x += step.x;
        rayEndPos.y += step.y;
        rayLength += deltaRayLength;
    }

    // We return the MAX_RAY_LEGHT if the ray length is biger, otherwise the ray length
    if (rayLength > MAX_RAY_LEGHT) rayHitInfo.length = MAX_RAY_LEGHT;
    else rayHitInfo.length = rayLength;

    rayHitInfo.textureId = mapManager->getCellId("walls", rayEndPos);

    if (cos_a < 0) rayHitInfo.textureCol = TEXTURE_SIZE - 1 - rayHitInfo.textureCol;
    
    return rayHitInfo;
}

RayHitInfo RayCasting::castRay(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, double angle)
{
    // Distance between the player and the closest in the angle dirrection by checking colision on the x axis
    RayHitInfo verticalRay = this->castVerticalRay(mapManager, pos, angle);
    // Distance between the player and the closest in the angle dirrection by checking colision on the y axis
    RayHitInfo horizontalRay = this->castHorizontalRay(mapManager, pos, angle);

    // Take the smallest ray
    RayHitInfo rayHitInfo;
    
    if (verticalRay.length < horizontalRay.length) rayHitInfo = verticalRay;
    else rayHitInfo = horizontalRay;

    return rayHitInfo;
}

// Accesors

// Public functions
void RayCasting::toogleFovDrawing(bool state)
{
    this->drawMap = state;
}

void RayCasting::update(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, short baseAngle)
{
    if (this->drawMap) this->fovVisualization[0].position = sf::Vector2f(pos.x, pos.y);

    // Go left to right, so the start angle is half the FOV left to the player angle
    double angle = get_degrees(baseAngle - FOV / 2);

    // Calculate the distance of each rays
    for (unsigned short i = 0; i < NB_RAY; i++)
    {
        //Distance between the player and the closest wall in the angle dirrection
        RayHitInfo rayHitInfo = this->castRay(mapManager, pos, decToRad(angle));

        if (this->drawMap)
        {
            sf::Vector2f hitPos = sf::Vector2f(pos.x + rayHitInfo.length * std::cos(decToRad(angle)), pos.y + rayHitInfo.length * std::sin(decToRad(angle)));
            this->fovVisualization[i + 1].position = hitPos;
        }
        
        // Remove fisheye effect
        rayHitInfo.length *= std::cos(decToRad(baseAngle - angle));
        
        this->raysHitInfos[i] = rayHitInfo;

        angle = get_degrees(angle + DELTA_ANGLE);
    }
}

void RayCasting::renderFovVisualisation(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    renderTarget->draw(this->fovVisualization);
}

void RayCasting::render(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    // Distance of the projection 
    double screeDist = (SCREEN_WIDTH / 2) / std::tan(decToRad(FOV / 2));
    short last_col_x = -1;

    // For all rays draw the projection column
    for (unsigned short i = 0; i < NB_RAY; i++)
    {
        RayHitInfo rayHitInfo = this->raysHitInfos[i];

        // Height of the projection column
        double projectionHeight = screeDist / (rayHitInfo.length / CELL_SIZE);

        // To avoid distortion at the borders of the screen
        double ray_direction = FOV * (std::floor(0.5f * SCREEN_WIDTH) - i) / (SCREEN_WIDTH - 1);
        double ray_position = 0.5f * std::tan(decToRad(ray_direction)) / std::tan(decToRad(0.5f * FOV));

        short current_col_x = (short)std::round(SCREEN_WIDTH * (0.5f - ray_position));
        short next_col_x = current_col_x + 1;

        // Filling missing column
        if (current_col_x - 1 == last_col_x)
        {
            double next_ray_direction = FOV * (std::floor(0.5f * SCREEN_WIDTH) - i - 1) / (SCREEN_WIDTH - 1);
            double next_ray_position = 0.5f * std::tan(decToRad(next_ray_direction)) / std::tan(decToRad(0.5f * FOV));

            next_col_x = (short)std::round(SCREEN_WIDTH * (0.5f - next_ray_position));
        }
            
        last_col_x = current_col_x;

        short colWidth = next_col_x - current_col_x;
        sf::Vector2f colPos(current_col_x, (SCREEN_HEIGHT / 2.f) - (projectionHeight / 2.f));
        
        this->textureManager->renderTextureLine(renderTarget, rayHitInfo.textureId, colPos, rayHitInfo.textureCol, colWidth, projectionHeight);
    }
}
