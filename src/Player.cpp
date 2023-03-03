#include "Player.hpp"

// Constructor
Player::Player()
{
    this->initSprite();
    this->initVariables();
}

Player::~Player()
{
    
}

// Private functions
void Player::initSprite()
{
    this->sprite.setRadius(10.f);
    this->sprite.setFillColor(sf::Color(125, 125, 125));
}

void Player::initVariables()
{
    this->dir = Vector2f(-1, 0);
    this->actionRange = 25;

    this->moveSpeed = 2;
    this->rotationSpeed = 0.05;
}

void Player::checkInputs(std::shared_ptr<RayCasting> rayCastingEngine, Vector2i mousePos)
{
    // Player rotaion / view 
    char turnDir = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) turnDir = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) turnDir = 1;

    if (turnDir != 0)
    {
        double oldDirX = this->dir.x;
        this->dir.x = this->dir.x * std::cos(this->rotationSpeed * turnDir) - this->dir.y * std::sin(this->rotationSpeed * turnDir);
        this->dir.y = oldDirX * std::sin(this->rotationSpeed * turnDir) + this->dir.y * std::cos(this->rotationSpeed * turnDir);

        rayCastingEngine->rotatePlane(this->rotationSpeed * turnDir);
    }
}

void Player::move(std::shared_ptr<MapManager> mapManager)
{
    Vector2f currentPos = this->sprite.getPosition();
    Vector2f newPos = currentPos;

    float playerSpeed = this->moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) playerSpeed *= 1.5;

    double cosSpeed = playerSpeed * this->dir.x;
    double sinSpeed = playerSpeed * this->dir.y;
    double cosSpeedDiagonal = playerSpeed * (this->dir.x * std::cos(decToRad(45)) - this->dir.y * std::sin(decToRad(45))); // cos(a + b) = cos a * cos b - sin a * sin b
    double sinSpeedDiagonal = playerSpeed * (this->dir.y * std::cos(decToRad(45)) + this->dir.x * std::sin(decToRad(45))); // sin(a + b) = sin a * cas b + cos a * sin b

    // Go up-right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        newPos.x += cosSpeedDiagonal;
        newPos.y += sinSpeedDiagonal;
    }
    // Go up-left
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        newPos.x += sinSpeedDiagonal;
        newPos.y -= cosSpeedDiagonal;
    }
    // Go down-left
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) 
    {
        newPos.x -= cosSpeedDiagonal;
        newPos.y -= sinSpeedDiagonal;
    }
    // Go down-right
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        newPos.x -= sinSpeedDiagonal;
        newPos.y += cosSpeedDiagonal;
    }
    // If player didn't move diagonaly
    if (newPos == currentPos)
    {
        // Go up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            newPos.x += cosSpeed;
            newPos.y += sinSpeed;
        }
        // Go left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            newPos.x += sinSpeed;
            newPos.y -= cosSpeed;
        }
        // Go down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
        {
            newPos.x -= cosSpeed;
            newPos.y -= sinSpeed;
        }
        // Go right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            newPos.x -= sinSpeed;
            newPos.y += cosSpeed;
        }
    }

    if (newPos != currentPos)
    {
        // Get the newPos with collision cheking
        newPos = mapManager->getNewPosition(newPos, currentPos, this->sprite.getGlobalBounds());

        this->sprite.setPosition(newPos);
    }
}

void Player::checkActions(std::shared_ptr<MapManager> mapManager)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        Vector2f doorPos((this->sprite.getPosition().x + this->sprite.getLocalBounds().width / 2) + this->actionRange * this->dir.x, (this->sprite.getPosition().y + this->sprite.getLocalBounds().width / 2) + this->actionRange * this->dir.y);
        
        if (MAP_VERTICAL_DOOR_ID == mapManager->getCellId("collision", doorPos) || MAP_HORIZONTAL_DOOR_ID == mapManager->getCellId("collision", doorPos))
        {
            Vector2i doorCellPos = mapManager->getCellPos(doorPos);
            mapManager->openDoor(doorCellPos);
        }    
    }
}

// Accesors
const Vector2f Player::getPos() const
{
    return this->sprite.getPosition();
}

const Vector2f Player::getCenterPos() const
{
    return Vector2f(this->sprite.getPosition().x + this->sprite.getLocalBounds().width / 2, this->sprite.getPosition().y + this->sprite.getLocalBounds().height / 2);
}

const Vector2f Player::getDir() const
{
    return this->dir;
}

const sf::FloatRect Player::getHitbox() const
{
    return this->sprite.getGlobalBounds();
}

// Public functions
void Player::setCenterPos(Vector2f pos)
{
    sf::FloatRect playerHitbox = this->sprite.getGlobalBounds();
    this->sprite.setPosition(pos.x - (playerHitbox.width / 2), pos.y - (playerHitbox.height / 2));
}

void Player::update(std::shared_ptr<MapManager> mapManager, std::shared_ptr<RayCasting> rayCastingEngine, Vector2i mousePos)
{
    this->checkInputs(rayCastingEngine, mousePos);
    this->move(mapManager);
    this->checkActions(mapManager);
}

void Player::renderPlayerDirection(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    sf::VertexArray line(sf::LinesStrip, 2);
    line[0].position = Vector2f(this->sprite.getPosition().x + this->sprite.getLocalBounds().width / 2, this->sprite.getPosition().y + this->sprite.getLocalBounds().height / 2);
    line[0].color = sf::Color::Red;
    line[1].position = Vector2f((this->sprite.getPosition().x + this->sprite.getLocalBounds().width / 2) + this->actionRange * this->dir.x, (this->sprite.getPosition().y + this->sprite.getLocalBounds().width / 2) + this->actionRange * this->dir.y);
    line[1].color = sf::Color::Red;
    renderTarget->draw(line);
}

void Player::render(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    this->renderPlayerDirection(renderTarget);
    renderTarget->draw(this->sprite);
}
