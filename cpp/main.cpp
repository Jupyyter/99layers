#include "../hpp/libs.hpp"
class GameMap;
enum class GameState
{
    Menu,
    CutScene,
    Playing,
    GameOver
};

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML Player", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    std::vector<std::string> cutSceneImages = {
        // Add your cutscene images here
    };
    bool gameOver = false;
    CutScene cutScene(cutSceneImages, window.getSize());
    world = std::make_unique<GameMap>("../map.mib",window, gameOver);
    GameOverScreen gameOverScreen(window);
    Menu menu(window);
    GameState currentState = GameState::Menu;

    sf::Clock frameClock;
    sf::Clock updateClock;
    const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    const float maxDeltaTime = 1.0f / 30.0f; // Maximum allowed delta time

    while (window.isOpen())
    {
        sf::Time frameTime = frameClock.restart();
        float deltaTime = std::min(frameTime.asSeconds(), maxDeltaTime); // Cap delta time
        //This ensures that even if there's a long pause (like when moving the window), the game won't try to simulate that entire duration in one step.

        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Handle other events based on current state
            switch (currentState)
            {
                case GameState::Menu:
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && menu.isPlayButtonClicked())
                    {
                        world->resetEntities();
                        gameOver = false;
                        currentState = GameState::CutScene;
                        updateClock.restart();
                    }
                    break;
                case GameState::GameOver:
                    if (gameOverScreen.handleEvent(event))
                    {
                        currentState = GameState::Menu;
                    }
                    break;
            }
        }

        // Update logic based on current state
        switch (currentState)
        {
            case GameState::Menu:
                menu.updateButtonColor();
                break;
            case GameState::CutScene:
                if (cutScene.update(deltaTime))
                {
                    currentState = GameState::Playing;
                }
                break;
            case GameState::Playing:
                if (gameOver)
                {
                    currentState = GameState::GameOver;
                    gameOver = false;
                }
                else
                {
                    world->updateEntities(deltaTime, window.getSize());
                    world->removeDeadEntities();
                }
                break;
        }

        // Render
        window.clear(sf::Color::Black);

        switch (currentState)
        {
            case GameState::Menu:
                menu.draw();
                break;
            case GameState::CutScene:
                cutScene.draw(window);
                break;
            case GameState::Playing:
                {
                   // map.draw();
                    world->drawEntities(window);
                }
                break;
            case GameState::GameOver:
                gameOverScreen.draw();
                break;
        }

        window.display();

        // Frame rate control
        sf::Time sleepTime = timePerFrame - frameClock.getElapsedTime();
        //if (sleepTime > sf::Time::Zero)
        std::this_thread::sleep_for(std::chrono::microseconds(sleepTime.asMicroseconds()));
    }

    return 0;
}