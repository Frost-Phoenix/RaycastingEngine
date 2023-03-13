#pragma once

#include "Utils.hpp"
#include "MapManager.hpp"
#include "TextureManager.hpp"
#include "TextRenderer.hpp"

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

        // Texure manager
        std::unique_ptr<TextureManager> textureManager;

        // Variables
        bool drawMap;
        double screenDist;
        Vector2f plane; // projection plane
        std::array<RayHitInfo, SCREEN_WIDTH> raysHitInfos;

        // Private functions
        void castWalls(std::shared_ptr<MapManager> mapManager, const Vector2f playerPos, const Vector2i playerCellPos, const Vector2f playerDir, const unsigned short startX, const unsigned short stopX);
        void castFloor(std::shared_ptr<MapManager> mapManager, const Vector2f playerPos, const Vector2f playerDir, const unsigned short startY, const unsigned short stopY);

        void renderFloor(std::shared_ptr<sf::RenderTarget> renderTarget);
        void renderWalls(std::shared_ptr<sf::RenderTarget> renderTarget);
        
    public:
        // Constructor
        RayCasting(bool drawMap = false);
        ~RayCasting();

        // Accesors

        // Public functions
        void toogleFovDrawing(const bool state);
        void rotatePlane(const double rotationSpeed);

        void update(std::shared_ptr<MapManager> mapManager, Vector2f playerPos, const Vector2f playerDir);

        void renderFovVisualisation(std::shared_ptr<sf::RenderTarget> renderTarget);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget);
};