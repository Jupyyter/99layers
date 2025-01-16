#include "../hpp/libs.hpp"
class GameMap;
class GameOverScreen;
enum class GameState {
    mainMenu,
    CutScene,
    Playing,
    GameOver
};

int main() {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "traiasca georgescu", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    std::vector<std::string> cutSceneImages = {
        // Add your cutscene images here
    };
    
    // Initialize gameplay music
    sf::Music gameplayMusic;
    if (gameplayMusic.openFromFile("../audio/gameplay1.wav")) {
        gameplayMusic.setLoop(true);
    }

    bool gameOver = false;
    CutScene cutScene(cutSceneImages, window.getSize());
    world = std::make_unique<GameMap>("../map.mib", window, gameOver);
    GameOverScreen gameOverScreen(window);
    mainMenu mainmenu(window);
    Menu settingsMenu(window, gameplayMusic);
    
    GameState currentState = GameState::mainMenu;
    GameState previousState = GameState::mainMenu;

    sf::Clock frameClock;
    sf::Clock updateClock;
    sf::Clock gameOverTransitionClock;
    bool inGameOverTransition = false;
    const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    const float maxDeltaTime = 1.0f / 30.0f;

    while (window.isOpen()) {
        sf::Time frameTime = frameClock.restart();
        float deltaTime = std::min(frameTime.asSeconds(), maxDeltaTime);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F1) {
                    world->toggleFullscreen();
                    if (world->playerRef->inventory) {
                        world->playerRef->inventory->handleScaling();
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape && 
                         currentState == GameState::Playing) {
                    settingsMenu.toggleVisibility();
                }
            }
            else if (event.type == sf::Event::Resized) {
                world->handleResize(window.getSize());
                world->handleResize();
                settingsMenu.updateButtonPosition();
            }

            // Handle menu events if visible
            if (currentState == GameState::Playing) {
                settingsMenu.handleEvent(event);
            }
        }

        window.clear(sf::Color::Black);

        if (currentState != previousState) {
            if (previousState == GameState::GameOver) {
                gameOverScreen.stopMusic();
            }
            if (currentState == GameState::Playing) {
                gameplayMusic.play();
            } else {
                gameplayMusic.stop();
            }
            previousState = currentState;
        }

        switch (currentState) {
        case GameState::mainMenu:
            mainmenu.updateButtonColor();
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left &&
                mainmenu.isPlayButtonClicked()) {
                world->spawnObjects();
                gameOver = false;
                currentState = GameState::CutScene;
                updateClock.restart();
            }
            mainmenu.draw();
            break;

        case GameState::CutScene:
            if (cutScene.update(deltaTime)) {
                currentState = GameState::Playing;
            }
            cutScene.draw(window);
            break;

        case GameState::Playing:
            if (gameOver && !inGameOverTransition) {
                inGameOverTransition = true;
                gameOverTransitionClock.restart();
            }

            if (inGameOverTransition && gameOverTransitionClock.getElapsedTime().asSeconds() >= 2.0f) {
                world->stopAllSounds();
                gameplayMusic.stop();
                currentState = GameState::GameOver;
                inGameOverTransition = false;
            }

            if (world->isPlayerValid) {
                world->playerRef->checkBounds(window.getSize());
            }
            world->updateObjects(deltaTime, window.getSize());
            world->removeDeadObjects();
            world->drawObjects(window);
            
            // Draw menu if visible
            if (settingsMenu.isMenuVisible()) {
                settingsMenu.draw();
            }
            break;

        case GameState::GameOver:
            world->deleteObjects();
            if (gameOverScreen.handleEvent(event)) {
                world->spawnObjects();
                gameOver = false;
                currentState = GameState::Playing;
                gameOverScreen.stopMusic();
            }
            if (!gameOverScreen.isPlayingMusic()) {
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