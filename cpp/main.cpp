#include "../hpp/libs.hpp"

class GameMap;
class GameOverScreen;
enum class GameState
{
    mainMenu,
    CutScene,
    Playing,
    GameOver,
    Win
};

// Constants for optimization
const float TICK_RATE = 60.0f;
const float TICK_TIME = 1.0f / TICK_RATE;
const float MAX_DELTA_TIME = 1.0f / 30.0f;

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "traiasca georgescu", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // Add the cutscene images here
    std::vector<std::string> cutSceneImages = {
        "../imgs/matei1.png",
        "../imgs/matei2.png",
        "../imgs/matei3.png"};

    // Initialize gameplay music
    sf::Music gameplayMusic;

    // Game state initialization
    bool gameOver = false;
    bool win = false;
    CutScene cutScene(cutSceneImages, window.getSize());
    world = std::make_unique<GameMap>("../map.mib", window, gameOver, win); // Modified to include win parameter
    GameOverScreen gameOverScreen(window);
    Win winScreen(window);
    mainMenu mainmenu(window);
    Menu settingsMenu(window, gameplayMusic);

    GameState currentState = GameState::mainMenu;
    GameState previousState = GameState::mainMenu;

    // Timing management
    sf::Clock deltaClock;
    float accumulator = 0.0f;
    bool inGameOverTransition = false;
    sf::Clock gameOverTransitionClock;
    bool inWinTransition = false;
    sf::Clock winTransitionClock;

    // Optimization flags
    bool shouldUpdate = false;
    bool isGamePaused = false;

    // Input handling improvement
    bool escapePressed = false;
    sf::Clock inputDelayClock; // Add delay between input checks

    // Main game loop
    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();
        deltaTime = std::min(deltaTime, MAX_DELTA_TIME);

        // Only accumulate time if the game is not paused
        if (!isGamePaused)
        {
            accumulator += deltaTime;
        }

        // Reset escape key state if enough time has passed
        if (inputDelayClock.getElapsedTime().asSeconds() > 0.2f)
        { // 200ms delay
            escapePressed = false;
        }

        // Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Handle keyboard input with state tracking
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::F1)
                {
                    world->toggleFullscreen();
                    if (world->playerRef->inventory)
                    {
                        world->playerRef->inventory->handleScaling();
                    }

                    // Add this line to update cutscene when toggling fullscreen
                    if (currentState == GameState::CutScene)
                    {
                        cutScene.handleResize(window.getSize());
                    }

                    winScreen.updatePositions();
                }
                else if (event.key.code == sf::Keyboard::Escape &&
                         currentState == GameState::Playing &&
                         !escapePressed)
                {
                    escapePressed = true;
                    inputDelayClock.restart();
                    settingsMenu.toggleVisibility();
                    isGamePaused = settingsMenu.isMenuVisible();
                }
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    escapePressed = false;
                }
            }
            else if (event.type == sf::Event::Resized)
            {
                world->handleResize(window.getSize());
                world->handleResize();
                settingsMenu.updateButtonPosition();

                // Add this line to update cutscene positions when window is resized
                if (currentState == GameState::CutScene)
                {
                    cutScene.handleResize(window.getSize());
                }

                // Add this line to update Win screen positions when window is resized
                winScreen.updatePositions();
            }

            // Handle menu events if visible
            if (currentState == GameState::Playing)
            {
                settingsMenu.handleEvent(event);
            }
        }

        // Direct input checking for more responsive controls
        if (currentState == GameState::Playing &&
            !escapePressed &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) &&
            inputDelayClock.getElapsedTime().asSeconds() > 0.2f)
        {
            escapePressed = true;
            inputDelayClock.restart();
            settingsMenu.toggleVisibility();
            isGamePaused = settingsMenu.isMenuVisible();
        }

        // If game is paused, reduce update frequency
        if (isGamePaused)
        {
            sf::sleep(sf::seconds(1.0f / 30.0f));
            window.clear(sf::Color::Black);
            settingsMenu.draw();
            window.display();
            continue;
        }

        // Determine if we should update based on game state
        shouldUpdate = (currentState == GameState::Playing); // Remove CutScene from fixed update

        // Fixed timestep update - only run if we should update
        if (shouldUpdate)
        {
            while (accumulator >= TICK_TIME)
            {
                if (world->isPlayerValid)
                {
                    world->playerRef->checkBounds(window.getSize());
                }
                world->updateObjects(TICK_TIME, window.getSize());
                accumulator -= TICK_TIME;
            }
        }

        // Rendering
        window.clear(sf::Color::Black);

        // State management and rendering
        switch (currentState)
        {
        case GameState::mainMenu:
            mainmenu.updateButtonColor();
            // Only handle mouse clicks during main menu
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                // Check if play button was clicked
                if (mainmenu.isPlayButtonClicked())
                {
                    world->spawnObjects();
                    gameOver = false;
                    win = false;
                    currentState = GameState::CutScene;
                    // Reset cutscene when starting from menu
                    cutScene = CutScene(cutSceneImages, window.getSize());
                }
            }
            mainmenu.draw();
            break;

        case GameState::CutScene:
            // Only update cutscene when in cutscene state
            if (!isGamePaused)
            {
                if (cutScene.update(deltaTime))
                {
                    currentState = GameState::Playing;
                    // Ensure game world is ready
                    world->updateObjects(0.0f, window.getSize()); // Initial update
                    if (gameplayMusic.getStatus() != sf::Music::Playing)
                    {
                        gameplayMusic.play();
                    }
                }
            }
            cutScene.draw(window);
            break;

        case GameState::Playing:
            // Check for win condition
            if (win && !inWinTransition)
            {
                inWinTransition = true;
                winTransitionClock.restart();
            }

            if (inWinTransition && winTransitionClock.getElapsedTime().asSeconds() >= 1.0f)
            {
                world->stopAllSounds();
                gameplayMusic.pause();
                currentState = GameState::Win;
                inWinTransition = false;
            }

            // Check for game over condition
            if (gameOver && !inGameOverTransition)
            {
                inGameOverTransition = true;
                gameOverTransitionClock.restart();
            }

            if (inGameOverTransition && gameOverTransitionClock.getElapsedTime().asSeconds() >= 2.0f)
            {
                world->stopAllSounds();
                gameplayMusic.pause();
                currentState = GameState::GameOver;
                inGameOverTransition = false;
            }

            world->drawObjects(window);

            if (settingsMenu.isMenuVisible())
            {
                settingsMenu.draw();
            }
            break;

        case GameState::GameOver:
            world->deleteObjects();
            if (gameOverScreen.handleEvent(event))
            {
                world->spawnObjects();
                gameOver = false;
                win = false;
                currentState = GameState::Playing;
                gameOverScreen.stopMusic();
            }
            if (!gameOverScreen.isPlayingMusic())
            {
                gameOverScreen.playMusic();
            }
            gameOverScreen.draw();
            break;

        case GameState::Win:
            // Make sure we're drawing something by forcing a solid color background
            window.clear(sf::Color(30, 150, 30)); // Dark green as base color

            winScreen.updateButtonColor();
            winScreen.draw();

            // Handle mouse clicks for the win screen
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                // Check if exit button was clicked
                if (winScreen.isExitButtonClicked())
                {
                    // Directly close the window instead of returning to main menu
                    window.close();
                }
            }
            break;
        }

        // Handle state transitions and music
        if (currentState != previousState)
        {
            if (previousState == GameState::GameOver)
            {
                gameOverScreen.stopMusic();
                gameplayMusic.play();
            }
            else if (currentState == GameState::GameOver)
            {
                gameplayMusic.pause();
            }
            else if (currentState == GameState::Win)
            {
                gameplayMusic.pause();
            }
            else if (currentState == GameState::Playing && previousState == GameState::CutScene)
            {
                // Don't do anything here since we handled music in the cutscene transition
                // This ensures an immediate transition
            }
            else if (currentState == GameState::Playing && previousState == GameState::mainMenu)
            {
                gameplayMusic.play();
            }
            else if (currentState == GameState::mainMenu || currentState == GameState::CutScene)
            {
                gameplayMusic.stop();
            }
            previousState = currentState;
        }

        window.display();

        // More efficient sleep calculation
        float frameTime = deltaClock.getElapsedTime().asSeconds();
        if (frameTime < TICK_TIME)
        {
            sf::sleep(sf::seconds(TICK_TIME - frameTime));
        }
    }

    return 0;
}