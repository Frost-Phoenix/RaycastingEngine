#pragma once


#include "Utils.hpp"


class TextRenderer
{
    private:
        // Sprite
        sf::Font font;
        sf::Text text;

        // Variables
       
        // Private functions
        
    public:
        // Constructor
        TextRenderer(const std::string fontPath = "data/font/upheavtt.ttf", const unsigned short& size = 20);
        ~TextRenderer();

        // Accesors

        // Public functions
        void setTextSize(const unsigned short& size);
        void setTextColor(const sf::Color& color);
        void setTextPosition(const sf::Vector2f& pos);
        
        void renderTxt(std::shared_ptr<sf::RenderTarget> renderTarget, const std::string& txt);
};