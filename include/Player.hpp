#pragma once

#include "Utils.hpp"
#include "RayCasting.hpp"
#include "MapManager.hpp"

class MapManager;
class RayCasting;

class Player
{
    private:
        // Sprite
        sf::CircleShape sprite;

        // Variables
        sf::Vector2f pos;
        sf::Vector2f dir;

        float moveSpeed;
        float rotationSpeed;
        short actionRange;

        // Private functions
        void initSprite();
        void initVariables();

        void checkInputs(std::shared_ptr<RayCasting> rayCastingEngine, sf::Vector2i mousePos);
        void move(std::shared_ptr<MapManager> mapManager);
        void checkActions(std::shared_ptr<MapManager> mapManager);

        void renderPlayerDirection(std::shared_ptr<sf::RenderTarget> renderTarget);

    public:
        // Constructor
        Player();
        ~Player();

        // Accesors
        const sf::Vector2f getPos() const;
        const sf::Vector2f getCenterPos() const;
        const sf::Vector2f getDir() const;
        const sf::FloatRect getHitbox() const;

        // Public functions
        void setCenterPos(sf::Vector2f pos);
        
        void update(std::shared_ptr<MapManager> mapManager, std::shared_ptr<RayCasting> rayCasting, sf::Vector2i mousePos);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};