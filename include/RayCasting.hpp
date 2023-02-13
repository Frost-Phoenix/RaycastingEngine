#pragma once

#include "pch.hpp"
#include "Utils.hpp"
#include "MapManager.hpp"
#include "TextureManager.hpp"


struct RayHitInfo
{
    double length;
    unsigned short textureId;
    unsigned short textureCol;
};

class RayCasting
{
    private:
        // Sprite
        sf::VertexArray fovVisualization;

        // Texure manager
        std::unique_ptr<TextureManager> textureManager;

        // Variables
        bool drawMap;
        std::array<RayHitInfo, NB_RAY> raysHitInfos;

        // Private functions
        RayHitInfo castHorizontalRay(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, double angle);
        RayHitInfo castVerticalRay(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, double angle);
        RayHitInfo castRay(std::shared_ptr<MapManager> mapManager, sf::Vector2f pos, double angle);

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