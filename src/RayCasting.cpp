#include "RayCasting.hpp"


// Constructor
RayCasting::RayCasting(bool drawMap)
{
    this->plane = Vector2f(0, 0.7);
    this->screeDist = (SCREEN_WIDTH / 2) / std::tan(decToRad(FOV / 2));
    
    this->drawMap = drawMap;
    if (drawMap) this->fovVisualization = sf::VertexArray(sf::TriangleFan, SCREEN_WIDTH + 1);

    this->textureManager = std::unique_ptr<TextureManager>(new TextureManager());
}

RayCasting::~RayCasting()
{
    
}

// Private functions
    // while (rayLength < MAX_RAY_LEGHT)
    // {
    //     // If the ray hit a wall we stop expending the ray
    //     if (mapManager->chekPointCollision(rayEndPos))
    //     {
    //         short collideCellId = mapManager->getCellId("collision", rayEndPos);
            
    //         if (collideCellId == MAP_WALL_ID)
    //         {
    //             rayHitInfo.textureCol = static_cast<short>(std::floor(std::fmod(rayEndPos.y, CELL_SIZE) * (TEXTURE_SIZE / CELL_SIZE)));
    //             // rayHitInfo.addShadows = true;
    //             break;   
    //         } 
    //         else if (collideCellId == MAP_VERTICAL_DOOR_ID)
    //         {
    //             Vector2d tmpRayEndPos(rayEndPos.x + step.x / 2, rayEndPos.y + step.y / 2);

    //             if (mapManager->chekPointCollision(tmpRayEndPos) && mapManager->getCellId("collision", tmpRayEndPos) == MAP_VERTICAL_DOOR_ID)
    //             {
    //                 double doorOpeningState = mapManager->getDoorOpeningState(mapManager->getCellPos(tmpRayEndPos));
                    
    //                 if (std::fmod(tmpRayEndPos.y, CELL_SIZE) > doorOpeningState)
    //                 {
    //                     hitDoor = true;
    //                     rayLength += deltaRayLength / 2;
    //                     rayHitInfo.textureCol = static_cast<short>(std::floor(std::fmod(tmpRayEndPos.y, CELL_SIZE) * 2 - doorOpeningState * (TEXTURE_SIZE / CELL_SIZE)));
    //                     // rayHitInfo.addShadows = false;
    //                     break;
    //                 }
    //             }
    //         }
    //     }

    // if (cos_a < 0 && !hitDoor) rayHitInfo.textureCol = TEXTURE_SIZE - 1 - rayHitInfo.textureCol;

void RayCasting::castWalls(std::shared_ptr<MapManager> mapManager, const Vector2f playerPos, const Vector2i playerCellPos, const Vector2f playerDir)
{
    RayHitInfo rayHitInfo;
    Vector2i cellPos;

    for (unsigned short x = 0; x < SCREEN_WIDTH; x++)
    {
        //calculate ray position and direction
        double cameraX = 2 * (SCREEN_WIDTH - x) / (double)SCREEN_WIDTH - 1; //x-coordinate in camera space

        sf::Vector2<double> rayDir;
        rayDir.x = playerDir.x + this->plane.x * cameraX;
        rayDir.y = playerDir.y + this->plane.y * cameraX;

        //which box of the map we're in
        cellPos = playerCellPos;

        //length of ray from one x or y-side to next x or y-side 
        Vector2d deltaDist;
        deltaDist.x = (rayDir.x == 0) ? 1e30 : std::abs(1 / rayDir.x);
        deltaDist.y = (rayDir.y == 0) ? 1e30 : std::abs(1 / rayDir.y);

        //what direction to step in x or y-direction (either +1 or -1)

        Vector2i step;
        Vector2d sideDist;

        //calculate step and initial sideDist
        if (rayDir.x < 0)
        {
            step.x = -1;
            sideDist.x = (playerPos.x - cellPos.x) * deltaDist.x;
        }
        else
        {
            step.x = 1;
            sideDist.x = (cellPos.x + 1.0 - playerPos.x) * deltaDist.x;
        }
        if (rayDir.y < 0)
        {
            step.y = -1;
            sideDist.y = (playerPos.y - cellPos.y) * deltaDist.y;
        }
        else
        {
            step.y = 1;
            sideDist.y = (cellPos.y + 1.0 - playerPos.y) * deltaDist.y;
        }

        //perform DDA
        while (true)
        {
            //jump to next map square, either in x-direction, or in y-direction
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                cellPos.x += step.x;
                rayHitInfo.hitSide = 0;
            }
            else
            {
                sideDist.y += deltaDist.y;
                cellPos.y += step.y;
                rayHitInfo.hitSide = 1;
            }
            //Check if ray has hit a wall
            if (mapManager->chekPointCollision(Vector2f(cellPos.x * CELL_SIZE, cellPos.y * CELL_SIZE))) break;
        }

        //Calculate distance of perpendicular ray (Euclidean distance would give fisheye effect!)
        if (rayHitInfo.hitSide == 0) rayHitInfo.length = (sideDist.x - deltaDist.x);
        else rayHitInfo.length = (sideDist.y - deltaDist.y);

        Vector2f rayEndPos;
        rayEndPos.x = (playerPos.x + rayHitInfo.length * rayDir.x) * CELL_SIZE;
        rayEndPos.y = (playerPos.y + rayHitInfo.length * rayDir.y) * CELL_SIZE;

        double deltaCell;
        if (rayHitInfo.hitSide == 0) deltaCell = playerPos.y + rayHitInfo.length * rayDir.y;
        else deltaCell = playerPos.x + rayHitInfo.length * rayDir.x;
        deltaCell -= std::floor(deltaCell);

        //x coordinate on the texture
        rayHitInfo.textureCol = short(deltaCell * double(TEXTURE_SIZE));

        // colision on vertical rayHitInfo.hitSide
        if (rayHitInfo.hitSide == 0)
        {
            if (rayDir.x < 0)
            {
                rayEndPos.x -= 0.001;
                rayHitInfo.textureCol = TEXTURE_SIZE - rayHitInfo.textureCol - 1;
            }
            else
            {
                rayEndPos.x += 0.001;
            }
        }
        // colision on horizontal rayHitInfo.hitSide
        else if (rayHitInfo.hitSide == 1)
        {
            if (rayDir.y > 0)
            {
                rayEndPos.y += 0.001;
                rayHitInfo.textureCol = TEXTURE_SIZE - rayHitInfo.textureCol - 1;
            }
            else
            {
                rayEndPos.y -= 0.001;
            }
        }

        if (this->drawMap) this->fovVisualization[x + 1].position = rayEndPos;
        
        rayHitInfo.textureId = mapManager->getCellId("walls", rayEndPos);

        this->raysHitInfos[x] = rayHitInfo;
    }
}

void RayCasting::castFloor(Vector2f playerPos, Vector2f playerDir)
{
    sf::Color color;

    // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
    double rayDirX0 = playerDir.x + this->plane.x;
    double rayDirY0 = playerDir.y + this->plane.y;
    double rayDirX1 = playerDir.x - this->plane.x;
    double rayDirY1 = playerDir.y - this->plane.y;

    //FLOOR CASTING
    for(unsigned short y = 0; y < SCREEN_HEIGHT / 2; y++)
    {
      // Current y position compared to the center of the screen (the horizon)
      short p = y - SCREEN_HEIGHT / 2;

      // Vertical position of the camera.
      double posZ = 0.5 * this->screeDist;

      // Horizontal distance from the camera to the floor for the current row.
      // 0.5 is the z position exactly in the middle between floor and ceiling.
      double rowDistance = posZ / p;

      // calculate the real world step vector we have to add for each x (parallel to camera plane)
      // adding step by step avoids multiplications with a weight in the inner loop
      double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
      double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

      // real world coordinates of the leftmost column. This will be updated as we step to the right.
      double floorX = -playerPos.x + rowDistance * rayDirX0;
      double floorY = -playerPos.y + rowDistance * rayDirY0;

      for(int x = 0; x < SCREEN_WIDTH; ++x)
      {
        // the cell coord is simply got from the integer parts of floorX and floorY
        short cellX = (short)(floorX);
        short cellY = (short)(floorY);

        floorX += floorStepX;
        floorY += floorStepY;

        // Floor 
        if ((cellX + cellY) % 2) color = sf::Color(75, 55, 25);
        else color = sf::Color(50, 35, 0);

        this->textureManager->drawPixel(x, SCREEN_HEIGHT - y - 1, color);

        // Ceiling 
        if ((cellX + cellY) % 2) color = sf::Color(0, 95, 120);
        else color = sf::Color(0, 60, 90);
        
        this->textureManager->drawPixel(x, y, color);
      }
    }
}

void RayCasting::renderFloor(std::shared_ptr<sf::RenderWindow> renderTarget)
{
    this->textureManager->renderScreenBuffer(renderTarget);
}

void RayCasting::renderWalls(std::shared_ptr<sf::RenderWindow> renderTarget)
{
    for (unsigned short x = 0; x < SCREEN_WIDTH; x++)
    {
        RayHitInfo rayHitInfo = this->raysHitInfos[x];
        
        //Calculate height of line to draw on screen
        short lineHeight = (this->screeDist / rayHitInfo.length);

        //calculate lowest pixel to fill in current stripe
        short drawStart = (SCREEN_HEIGHT / 2 - lineHeight / 2);

        this->textureManager->renderTextureLine(renderTarget, rayHitInfo.textureId, Vector2f(x, drawStart), rayHitInfo.textureCol, lineHeight, rayHitInfo.hitSide == 0);
    }
}

// Accesors

// Public functions
void RayCasting::toogleFovDrawing(const bool state)
{
    this->drawMap = state;
}

void RayCasting::rotatePlane(const double rotationSpeed)
{
    double oldDirX = this->plane.x;
    this->plane.x = this->plane.x * std::cos(rotationSpeed) - this->plane.y * std::sin(rotationSpeed);
    this->plane.y = oldDirX * std::sin(rotationSpeed) + this->plane.y * std::cos(rotationSpeed);
}

void RayCasting::update(std::shared_ptr<MapManager> mapManager, Vector2f playerPos, const Vector2f playerDir)
{
    if (this->drawMap) this->fovVisualization[0].position = Vector2f(playerPos.x, playerPos.y);

    Vector2i playerCellPos = mapManager->getCellPos(playerPos);
    playerPos = Vector2f(playerPos.x / CELL_SIZE, playerPos.y / CELL_SIZE);
    
    this->castFloor(playerPos, playerDir);
    this->castWalls(mapManager, playerPos, playerCellPos, playerDir);
}

void RayCasting::renderFovVisualisation(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    renderTarget->draw(this->fovVisualization);
}

void RayCasting::render(std::shared_ptr<sf::RenderWindow> renderTarget)
{
    this->renderFloor(renderTarget);
    this->renderWalls(renderTarget);
}