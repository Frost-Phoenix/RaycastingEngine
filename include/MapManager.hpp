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
        std::array<std::vector<unsigned short>, NB_LAYERS> map;
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
        const unsigned short getMapWidth() const;
        const unsigned short getMapHeight() const;
        const Vector2i getCellPos(const Vector2f &pos) const;
        const short getCellId(const unsigned short layerId, const Vector2i &pos) const;
        const float getDoorOpeningState(const Vector2i& cellPos);

        // Public functions
        void loadMap(std::shared_ptr<Player> player);
        
        bool chekPointCollision(const Vector2f pos, const bool checkDoorCollision = false);
        bool chekRectCollision(const Vector2f pos, const sf::FloatRect rect, const bool checkDoorCollision = false);
        Vector2f getNewPosition(Vector2f &nextPos, const Vector2f &currentPos, const sf::FloatRect &rect);

        void openDoor(const Vector2i &doorCellPos);
        
        void update(std::shared_ptr<Player> player);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};