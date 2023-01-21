#pragma once

#include "pch.hpp"
#include "Utils.hpp"
#include "MapManager.hpp"


class RayCasting
{
    private:
        // Sprite
        // sf::VertexArray fovVisualization;
        sf::VertexArray fovVisualization;
        
        // Variables

        // Private functions
        float RayDepthHorizontalCollison(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, float angle);
        float RayDepthVerticalCollison(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, float angle);
        sf::Vector2f castRay(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, float angle);

    public:
        // Constructor
        RayCasting();
        ~RayCasting();

        // Accesors

        // Public functions
        void update(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, short baseAngle);
        void renderFovVisualisation(std::shared_ptr<sf::RenderTarget> renderTarget);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};