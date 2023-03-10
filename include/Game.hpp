#pragma once

#include "Utils.hpp"
#include "Player.hpp"
#include "MapManager.hpp"
#include "RayCasting.hpp"


class Game 
{
    private:
        // Window 
        std::shared_ptr<sf::RenderWindow> window;
        sf::Event events;
        bool isFocus;

        // Frame rate independency
        sf::Clock clock;

        // Player / MapManager
        std::shared_ptr<Player> player;
        std::shared_ptr<MapManager> mapManager;

        // Raycating engine
        std::shared_ptr<RayCasting> rayCastingEngine;
        bool drawMap;
        bool showMiniMap;

        // Private functions
        void initWindow();
        void initVariables();

        void pollEvents();
        void updateDeltaTime();

        void drawMiniMap();

    public:
        // Constructor 
        Game();
        ~Game();

        // Accesors
        const bool isRunning() const;
        const unsigned short getMousePosX() const;

        // Public functions
        void update();
        void render();
};