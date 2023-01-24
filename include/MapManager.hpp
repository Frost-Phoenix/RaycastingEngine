#pragma once

#include "pch.hpp"
#include "Utils.hpp"

// Ne peut pas metre #includde "Player.hpp" car sinon le compileur tourne en rond dans les include (donc erreur)
// car MapManager inclue Player et Player inclue MapManager
class Player;

#define MAP_AS_JSON


class MapManager
{
    private:
        // Sprite
        sf::Texture cellTexture;
        sf::Sprite cellSprite;
        sf::Texture emptyCellTexture;
        sf::Sprite emptyCellSprite;

        // Variables
        std::map<std::string, std::vector<std::vector<unsigned short>>> map;
        unsigned short mapHeight;
        unsigned short mapWidth;

        // Private functions
        void initSprite();
        void initVariables();

        void loadMapFromJson(std::shared_ptr<Player> player);

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
        void loadMap(std::shared_ptr<Player> player);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};