#pragma once

#include "pch.hpp"
#include "Utils.hpp"
#include "MapManager.hpp"


class RayCasting
{
    private:
        // Sprite
        sf::VertexArray fovVisualization;
        
        // Variables
        bool drawMap;
        std::array<double, NB_RAY> raysLength;

        // Private functions
        double RayDepthHorizontalCollison(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, double angle);
        double RayDepthVerticalCollison(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, double angle);
        double castRay(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, double angle);

    public:
        // Constructor
        RayCasting(bool drawMap = false);
        ~RayCasting();

        // Accesors

        // Public functions
        void toogleFovDrawing(bool state);
        
        void renderFovVisualisation(std::shared_ptr<sf::RenderTarget> renderTarget);
        void update(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, short baseAngle);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};