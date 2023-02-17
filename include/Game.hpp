#pragma once

#include "pch.hpp"
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
        std::unique_ptr<RayCasting> rayCastingEngine;
        bool drawMap;
        bool showMiniMap;

        // Private functions
        void initWindow();
        void initVariables();

        void pollEvents();
        void updateDeltaTime();

        void updateMouse();
        void drawMiniMap();
        void drawFloor();

    public:
        // Constructor 
        Game();
        ~Game();

        // Accesors
        const bool isRunning() const;

        // Public functions
        void update();
        void render();
};