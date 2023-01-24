#include "Game.hpp"


// Constructor
Game::Game()
{
    this->initWindow();
    this->initVariables();
}

// Destructor
Game::~Game()
{    

}

// Private functions
void Game::initWindow()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    sf::VideoMode videoMode;
    videoMode.width = SCREEN_WIDTH;
    videoMode.height = SCREEN_HEIGHT;
    
    this->window = std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow(videoMode, "Game", sf::Style::Titlebar | sf::Style::Close, settings));

    this->window->setVerticalSyncEnabled(false);
    this->window->setFramerateLimit(60);
}

void Game::initVariables()
{
    this->drawMap = true;
    // this->drawMap = false;
    
    this->player = std::shared_ptr<Player>(new Player());
    this->mapManager = std::shared_ptr<MapManager>(new MapManager());

    this->rayCastingEngine = std::unique_ptr<RayCasting>(new RayCasting(drawMap=this->drawMap));

    this->mapManager->loadMap(this->player);
}

void Game::pollEvents()
{
    while (this->window->pollEvent(this->events))
    {
        if (this->events.type == sf::Event::Closed || (this->events.KeyPressed && this->events.key.code == sf::Keyboard::Escape))
        {
            this->window->close();
        }
    }
}

void Game::updateDeltaTime()
{
    this->window->setTitle("Game - " + std::to_string(static_cast<int>(1000000 / this->clock.restart().asMicroseconds())) + " fps");
}

void Game::moveCamera()
{
    sf::View view = this->window->getView();
    view.setCenter(this->player->getCenterPos());
    this->window->setView(view);
}

void Game::drawMiniMap()
{
    // sf::View miniMap = this->window->getDefaultView();
    // miniMap.setViewport(sf::FloatRect(0.f, 0.f, 0.2f, 0.2f));
    // this->window->setView(miniMap);
    
    this->rayCastingEngine->renderFovVisualisation(this->window);
    this->player->render(this->window);
    this->mapManager->render(this->window);

    // this->window->setView(this->window->getDefaultView());
}

// Accesors 
const bool Game::isRunning() const
{
    return this->window->isOpen();
}

// Public functions
void Game::update()
{
    this->updateDeltaTime();
    this->pollEvents();

    if (this->isRunning())
    {
        this->player->update(this->mapManager);
        this->rayCastingEngine->update(this->mapManager, this->player->getCenterPos(), this->player->getAngle());
    }
}

void Game::render()
{
    if (this->isRunning())
    {
        this->window->clear(sf::Color(20, 20, 20));

        if (this->drawMap)
        {
            this->moveCamera();
            this->drawMiniMap();
        }
        else
        {
            this->rayCastingEngine->render(this->window);
        }

        this->window->display();
    }
}   