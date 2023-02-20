#include "TextureManager.hpp"


// Constructor
TextureManager::TextureManager()
{
    this->loadAllTextures();
}

TextureManager::~TextureManager()
{
    
}

// Private functions

// Accesors

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
        std::string path = texture_info["path"].asString();
        
        this->textures[id].loadFromFile(path);
        this->sprites[id].setTexture(this->textures[id]);
    }
}

void TextureManager::renderTextureLine(std::shared_ptr<sf::RenderTarget> renderTarget, unsigned short textureId, sf::Vector2f pos, unsigned short columnX, unsigned short width, double height, bool addShadows)
{
    this->sprites[textureId].setPosition(pos);
    this->sprites[textureId].setTextureRect(sf::IntRect(columnX, 0, 1, TEXTURE_SIZE));
    this->sprites[textureId].setScale(width, height / static_cast<double>(TEXTURE_SIZE));

    if (addShadows) this->sprites[textureId].setColor(sf::Color(175, 175, 175));;

    renderTarget->draw(this->sprites[textureId]);
    
    if (addShadows) this->sprites[textureId].setColor(sf::Color(255, 255, 255));;
}

void TextureManager::render(std::shared_ptr<sf::RenderTarget> renderTarget, unsigned short textureId, sf::Vector2f pos)
{
    this->sprites[textureId].setPosition(pos);
    this->sprites[textureId].setTextureRect(sf::IntRect(0, 0, TEXTURE_SIZE, TEXTURE_SIZE));
    this->sprites[textureId].setScale(1, 1);

    renderTarget->draw(this->sprites[textureId]);
}
