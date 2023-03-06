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
        std::map<unsigned short, std::vector<unsigned short>> map;
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
        Vector2i getCellPos(const Vector2f pos);
        short getCellId(unsigned short layerId, const Vector2i pos);
        float getDoorOpeningState(const Vector2i cellPos);

        // Public functions
        void loadMap(std::shared_ptr<Player> player);
        
        bool chekPointCollision(const Vector2f pos, const bool checkDoorCollision = false);
        bool chekRectCollision(const Vector2f pos, const sf::FloatRect rect, const bool checkDoorCollision = false);
        Vector2f getNewPosition(const Vector2f nextPos, const Vector2f currentPos, const sf::FloatRect rect);

        void openDoor(Vector2i doorCellPos);
        
        void update(std::shared_ptr<Player> player);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};