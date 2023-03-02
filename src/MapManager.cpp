#include "MapManager.hpp"


// Constructor
MapManager::MapManager()
{
    this->initSprites();
    this->initVariables();
}

MapManager::~MapManager()
{
    
}

// Private functions
void MapManager::initSprites()
{
    this->wallCellTexture.loadFromFile("data/images/map_cells/wall.png");
    this->verticalDoorCellTexture.loadFromFile("data/images/map_cells/vertical_door.png");
    this->horizontalDoorCellTexture.loadFromFile("data/images/map_cells/horizontal_door.png");
    
    this->wallCellSprite.setTexture(wallCellTexture);
    this->verticalDoorCellSprite.setTexture(verticalDoorCellTexture);
    this->horizontalDoorCellSprite.setTexture(horizontalDoorCellTexture);
}

void MapManager::initVariables()
{
    this->mapHeight = 0;
    this->mapWidth = 0;
}

void MapManager::loadMapFromJson(std::shared_ptr<Player> player)
{
    std::ifstream jsFile("data/map/level_test.json");
    Json::Value jsonReader;
    Json::Reader reader;

    reader.parse(jsFile, jsonReader);

    this->mapHeight = static_cast<short>(jsonReader["height"].asInt());
    this->mapWidth = static_cast<short>(jsonReader["width"].asInt());
    
    for (Json::Value& layer : jsonReader["layers"])
    {
        std::string layerName = layer["name"].asString();
        this->map[layerName].resize(this->mapHeight);

        for (unsigned short row = 0; row < this->mapHeight; row++)
        {
            for (unsigned short col = 0; col < this->mapWidth; col++)
            {
                if (layerName != "player")
                {
                    this->map[layerName][row].push_back(static_cast<short>(layer["data"][row * this->mapWidth + col].asInt()));

                    if (this->map[layerName][row][col] == MAP_VERTICAL_DOOR_ID || this->map[layerName][row][col] == MAP_HORIZONTAL_DOOR_ID)
                    {
                        DoorInfo doorInfo;
                        
                        doorInfo.isOpen = false;
                        doorInfo.isOpening = false;
                        doorInfo.isClose = true;
                        doorInfo.isClosing = false;
                        doorInfo.isOpen = 0;
                        doorInfo.maxOpenTime = 100;
                        doorInfo.movingSpeed = 0.85;
                        doorInfo.openingState = 0;
                        
                        this->doors[{row, col}] = doorInfo;
                    }
                }
                else
                {
                    if (layer["data"][row * this->mapWidth + col].asInt() == MAP_PLAYER_ID)
                    {
                        player->setCenterPos(sf::Vector2f(col * CELL_SIZE + CELL_SIZE / 2, row * CELL_SIZE + CELL_SIZE / 2));
                    }
                }
            }
        }
    }
}

// Accesors
unsigned short MapManager::getMapWidth()
{
    return this->mapWidth;
}

unsigned short MapManager::getMapHeight()
{
    return this->mapHeight;
}

sf::Vector2i MapManager::getCellPos(sf::Vector2f pos)
{
    return sf::Vector2i(static_cast<int>(pos.x / CELL_SIZE), static_cast<int>(pos.y / CELL_SIZE));
}

short MapManager::getCellId(std::string layer, sf::Vector2f pos)
{
    sf::Vector2i cellPos = this->getCellPos(pos);

    if (0 <= cellPos.x && this->mapWidth > cellPos.x && 0 <= cellPos.y && this->mapHeight > cellPos.y)
    {
        unsigned short id = this->map[layer][cellPos.y][cellPos.x];
        return id;
    }
    
    return -1;
}

/* Check if a point is in a non walkables cell */
bool MapManager::chekPointCollision(sf::Vector2f pos, bool checkDoorCollision)
{
    unsigned short cellId = this->getCellId("collision", pos);

    if (cellId != 0)
    {
        if (checkDoorCollision)
        {
            return cellId == MAP_VERTICAL_DOOR_ID || cellId == MAP_HORIZONTAL_DOOR_ID;
        }
        else
        {
            if (cellId == MAP_WALL_ID)
            {
                return true;
            } 
            else if (cellId == MAP_VERTICAL_DOOR_ID || cellId == MAP_HORIZONTAL_DOOR_ID)
            {
                sf::Vector2i cellPos = this->getCellPos(pos);

                if (this->doors[{cellPos.y, cellPos.x}].isOpen == false)
                {
                    return true;
                }   
            }
        }
    }

    return false;
}

/* Check if a rect is on walkables cells */
bool MapManager::chekRectCollision(sf::Vector2f pos, sf::FloatRect rect, bool checkDoorCollision)
{
    // Check th four corner of the rect
    
    // Corner top-left
    if (this->chekPointCollision(pos, checkDoorCollision) == true) return true;
    // Corner top-right
    if (this->chekPointCollision(sf::Vector2f(pos.x + rect.width, pos.y), checkDoorCollision) == true) return true;
    // Corner botom-left
    if (this->chekPointCollision(sf::Vector2f(pos.x, pos.y + rect.height), checkDoorCollision) == true) return true;
    // Corner botom-right
    if (this->chekPointCollision(sf::Vector2f(pos.x + rect.width, pos.y + rect.height), checkDoorCollision) == true) return true;

    return false;
}

/* Get position of a rect at close as possible of the non walkables cells */
sf::Vector2f MapManager::getNewPosition(sf::Vector2f nextPos, sf::Vector2f currentPos, sf::FloatRect rect)
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
void MapManager::loadMap(std::shared_ptr<Player> player)
{
    this->loadMapFromJson(player);
}

float MapManager::getDoorOpeningState(sf::Vector2i cellPos)
{
    return this->doors[{cellPos.y, cellPos.x}].openingState;
}

void MapManager::openDoor(sf::Vector2i doorCellPos)
{
    DoorInfo& door = this->doors[{doorCellPos.y, doorCellPos.x}];
    
    if (door.isClose || door.isClosing)
    {
        door.isClose = false;
        door.isClosing = false;
        door.isOpening = true;

        door.openingState += door.movingSpeed;
    }
}

void MapManager::updateDoors(std::shared_ptr<Player> player)
{
    for (auto& [pos, door] : this->doors)
    {
        if (door.isOpening)
        {
            door.openingState += door.movingSpeed;

            if (door.openingState >= CELL_SIZE)
            {
                door.openingState = CELL_SIZE;
                door.isOpening = false;
                door.isOpen = true;
            }
        }
        else if (door.isOpen)
        {
            door.openTime += 1;
            
            if (door.openTime >= door.maxOpenTime && this->chekRectCollision(player->getPos(), player->getHitbox(), true) == false)
            {
                door.openTime = 0;
                door.isOpen = false;
                door.isClosing = true;
            }
        }
        else if (door.isClosing)
        {
            door.openingState -= door.movingSpeed;

            if (door.openingState <= 0)
            {
                door.openingState = 0;
                door.isClosing = false;
                door.isClose = true;
            }
        }
    }
}

void MapManager::renderMap(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    for (unsigned char row = 0; row < this->mapHeight; row++)
    {
        for (unsigned char col = 0; col < this->mapWidth; col++)
        {
            switch (this->map["collision"][row][col])
            {
                case MAP_WALL_ID:
                    this->wallCellSprite.setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
                    renderTarget->draw(this->wallCellSprite);
                    break;

                case MAP_HORIZONTAL_DOOR_ID:
                    if (this->doors[{row, col}].isOpen == false)
                    {
                        this->horizontalDoorCellSprite.setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
                        renderTarget->draw(this->horizontalDoorCellSprite);
                    }
                    break;

                case MAP_VERTICAL_DOOR_ID:
                    if (this->doors[{row, col}].isOpen == false)
                    {
                        this->verticalDoorCellSprite.setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));
                        renderTarget->draw(this->verticalDoorCellSprite);
                    }
                    break;
                
                default:
                    break;
            }
        }
    }
}

void MapManager::update(std::shared_ptr<Player> player)
{
    this->updateDoors(player);
}

void MapManager::render(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    this->renderMap(renderTarget);
}
