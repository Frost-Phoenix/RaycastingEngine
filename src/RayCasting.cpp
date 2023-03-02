#include "RayCasting.hpp"


// Constructor
RayCasting::RayCasting(bool drawMap)
{
    this->plane = sf::Vector2f(0, 0.7);
    this->screeDist = (SCREEN_WIDTH / 2) / std::tan(decToRad(FOV / 2));
    
    this->drawMap = drawMap;
    if (drawMap) this->fovVisualization = sf::VertexArray(sf::TriangleFan, SCREEN_WIDTH + 1);

    this->textureManager = std::unique_ptr<TextureManager>(new TextureManager());
}

RayCasting::~RayCasting()
{
    
}

void RayCasting::castWalls(std::shared_ptr<MapManager> mapManager, const sf::Vector2f playerPos, const sf::Vector2i playerCellPos, const sf::Vector2f playerDir)
{
    RayHitInfo rayHitInfo;
    sf::Vector2i cellPos;

    for (unsigned short x = 0; x < SCREEN_WIDTH; x++)
    {
        //calculate ray position and direction
        double cameraX = 2 * (SCREEN_WIDTH - x) / (double)SCREEN_WIDTH - 1; //x-coordinate in camera space

        sf::Vector2f rayDir;
        rayDir.x = playerDir.x + this->plane.x * cameraX;
        rayDir.y = playerDir.y + this->plane.y * cameraX;

        //which box of the map we're in
        cellPos = playerCellPos;

        //length of ray from one x or y-sided to next x or y-sided
        sf::Vector2f deltaDist;
        deltaDist.x = (rayDir.x == 0) ? 1e30 : std::abs(1 / rayDir.x);
        deltaDist.y = (rayDir.y == 0) ? 1e30 : std::abs(1 / rayDir.y);

        //what direction to step in x or y-direction (either +1 or -1)
        sf::Vector2i step;
        sf::Vector2f sideDist;

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
            if (mapManager->chekPointCollision(sf::Vector2f(cellPos.x * CELL_SIZE, cellPos.y * CELL_SIZE))) break;
        }

        //Calculate distance of perpendicular ray (Euclidean distance would give fisheye effect!)
        if (rayHitInfo.hitSide == 0) rayHitInfo.length = (sideDist.x - deltaDist.x);
        else rayHitInfo.length = (sideDist.y - deltaDist.y);

        sf::Vector2f rayEndPos;
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

void RayCasting::renderWalls(std::shared_ptr<sf::RenderWindow> renderTarget)
{
    for (unsigned short x = 0; x < SCREEN_WIDTH; x++)
    {
        RayHitInfo rayHitInfo = this->raysHitInfos[x];
        
        //Calculate height of line to draw on screen
        short lineHeight = (this->screeDist / rayHitInfo.length);

        //calculate lowest pixel to fill in current stripe
        short drawStart = (SCREEN_HEIGHT / 2 - lineHeight / 2);

        this->textureManager->renderTextureLine(renderTarget, rayHitInfo.textureId, sf::Vector2f(x, drawStart), rayHitInfo.textureCol, lineHeight, rayHitInfo.hitSide == 0);
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

void RayCasting::update(std::shared_ptr<MapManager> mapManager, sf::Vector2f playerPos, const sf::Vector2f playerDir)
{
    if (this->drawMap) this->fovVisualization[0].position = sf::Vector2f(playerPos.x, playerPos.y);

    sf::Vector2i playerCellPos = mapManager->getCellPos(playerPos);
    playerPos = sf::Vector2f(playerPos.x / CELL_SIZE, playerPos.y / CELL_SIZE);
    
    this->castWalls(mapManager, playerPos, playerCellPos, playerDir);
}

void RayCasting::renderFovVisualisation(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    renderTarget->draw(this->fovVisualization);
}

void RayCasting::render(std::shared_ptr<sf::RenderWindow> renderTarget)
{
    this->renderWalls(renderTarget);
}