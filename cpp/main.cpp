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
    sf::RenderWindow window(sf::VideoMode(1024, 768), "traiasca georgescu", sf::Style::Titlebar | sf::Style::Close);
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
    sf::Clock gameOverTransitionClock;  // New clock for game over transition
    bool inGameOverTransition = false;  // Flag to track if we're in transition
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
                    world->spawnObjects();
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
                if (gameOver && !inGameOverTransition)
                {
                    // Start the transition period
                    inGameOverTransition = true;
                    gameOverTransitionClock.restart();
                }

                if (inGameOverTransition && gameOverTransitionClock.getElapsedTime().asSeconds() >= 2.0f)
                {
                    // Transition period is over, move to game over state
                    world->stopAllSounds();
                    currentState = GameState::GameOver;
                    inGameOverTransition = false;
                }

                // Continue updating and drawing the game
                if(world->isPlayerValid){
                    world->playerRef->checkBounds( window.getSize()); // this is literally a skill issue
                }
                world->updateObjects(deltaTime, window.getSize());
                world->removeDeadObjects();
                world->drawObjects(window);
                break;

            case GameState::GameOver:
                if (gameOverScreen.handleEvent(event))
                {
                    currentState = GameState::Menu;
                }
                if (!gameOverScreen.isPlayingMusic())
                {
                    gameOverScreen.playMusic();
                    world->deleteObjects();
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