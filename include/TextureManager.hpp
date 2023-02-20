#pragma once


#include "Utils.hpp"


class TextureManager
{
    private:
        // Sprite
        std::map<unsigned short, sf::Texture> textures;
        std::map<unsigned short, sf::Sprite> sprites;

        // Variables
       
        // Private functions
        
    public:
        // Constructor
        TextureManager();
        ~TextureManager();

        // Accesors

        // Public functions
        void loadAllTextures();
        void renderTextureLine(std::shared_ptr<sf::RenderTarget> renderTarget, unsigned short textureId, sf::Vector2f pos, unsigned short columnX, unsigned short width, double height, bool addShadows);
        void render(std::shared_ptr<sf::RenderTarget> renderTarget, unsigned short textureId, sf::Vector2f pos);
};