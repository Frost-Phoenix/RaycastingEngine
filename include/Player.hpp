#pragma once

#include "pch.hpp"
#include "Utils.hpp"
#include "MapManager.hpp"

class MapManager;

class Player
{
    private:
        // Sprite
        sf::CircleShape sprite;

        // Variables
        sf::Vector2f pos;

        short moveSpeed;
        short rotationSpeed;
        
        short rotation;
        short actionRange;

        // Private functions
        void initSprite();
        void initVariables();

        void checkInputs();
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
        const short getAngle() const;
        const sf::FloatRect getHitbox() const;

        // Public functions
        void setCenterPos(sf::Vector2f pos);
        
        void update(std::shared_ptr<MapManager> mapManager);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};