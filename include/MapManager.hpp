#pragma once

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
        Vector2i getCellPos(Vector2f pos);
        short getCellId(std::string layer, Vector2f pos);
        float getDoorOpeningState(Vector2i cellPos);

        // Public functions
        void loadMap(std::shared_ptr<Player> player);
        
        bool chekPointCollision(Vector2f pos, bool checkDoorCollision = false);
        bool chekRectCollision(Vector2f pos, sf::FloatRect rect, bool checkDoorCollision = false);
        Vector2f getNewPosition(Vector2f pos, Vector2f currentPos, sf::FloatRect rect);

        void openDoor(Vector2i doorCellPos);
        
        void update(std::shared_ptr<Player> player);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};