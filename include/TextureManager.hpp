#pragma once


#include "Utils.hpp"


class TextureManager
{
    private:
        // Sprite
        std::map<unsigned short, sf::Texture> textures;
        std::map<unsigned short, sf::Sprite> sprites;

        sf::Image screenBuffer;
        sf::Texture screenTexture;
        sf::Sprite screen;

        // Variables
       
        // Private functions
        
    public:
        // Constructor
        TextureManager();
        ~TextureManager();

        // Accesors

        // Public functions
        void loadAllTextures();
        void drawPixel(int x, int y, sf::Color color);
        void renderTextureLine(std::shared_ptr<sf::RenderTarget> renderTarget, unsigned short textureId, Vector2f pos, unsigned short columnX, double height, bool addShadows);
        void renderTexture(std::shared_ptr<sf::RenderTarget> renderTarget, unsigned short textureId, Vector2f pos);
        void renderScreenBuffer(std::shared_ptr<sf::RenderTarget> renderTarget);
};