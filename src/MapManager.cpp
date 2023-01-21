#include "MapManager.hpp"


// Constructor
MapManager::MapManager()
{
    this->initSprite();
    this->initVariables();
    this->loadMapFromImage();
}

MapManager::~MapManager()
{
    
}

// Private functions
void MapManager::initSprite()
{
    this->cellTexture.loadFromFile("data/images/cell.png");
    this->cellSprite.setTexture(this->cellTexture);
    this->emptyCellTexture.loadFromFile("data/images/empty_cell.png");
    this->emptyCellSprite.setTexture(this->emptyCellTexture);
}

void MapManager::initVariables()
{
    
}

void MapManager::loadMapFromImage()
{
    sf::Image mapSketch;
    mapSketch.loadFromFile("data/images/map.png");

    for (unsigned char row = 0; row < MAP_HEIGHT; row++)
    {
        for (unsigned char col = 0; col < MAP_WIDTH; col++)
        {
            sf::Color pixelColor = mapSketch.getPixel(col, row);

            if (pixelColor == sf::Color(0, 0, 0)) this->map[row][col] = 1;
            else this->map[row][col] = 0;
        }
    }
}

// Accesors
bool MapManager::chekPointCollision(sf::Vector2f pos)
/* Check is a point is in a non walkables cell */
{
    sf::Vector2i cellPos(static_cast<int>(pos.x / CELL_SIZE), static_cast<int>(pos.y / CELL_SIZE));

    // If cellPos in map 
    if (0 <= cellPos.x && MAP_WIDTH > cellPos.x && 0 <= cellPos.y && MAP_HEIGHT > cellPos.y)
    {
        // If cellPos is an non walkable cell
        if (this->map[cellPos.y][cellPos.x] == 1) return true;
    }

    return false;
}

bool MapManager::chekRectCollision(sf::Vector2f pos, sf::FloatRect rect)
/* Check if a rect is on walkables cells */
{
    // Check th four corner of the rect
    
    // Corner top-left
    if (this->chekPointCollision(pos) == true) return true;
    // Corner top-right
    if (this->chekPointCollision(sf::Vector2f(pos.x + rect.width, pos.y)) == true) return true;
    // Corner botom-left
    if (this->chekPointCollision(sf::Vector2f(pos.x, pos.y + rect.height)) == true) return true;
    // Corner botom-right
    if (this->chekPointCollision(sf::Vector2f(pos.x + rect.width, pos.y + rect.height)) == true) return true;

    return false;
}

sf::Vector2f MapManager::getNewPosition(sf::Vector2f nextPos, sf::Vector2f currentPos, sf::FloatRect rect)
/* Get position of a rect at close as possible of the non walkables cells */
{
    // if cannot directly move on both axes
    if (this->chekRectCollision(nextPos, rect) == true)
    {
        // Can move on x but not on y, so put the closest to wall on y
        if (this->chekRectCollision(sf::Vector2f(nextPos.x, currentPos.y), rect) == false)
        {
            // Bottom collide
            if (this->chekPointCollision(sf::Vector2f(nextPos.x, nextPos.y + rect.height)) || this->chekPointCollision(sf::Vector2f(nextPos.x + rect.width, nextPos.y + rect.height)))
            {
                nextPos.y = (CELL_SIZE * std::floor(currentPos.y / CELL_SIZE)) + (CELL_SIZE - rect.height - 0.01);
            }
            else nextPos.y = CELL_SIZE * std::floor(currentPos.y / CELL_SIZE);

        }
        // Can move on y but not on x, so put the closest to wall on x
        else if (this->chekRectCollision(sf::Vector2f(currentPos.x, nextPos.y), rect) == false)
        {
            // Right collide
            if (this->chekPointCollision(sf::Vector2f(nextPos.x + rect.width, nextPos.y)) || this->chekPointCollision(sf::Vector2f(nextPos.x + rect.width, nextPos.y + rect.height)))
            {
                nextPos.x = (CELL_SIZE * std::floor(currentPos.x / CELL_SIZE)) + (CELL_SIZE - rect.width - 0.01);
            }
            // Else left collide
            else nextPos.x = CELL_SIZE * std::floor(currentPos.x / CELL_SIZE);
        }
        // Can't move on any axes
        else
        {
            // Bottom collide
            if (this->chekPointCollision(sf::Vector2f(currentPos.x, nextPos.y + rect.height)) || this->chekPointCollision(sf::Vector2f(currentPos.x + rect.width, nextPos.y + rect.height)))
            {
                nextPos.y = (CELL_SIZE * std::floor(currentPos.y / CELL_SIZE)) + (CELL_SIZE - rect.height - 0.01);
            }
            // Else top collide
            else nextPos.y = CELL_SIZE * std::floor(currentPos.y / CELL_SIZE);

            // Right collide
            if (this->chekPointCollision(sf::Vector2f(nextPos.x + rect.width, currentPos.y)) || this->chekPointCollision(sf::Vector2f(nextPos.x + rect.width, currentPos.y + rect.height)))
            {
                nextPos.x = (CELL_SIZE * std::floor(currentPos.x / CELL_SIZE)) + (CELL_SIZE - rect.width - 0.01);
            }
            // Else left collide
            else nextPos.x = CELL_SIZE * std::floor(currentPos.x / CELL_SIZE);
        }
    }
    
    return nextPos;
}

// Public functions
void MapManager::renderMap(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    for (unsigned char row = 0; row < MAP_HEIGHT; row++)
    {
        for (unsigned char col = 0; col < MAP_WIDTH; col++)
        {
            if (this->map[row][col] == 1)
            {
                this->cellSprite.setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
                renderTarget->draw(this->cellSprite);
            }
            // else if (this->map[row][col] == 0)
            // {
            //     this->emptyCellSprite.setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
            //     renderTarget->draw(this->emptyCellSprite);   
            // }
        }
    }
}

void MapManager::render(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    this->renderMap(renderTarget);
}
