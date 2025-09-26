#include "../hpp/libs.hpp"

Menu::Menu(sf::RenderWindow& window, sf::Music& gameplayMusic) 
    : window(window), gameplayMusic(gameplayMusic), isVisible(false), currentMusicIndex(0),
      isDraggingSlider(false), currentVolume(1.0f) {
    // Load available music files
    musicFiles = {
        "../audio/marijuana cocaina eroina crack.wav",
        "../audio/FoochTrax - Rat King.wav",
        "../audio/SS Marschiert in Feindesland.wav",
        "../audio/heiakim - selling ice cream.wav",
        "../audio/Bing Chilling.wav",
        "../audio/GOOFY'S TRIAL.wav",
        "../audio/Intr-o nu stiu care seara.wav",
        "../audio/Shadow wizard money gang.wav",
        "../audio/(Remix).wav",
        "../audio/Korn - Twist.wav"
    };

    // Load font
    if (!font.loadFromFile("../fonts/ARIAL.TTF")) {
        // Handle font loading error
    }

    // Initialize title text
    titleText.setFont(font);
    titleText.setString("SETTINGS");
    titleText.setFillColor(sf::Color::White);

    // Initialize existing components
    background.setFillColor(sf::Color(0, 0, 0, 200));
    
    settingText.setFont(font);
    settingText.setString("Music:");
    settingText.setFillColor(sf::Color::White);
    
    musicButton.setFillColor(sf::Color::White);
    
    musicButtonText.setFont(font);
    musicButtonText.setString(musicFiles[currentMusicIndex].substr(musicFiles[currentMusicIndex].find_last_of('/') + 1));
    musicButtonText.setFillColor(sf::Color::Black);

    // Initialize volume control components
    volumeText.setFont(font);
    volumeText.setString("Volume:");
    volumeText.setFillColor(sf::Color::White);

    sliderTrack.setFillColor(sf::Color(150, 150, 150));
    
    sliderHandle.setFillColor(sf::Color::White);
    sliderHandle.setPointCount(30);

    volumePercentage.setFont(font);
    volumePercentage.setFillColor(sf::Color::White);

    // Initialize the first music track
    if (gameplayMusic.openFromFile(musicFiles[currentMusicIndex])) {
        gameplayMusic.setLoop(true);
        gameplayMusic.setVolume(currentVolume * 100);
    }

    updateButtonPosition();
}

void Menu::updateButtonPosition() {
    // Get current window dimensions
    sf::Vector2u windowSize = window.getSize();
    float windowWidth = static_cast<float>(windowSize.x);
    float windowHeight = static_cast<float>(windowSize.y);

    // Calculate relative sizes
    float backgroundWidth = windowWidth * 0.6f;
    float backgroundHeight = windowHeight * 0.4f;
    background.setSize(sf::Vector2f(backgroundWidth, backgroundHeight));

    float baseTextSize = windowHeight * 0.03f;
    titleText.setCharacterSize(static_cast<unsigned int>(baseTextSize * 1.2f));  // Title slightly larger
    settingText.setCharacterSize(static_cast<unsigned int>(baseTextSize));
    musicButtonText.setCharacterSize(static_cast<unsigned int>(baseTextSize * 0.9f));
    volumeText.setCharacterSize(static_cast<unsigned int>(baseTextSize));
    volumePercentage.setCharacterSize(static_cast<unsigned int>(baseTextSize * 0.8f));

    float buttonWidth = backgroundWidth * 0.7f;
    float buttonHeight = backgroundHeight * 0.15f;
    musicButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));

    // Position background
    background.setPosition(
        (windowWidth - backgroundWidth) / 2.0f,
        (windowHeight - backgroundHeight) / 2.0f
    );

    // Position title text
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        background.getPosition().x + (backgroundWidth - titleBounds.width) / 2.0f,
        background.getPosition().y + backgroundHeight * 0.05f  // Small top padding
    );

    float textPadding = backgroundWidth * 0.03f;
    
    // Position music controls (adjusted down to make room for title)
    settingText.setPosition(
        background.getPosition().x + textPadding,
        titleText.getPosition().y + titleText.getLocalBounds().height + backgroundHeight * 0.15f
    );

    musicButton.setPosition(
        background.getPosition().x + (backgroundWidth - buttonWidth) / 2.0f,
        settingText.getPosition().y - buttonHeight * 0.1f
    );

    sf::FloatRect textBounds = musicButtonText.getLocalBounds();
    musicButtonText.setPosition(
        musicButton.getPosition().x + (buttonWidth - textBounds.width) / 2.0f,
        musicButton.getPosition().y + (buttonHeight - textBounds.height) / 2.0f
    );

    // Position volume controls
    volumeText.setPosition(
        background.getPosition().x + textPadding,
        musicButton.getPosition().y + buttonHeight + backgroundHeight * 0.1f
    );

    // Slider track
    float sliderWidth = buttonWidth * 0.8f;
    float sliderHeight = buttonHeight * 0.1f;
    sliderTrack.setSize(sf::Vector2f(sliderWidth, sliderHeight));
    sliderTrack.setPosition(
        background.getPosition().x + (backgroundWidth - sliderWidth) / 2.0f,
        volumeText.getPosition().y + volumeText.getLocalBounds().height + backgroundHeight * 0.05f
    );

    // Slider handle
    float handleRadius = sliderHeight * 2.0f;
    sliderHandle.setRadius(handleRadius);
    float handleX = sliderTrack.getPosition().x + (sliderTrack.getSize().x * currentVolume) - handleRadius;
    float handleY = sliderTrack.getPosition().y + (sliderTrack.getSize().y / 2.0f) - handleRadius;
    sliderHandle.setPosition(handleX, handleY);

    // Volume percentage text
    volumePercentage.setString(std::to_string(static_cast<int>(currentVolume * 100)) + "%");
    volumePercentage.setPosition(
        sliderTrack.getPosition().x + sliderTrack.getSize().x + textPadding,
        sliderTrack.getPosition().y - sliderHeight
    );
}
void Menu::toggleVisibility() {
    isVisible = !isVisible;
    updateButtonPosition();
}
void Menu::updateVolumeFromSlider() {
    float sliderLeft = sliderTrack.getPosition().x;
    float sliderRight = sliderLeft + sliderTrack.getSize().x;
    float handleX = sliderHandle.getPosition().x + sliderHandle.getRadius();
    
    currentVolume = (handleX - sliderLeft) / (sliderRight - sliderLeft);
    currentVolume = std::max(0.0f, std::min(1.0f, currentVolume));
    
    gameplayMusic.setVolume(currentVolume * 100);
    volumePercentage.setString(std::to_string(static_cast<int>(currentVolume * 100)) + "%");
}

void Menu::handleEvent(const sf::Event& event) {
    if (!isVisible) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    
    // Handle music button
    bool isHoveringButton = musicButton.getGlobalBounds().contains(mousePos.x, mousePos.y);
    musicButton.setFillColor(isHoveringButton ? sf::Color(200, 200, 200) : sf::Color::White);

    // Handle slider
    sf::FloatRect handleBounds = sliderHandle.getGlobalBounds();
    bool isHoveringHandle = handleBounds.contains(mousePos.x, mousePos.y);
    sliderHandle.setFillColor(isHoveringHandle || isDraggingSlider ? sf::Color(200, 200, 200) : sf::Color::White);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (isHoveringButton) {
            currentMusicIndex = (currentMusicIndex + 1) % musicFiles.size();
            musicButtonText.setString(musicFiles[currentMusicIndex].substr(musicFiles[currentMusicIndex].find_last_of('/') + 1));
            
            gameplayMusic.stop();
            if (gameplayMusic.openFromFile(musicFiles[currentMusicIndex])) {
                gameplayMusic.play();
                gameplayMusic.setLoop(true);
                gameplayMusic.setVolume(currentVolume * 100);
            }
        } else if (isHoveringHandle) {
            isDraggingSlider = true;
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDraggingSlider = false;
    } else if (event.type == sf::Event::MouseMoved && isDraggingSlider) {
        float newX = mousePos.x - sliderHandle.getRadius();
        float minX = sliderTrack.getPosition().x - sliderHandle.getRadius();
        float maxX = sliderTrack.getPosition().x + sliderTrack.getSize().x - sliderHandle.getRadius();
        
        newX = std::max(minX, std::min(maxX, newX));
        sliderHandle.setPosition(newX, sliderHandle.getPosition().y);
        
        updateVolumeFromSlider();
    }
}

void Menu::draw() {
    if (!isVisible) return;

    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    window.draw(background);
    window.draw(titleText);
    window.draw(settingText);
    window.draw(musicButton);
    window.draw(musicButtonText);
    window.draw(volumeText);
    window.draw(sliderTrack);
    window.draw(sliderHandle);
    window.draw(volumePercentage);

    window.setView(originalView);
}