#pragma once

#include "pch.hpp"
#include "Utils.hpp"
#include "Player.hpp"


class Player;


struct DoorInfo
{
    bool isOpen;
    bool isOpening;
    bool isClose;
    bool isClosing;
    short openTime;
    float maxOpenTime;
    float movingSpeed;
    float openingState;
};


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
        std::map<std::tuple<unsigned short, unsigned short>, DoorInfo> doors;
        unsigned short mapHeight;
        unsigned short mapWidth;

        // Private functions
        void initSprites();
        void initVariables();

        void loadMapFromJson(std::shared_ptr<Player> player);

        void updateDoors(std::shared_ptr<Player> player);

        void renderMap(std::shared_ptr<sf::RenderTarget> renderTarget);

    public:
        // Constructor
        MapManager();
        ~MapManager();

        // Accesors
        unsigned short getMapWidth();
        unsigned short getMapHeight();
        sf::Vector2i getCellPos(sf::Vector2f pos);
        short getCellId(std::string layer, sf::Vector2f pos);

        // Public functions
        void loadMap(std::shared_ptr<Player> player);
        
        bool chekPointCollision(sf::Vector2f pos, bool checkDoorCollision = false);
        bool chekRectCollision(sf::Vector2f pos, sf::FloatRect rect, bool checkDoorCollision = false);
        sf::Vector2f getNewPosition(sf::Vector2f pos, sf::Vector2f currentPos, sf::FloatRect rect);

        void openDoor(sf::Vector2i doorCellPos);
        
        void update(std::shared_ptr<Player> player);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};