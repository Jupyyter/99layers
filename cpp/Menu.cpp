#include "../hpp/libs.hpp"
Menu::Menu(sf::RenderWindow& window, sf::Music& gameplayMusic) 
    : window(window), gameplayMusic(gameplayMusic), isVisible(false), currentMusicIndex(0) {
    // Load available music files
    musicFiles = {
        "../audio/marijuana cocaina eroina crack.wav",
        "../audio/FoochTrax - Rat King.wav"
        // Add more music files as needed
    };

    // Load font
    if (!font.loadFromFile("../fonts/ARIAL.TTF")) {
        // Handle font loading error
    }

    // Initialize background
    background.setSize(sf::Vector2f(700, 300));
    background.setFillColor(sf::Color(0, 0, 0, 200));

    // Initialize setting text
    settingText.setFont(font);
    settingText.setString("Music:");
    settingText.setCharacterSize(24);
    settingText.setFillColor(sf::Color::White);

    // Initialize music button
    musicButton.setSize(sf::Vector2f(500, 40));
    musicButton.setFillColor(sf::Color::White);

    // Initialize music button text
    musicButtonText.setFont(font);
    musicButtonText.setString(musicFiles[currentMusicIndex].substr(musicFiles[currentMusicIndex].find_last_of('/') + 1));
    musicButtonText.setCharacterSize(20);
    musicButtonText.setFillColor(sf::Color::Black);

    updateButtonPosition();
}

void Menu::toggleVisibility() {
    isVisible = !isVisible;
    updateButtonPosition();
}

void Menu::updateButtonPosition() {
    // Center the menu
    background.setPosition(
        (window.getSize().x - background.getSize().x) / 2,
        (window.getSize().y - background.getSize().y) / 2
    );

    // Position setting text
    settingText.setPosition(
        background.getPosition().x + 20,
        background.getPosition().y + 50
    );

    // Position music button
    musicButton.setPosition(
        background.getPosition().x + 150,
        background.getPosition().y + 45
    );

    // Position music button text
    musicButtonText.setPosition(
        musicButton.getPosition().x + 10,
        musicButton.getPosition().y + 5
    );
}

void Menu::handleEvent(const sf::Event& event) {
    if (!isVisible) return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (musicButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            // Change to next music file
            currentMusicIndex = (currentMusicIndex + 1) % musicFiles.size();
            musicButtonText.setString(musicFiles[currentMusicIndex].substr(musicFiles[currentMusicIndex].find_last_of('/') + 1));
            
            // Stop current music and play new selection
            gameplayMusic.stop();
            if (gameplayMusic.openFromFile(musicFiles[currentMusicIndex])) {
                gameplayMusic.play();
                gameplayMusic.setLoop(true);
            }
        }
    }
}

void Menu::draw() {
    if (!isVisible) return;

    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    window.draw(background);
    window.draw(settingText);
    window.draw(musicButton);
    window.draw(musicButtonText);

    window.setView(originalView);
}