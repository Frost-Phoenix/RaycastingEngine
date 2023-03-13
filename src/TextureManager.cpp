#include "TextureManager.hpp"


// Constructor
TextureManager::TextureManager()
{
    this->screenBuffer.create(SCREEN_WIDTH, SCREEN_HEIGHT, sf::Color(255, 255, 255, 0));
    this->screenTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    this->screen.setTexture(this->screenTexture);

    this->loadAllTextures();
}

TextureManager::~TextureManager()
{

}

// Private functions

// Accesors
const sf::Color& TextureManager::getPixel(const unsigned short textureId, const Vector2i &pos)
{
    return this->images[textureId][pos.x + pos.y * TEXTURE_SIZE];
}

// Public functions
void TextureManager::loadAllTextures()
{
    std::ifstream jsFile("data/images/textures/path.json");
    Json::Value jsonReader;
    Json::Reader reader;

    reader.parse(jsFile, jsonReader);

    for (Json::Value& texture_info : jsonReader["paths"])
    {
        unsigned short id = static_cast<short>(texture_info["index"].asInt());
        std::string type = texture_info["type"].asString();
        std::string path = texture_info["path"].asString();

        if (type == "floor" || type == "ceiling")
        {
            sf::Image image;
            image.loadFromFile(path);

            for (int x = 0; x < TEXTURE_SIZE; x++)
            {
                for (int y = 0; y < TEXTURE_SIZE; y++)
                {
                    this->images[id][y * TEXTURE_SIZE + x] = image.getPixel(x, y);
                }
            }
        }
        else if (type == "wall" || type == "door")
        {
            this->textures[id].loadFromFile(path);
            this->sprites[id].setTexture(this->textures[id]);
        }
    }
}

void TextureManager::drawPixel(const Vector2f &pos, const sf::Color &color)
{
    this->screenBuffer.setPixel(pos.x, pos.y, color);
}

void TextureManager::renderTextureLine(std::shared_ptr<sf::RenderTarget> renderTarget, const unsigned short textureId, const Vector2f &pos, const unsigned short columnX, const double height, const bool addShadows)
{
    this->sprites[textureId].setPosition(pos);
    this->sprites[textureId].setTextureRect(sf::IntRect(columnX, 0, 1, TEXTURE_SIZE));
    this->sprites[textureId].setScale(1, height / static_cast<double>(TEXTURE_SIZE));
    
    if (addShadows) this->sprites[textureId].setColor(sf::Color(175, 175, 175));

    renderTarget->draw(this->sprites[textureId]);
    
    if (addShadows) this->sprites[textureId].setColor(sf::Color(255, 255, 255));
}

void TextureManager::renderTexture(std::shared_ptr<sf::RenderTarget> renderTarget, const unsigned short textureId, const Vector2f &pos)
{
    this->sprites[textureId].setPosition(pos);
    this->sprites[textureId].setTextureRect(sf::IntRect(0, 0, TEXTURE_SIZE, TEXTURE_SIZE));
    this->sprites[textureId].setScale(1, 1);

    renderTarget->draw(this->sprites[textureId]);
}

void TextureManager::renderScreenBuffer(std::shared_ptr<sf::RenderTarget> renderTarget)
{
    this->screenTexture.update(this->screenBuffer.getPixelsPtr());

    renderTarget->draw(this->screen);
}