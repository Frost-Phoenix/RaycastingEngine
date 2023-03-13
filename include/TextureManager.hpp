#pragma once


#include "Utils.hpp"


class TextureManager
{
    private:
        // Sprite
        std::map<unsigned short, std::array<sf::Color, TEXTURE_SIZE * TEXTURE_SIZE>> images;
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
        const sf::Color& getPixel(const unsigned short textureId, const Vector2i& pos);

        // Public functions
        void loadAllTextures();
        void drawPixel(const Vector2f &pos, const sf::Color &color);
        void renderTextureLine(std::shared_ptr<sf::RenderTarget> renderTarget, const unsigned short textureId, const Vector2f &pos, const unsigned short columnX, const double height, const bool addShadows);
        void renderTexture(std::shared_ptr<sf::RenderTarget> renderTarget, const unsigned short textureId, const Vector2f &pos);
        void renderScreenBuffer(std::shared_ptr<sf::RenderTarget> renderTarget);
};