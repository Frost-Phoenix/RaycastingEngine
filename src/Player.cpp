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
    this->rotation = 270;
    this->actionRange = 25;

    this->moveSpeed = 2;
    this->rotationSpeed = 2;
}

void Player::checkInputs()
{
    // Player rotaion / view 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  this->rotation = get_degrees(this->rotation - this->rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) this->rotation = get_degrees(this->rotation + this->rotationSpeed);
}

void Player::move(std::shared_ptr<MapManager> mapManager)
{
    sf::Vector2f currentPos = this->sprite.getPosition();
    sf::Vector2f newPos = currentPos;

    double cosSpeed = this->moveSpeed * std::cos(decToRad(this->rotation));
    double sinSpeed = this->moveSpeed * std::sin(decToRad(this->rotation));
    double cosSpeedDiagonal = this->moveSpeed * std::cos(decToRad(this->rotation + 45));
    double sinSpeedDiagonal = this->moveSpeed * std::sin(decToRad(this->rotation + 45));

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
        sf::Vector2f doorPos((this->sprite.getPosition().x + this->sprite.getLocalBounds().width / 2) + this->actionRange * std::cos(decToRad(this->rotation)), (this->sprite.getPosition().y + this->sprite.getLocalBounds().width / 2) + this->actionRange * std::sin(decToRad(this->rotation)));
        
        if (MAP_VERTICAL_DOOR_ID == mapManager->getCellId("collision", doorPos) || MAP_HORIZONTAL_DOOR_ID == mapManager->getCellId("collision", doorPos))
        {
            sf::Vector2i doorCellPos = mapManager->getCellPos(doorPos);
            mapManager->openDoor(doorCellPos);
        }    
    }
}

// Accesors
const sf::Vector2f Player::getPos() const
{
    return this->sprite.getPosition();
}

const sf::Vector2f Player::getCenterPos() const
{
    return sf::Vector2f(this->sprite.getPosition().x + this->sprite.getLocalBounds().width / 2, this->sprite.getPosition().y + this->sprite.getLocalBounds().height / 2);
}

const short Player::getAngle() const
{
    return this->rotation;
}

const sf::FloatRect Player::getHitbox() const
{
    return this->sprite.getGlobalBounds();
}

// Public functions
void Player::setCenterPos(sf::Vector2f pos)
{
    sf::FloatRect playerHitbox = this->sprite.getGlobalBounds();
    this->sprite.setPosition(pos.x - (playerHitbox.width / 2), pos.y - (playerHitbox.height / 2));
}

void Player::update(std::shared_ptr<MapManager> mapManager)
{
    this->checkInputs();
    this->move(mapManager);
    this->checkActions(mapManager);
}

void Player::renderPlayerDirection(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    sf::VertexArray line(sf::LinesStrip, 2);
    line[0].position = sf::Vector2f(this->sprite.getPosition().x + this->sprite.getLocalBounds().width / 2, this->sprite.getPosition().y + this->sprite.getLocalBounds().height / 2);
    line[0].color = sf::Color::Red;
    line[1].position = sf::Vector2f((this->sprite.getPosition().x + this->sprite.getLocalBounds().width / 2) + this->actionRange * std::cos(decToRad(this->rotation)), (this->sprite.getPosition().y + this->sprite.getLocalBounds().width / 2) + this->actionRange * std::sin(decToRad(this->rotation)));
    line[1].color = sf::Color::Red;
    renderTarget->draw(line);
}

void Player::render(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    this->renderPlayerDirection(renderTarget);
    renderTarget->draw(this->sprite);
}
