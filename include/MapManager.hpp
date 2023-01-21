#pragma once

#include "pch.hpp"
#include "Utils.hpp"


class MapManager
{
    private:
        // Sprite
        sf::Texture cellTexture;
        sf::Sprite cellSprite;
        sf::Texture emptyCellTexture;
        sf::Sprite emptyCellSprite;

        // Variables
        std::array<std::array<int, MAP_WIDTH>, MAP_HEIGHT> map;

        // Private functions
        void initSprite();
        void initVariables();

        void loadMapFromImage();

        void renderMap(std::shared_ptr<sf::RenderTarget> renderTarget);

    public:
        // Constructor
        MapManager();
        ~MapManager();

        // Accesors
        bool chekPointCollision(sf::Vector2f pos);
        bool chekRectCollision(sf::Vector2f pos, sf::FloatRect rect);
        sf::Vector2f getNewPosition(sf::Vector2f pos, sf::Vector2f currentPos, sf::FloatRect rect);

        // Public functions
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};