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
        
        // Frame rate independency
        sf::Clock clock;

        // Player / MapManager
        std::unique_ptr<Player> player;
        std::shared_ptr<MapManager> mapManager;

        // Raycating engine
        std::unique_ptr<RayCasting> rayCastingEngine;
        bool drawMap;

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
        
        // Public functions
        void update();
        void render();
};