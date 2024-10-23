#include "../hpp/libs.hpp"
class GameMap;
class GameOverScreen;
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
    world = std::make_unique<GameMap>("../map.mib", window, gameOver);
    GameOverScreen gameOverScreen(window);
    Menu menu(window);
    GameState currentState = GameState::Menu;
    GameState previousState = GameState::Menu;  // Track previous state for transitions

    sf::Clock frameClock;
    sf::Clock updateClock;
    const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    const float maxDeltaTime = 1.0f / 30.0f;

    while (window.isOpen())
    {
        sf::Time frameTime = frameClock.restart();
        float deltaTime = std::min(frameTime.asSeconds(), maxDeltaTime);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        // Handle state transitions and music
        if (currentState != previousState)
        {
            if (previousState == GameState::GameOver)
            {
                gameOverScreen.stopMusic();
            }
            previousState = currentState;
        }

        switch (currentState)
        {
            case GameState::Menu:
                menu.updateButtonColor();
                if (event.type == sf::Event::MouseButtonPressed && 
                    event.mouseButton.button == sf::Mouse::Left && 
                    menu.isPlayButtonClicked())
                {
                    world->reset();
                    gameOver = false;
                    currentState = GameState::CutScene;
                    updateClock.restart();
                }
                menu.draw();
                break;

            case GameState::CutScene:
                if (cutScene.update(deltaTime))
                {
                    currentState = GameState::Playing;
                }
                cutScene.draw(window);
                break;

            case GameState::Playing:
                if (gameOver)
                {
                    currentState = GameState::GameOver;
                    gameOver = true;
                    // Pause the game for 1 second before going to the game over screen
                    sf::Clock pauseClock;
                    while (pauseClock.getElapsedTime().asSeconds() < 2.0f)
                    {
                        window.clear(sf::Color::Black);
                        world->drawObjects(window);
                        window.display();
                    }
                }
                else
                {
                    world->updateObjects(deltaTime, window.getSize());
                    world->removeDeadObjects();
                    world->drawObjects(window);
                }
                break;

            case GameState::GameOver:
                if (gameOverScreen.handleEvent(event))
                {
                    currentState = GameState::Menu;
                }
                if (!gameOverScreen.isPlayingMusic())
                {
                    gameOverScreen.playMusic();
                }
                gameOverScreen.draw();
                break;
        }

        window.display();

        sf::Time sleepTime = timePerFrame - frameClock.getElapsedTime();
        std::this_thread::sleep_for(std::chrono::microseconds(sleepTime.asMicroseconds()));
    }

    return 0;
}