#pragma once

#include "Utils.hpp"
#include "MapManager.hpp"
#include "TextureManager.hpp"

class MapManager;

struct RayHitInfo
{
    bool hitSide;
    double length;
    unsigned short textureId;
    unsigned short textureCol;
};

class RayCasting
{
    private:
        // Sprite for fov visualization
        sf::VertexArray fovVisualization;

        // Texture manager
        std::unique_ptr<TextureManager> textureManager;

        // Variables
        bool drawMap;
        double screeDist;
        sf::Vector2f plane; // projection plane
        std::array<RayHitInfo, SCREEN_WIDTH> raysHitInfos;

        // Private functions
        void castWalls(std::shared_ptr<MapManager> mapManager, const sf::Vector2f playerPos, const sf::Vector2i playerCellPos, const sf::Vector2f playerDir);

        void renderWalls(std::shared_ptr<sf::RenderWindow> renderTarget);
        
    public:
        // Constructor
        RayCasting(bool drawMap = false);
        ~RayCasting();

        // Accesors

        // Public functions
        void toogleFovDrawing(const bool state);
        void rotatePlane(const double rotationSpeed);

        void update(std::shared_ptr<MapManager> mapManager, sf::Vector2f playerPos, const sf::Vector2f playerDir);

        void renderFovVisualisation(std::shared_ptr<sf::RenderTarget> renderTarget);
        void render(std::shared_ptr<sf::RenderWindow> renderTarget);
};