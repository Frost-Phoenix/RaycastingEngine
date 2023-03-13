#include "TextRenderer.hpp"


// Constructor
TextRenderer::TextRenderer(const std::string fontPath, const unsigned short size)
{
    this->font.loadFromFile(fontPath);

    this->text.setFont(this->font);
    this->text.setCharacterSize(size);
}

TextRenderer::~TextRenderer()
{
    
}

// Private functions

// Accesors

// Public functions
void TextRenderer::setTextSize(const unsigned short size)
{
    this->text.setCharacterSize(size);
}

void TextRenderer::setTextColor(const sf::Color& color)
{
    this->text.setFillColor(color);
    this->text.setOutlineColor(color);
}

void TextRenderer::setTextPosition(const Vector2f& pos)
{
    this->text.setPosition(pos);
}

void TextRenderer::renderTxt(std::shared_ptr<sf::RenderTarget> renderTarget, const std::string& txt)
{
    this->text.setString(txt);
    
    renderTarget->draw(this->text);
}
