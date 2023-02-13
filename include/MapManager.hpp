#pragma once

#include "pch.hpp"
#include "Utils.hpp"
#include "Player.hpp"


class Player;


class MapManager
{
    private:
        // Textures
        sf::Texture wallCellTexture;
        sf::Texture verticalDoorCellTexture;
        sf::Texture horizontalDoorCellTexture;
        
        // Sprite
        sf::Sprite wallCellSprite;
        sf::Sprite verticalDoorCellSprite;
        sf::Sprite horizontalDoorCellSprite;

        // Variables
        std::map<std::string, std::vector<std::vector<unsigned short>>> map;
        unsigned short mapHeight;
        unsigned short mapWidth;

        // Private functions
        void initSprites();
        void initVariables();

        void loadMapFromJson(std::shared_ptr<Player> player);

        void renderMap(std::shared_ptr<sf::RenderTarget> renderTarget);

    public:
        // Constructor
        MapManager();
        ~MapManager();

        // Accesors
        unsigned short getMapWidth();
        unsigned short getMapHeight();
        short getCellId(std::string layer, sf::Vector2f pos);
        bool chekPointCollision(sf::Vector2f pos);
        bool chekRectCollision(sf::Vector2f pos, sf::FloatRect rect);
        sf::Vector2f getNewPosition(sf::Vector2f pos, sf::Vector2f currentPos, sf::FloatRect rect);

        // Public functions
        void loadMap(std::shared_ptr<Player> player);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};