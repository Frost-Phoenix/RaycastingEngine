#pragma once

#include "pch.hpp"
#include "Utils.hpp"
#include "MapManager.hpp"


class Player
{
    private:
        // Sprite
        sf::Texture texture;
        sf::CircleShape sprite;

        // Variables
        // float deltaTime;
        sf::Vector2f pos;

        short moveSpeed;
        short rotationSpeed;
        
        short rotation;

        // Private functions
        void initSprite();
        void initVariables();

        void checkInputs();
        void move(std::shared_ptr<MapManager> mapManager);

        void renderPlayerDirection(std::shared_ptr<sf::RenderTarget> renderTarget);

    public:
        // Constructor
        Player();
        ~Player();

        // Accesors
        const sf::Vector2f getPos() const;
        const sf::Vector2f getCenterPos() const;
        const short getAngle() const;

        // Public functions
        void update(std::shared_ptr<MapManager> mapManager);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};