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

void RayCasting::castWalls(std::shared_ptr<MapManager> mapManager, const Vector2f playerPos, const Vector2i playerCellPos, const Vector2f playerDir)
{
    RayHitInfo rayHitInfo;

    for (unsigned short x = 0; x < SCREEN_WIDTH; x++)
    {
        //calculate ray position and direction
        double cameraX = 2 * (SCREEN_WIDTH - x) / (double)SCREEN_WIDTH - 1; //x-coordinate in camera space

        Vector2d rayDir;
        rayDir.x = playerDir.x + this->plane.x * cameraX;
        rayDir.y = playerDir.y + this->plane.y * cameraX;

        //which box of the map we're in
        Vector2i cellPos = playerCellPos;

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

        bool hitDoor = false;

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
            
            // Check ray collision
            if (mapManager->chekPointCollision(Vector2f(cellPos.x * CELL_SIZE, cellPos.y * CELL_SIZE)))
            {
                short collideCellId = mapManager->getCellId(MAP_COLLISION_LAYER, cellPos);
                
                if (collideCellId == MAP_WALL_ID)
                {
                    break;   
                } 
                else if ((collideCellId == MAP_HORIZONTAL_DOOR_ID || collideCellId == MAP_VERTICAL_DOOR_ID))
                {
                    Vector2f tmpRayEndPos;
                    
                    if (rayHitInfo.hitSide == 0)
                    {
                        tmpRayEndPos.x = (playerPos.x + (sideDist.x + deltaDist.x / 2 - deltaDist.x) * rayDir.x) * CELL_SIZE;
                        tmpRayEndPos.y = (playerPos.y + (sideDist.x + deltaDist.x / 2 - deltaDist.x) * rayDir.y) * CELL_SIZE;
                    }
                    else
                    {
                        tmpRayEndPos.x = (playerPos.x + (sideDist.y + deltaDist.y / 2 - deltaDist.y) * rayDir.x) * CELL_SIZE;
                        tmpRayEndPos.y = (playerPos.y + (sideDist.y + deltaDist.y / 2 - deltaDist.y) * rayDir.y) * CELL_SIZE;
                    }
                    
                    collideCellId = mapManager->getCellId(MAP_COLLISION_LAYER, Vector2i(tmpRayEndPos.x / CELL_SIZE, tmpRayEndPos.y / CELL_SIZE));

                    if (mapManager->chekPointCollision(tmpRayEndPos) && (collideCellId == MAP_HORIZONTAL_DOOR_ID || collideCellId == MAP_VERTICAL_DOOR_ID))
                    {
                        float doorOpeningState = mapManager->getDoorOpeningState(mapManager->getCellPos(tmpRayEndPos));

                        double tmpDeltaCell;
                        
                        if (rayHitInfo.hitSide == 0)
                        {
                            tmpDeltaCell = std::fmod(tmpRayEndPos.y, CELL_SIZE);
                        }
                        else
                        {
                            tmpDeltaCell = std::fmod(tmpRayEndPos.x, CELL_SIZE);
                        }
                        
                        if (tmpDeltaCell > doorOpeningState)
                        {
                            hitDoor = true;
                            
                            if (rayHitInfo.hitSide == 0)
                            {
                                sideDist.x += deltaDist.x / 2;
                            }
                            else
                            {
                                sideDist.y += deltaDist.y / 2;
                            }

                            rayHitInfo.textureCol = (short)(std::floor(tmpDeltaCell * 2 - doorOpeningState * (TEXTURE_SIZE / CELL_SIZE)));

                            break;
                        }
                    }
                }
            }
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
        if (!hitDoor) rayHitInfo.textureCol = short(deltaCell * double(TEXTURE_SIZE));

        // colision on vertical rayHitInfo.hitSide
        if (rayHitInfo.hitSide == 0)
        {
            if (rayDir.x < 0)
            {
                rayEndPos.x -= 0.001;
                if (!hitDoor) rayHitInfo.textureCol = TEXTURE_SIZE - rayHitInfo.textureCol - 1;
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
                if (!hitDoor) rayHitInfo.textureCol = TEXTURE_SIZE - rayHitInfo.textureCol - 1;
            }
            else
            {
                rayEndPos.y -= 0.001;
            }
        }

        if (this->drawMap) this->fovVisualization[x + 1].position = rayEndPos;
        
        rayHitInfo.textureId = mapManager->getCellId(MAP_WALL_LAYER, mapManager->getCellPos(rayEndPos));

        this->raysHitInfos[x] = rayHitInfo;
    }
}

void RayCasting::castFloor(std::shared_ptr<MapManager> mapManager, const Vector2f playerPos, const Vector2f playerDir)
{
    double rayDirX0 = playerDir.x - this->plane.x;
    double rayDirY0 = playerDir.y - this->plane.y;
    double rayDirX1 = playerDir.x + this->plane.x;
    double rayDirY1 = playerDir.y + this->plane.y;

    for(unsigned short y = 0; y < SCREEN_HEIGHT / 2; y++)
    {
        // Current y position compared to the center of the screen (the horizon)
        short p = y - SCREEN_HEIGHT / 2;

        // Vertical position of the camera.
        double posZ = 0.5 * this->screeDist;

        // Horizontal distance from the camera to the floor for the current row.
        // 0.5 is the z position exactly in the middle between floor and ceiling.
        double rowDistance = posZ / p;

        Vector2d floorStep;
        floorStep.x = rowDistance * (rayDirX0 - rayDirX1) / SCREEN_WIDTH;
        floorStep.y = rowDistance * (rayDirY0 - rayDirY1) / SCREEN_WIDTH;

        Vector2d floorPos;
        floorPos.x = playerPos.x - rowDistance * rayDirX1;
        floorPos.y = playerPos.y - rowDistance * rayDirY1;

        for(int x = 0; x < SCREEN_WIDTH; ++x)
        {
            sf::Vector2i cellPos;
            cellPos.x = (short)(floorPos.x);
            cellPos.y = (short)(floorPos.y);

            Vector2d deltaCell;
            deltaCell.x = (floorPos.x - cellPos.x);
            deltaCell.y = (floorPos.y - cellPos.y);

            short floorCollideCellId = mapManager->getCellId(MAP_FLOOR_LAYER, cellPos);
            short ceilingCollideCellId = mapManager->getCellId(MAP_CEILING_LAYER, cellPos);

            if (floorCollideCellId != 0)
            {
                this->textureManager->drawPixel(Vector2f(x, SCREEN_HEIGHT - y - 1), this->textureManager->getPixel(floorCollideCellId, Vector2i(deltaCell.x * TEXTURE_SIZE, deltaCell.y * TEXTURE_SIZE)));
            }
            if (floorCollideCellId != 0)
            {
                this->textureManager->drawPixel(Vector2f(x, y), this->textureManager->getPixel(ceilingCollideCellId, Vector2i(deltaCell.x * TEXTURE_SIZE, deltaCell.y * TEXTURE_SIZE)));
            }

            floorPos.x -= floorStep.x;
            floorPos.y -= floorStep.y;
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
    
    this->castFloor(mapManager, playerPos, playerDir);
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