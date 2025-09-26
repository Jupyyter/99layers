#include "../hpp/libs.hpp"

GameOverScreen::GameOverScreen(sf::RenderWindow & window) : windowr(window), isButtonHovered(false) {
    if (!font.loadFromFile("../fonts//ARIAL.TTF")) {
        // Handle font loading error
    }
    
    if (!gameOverMusic.openFromFile("../audio//gameOverMusic.wav")) {
        // Handle music loading error
    }
    stopMusic();
    initializeText();
    initializeButton();
}

GameOverScreen::~GameOverScreen() {
    gameOverMusic.stop();
}

void GameOverScreen::initializeText() {
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);
    
    returnToMenuText.setFont(font);
    returnToMenuText.setString("die again later");
    returnToMenuText.setCharacterSize(30);
    returnToMenuText.setFillColor(sf::Color::Black);
}

void GameOverScreen::initializeButton() {
    returnToMenuButton.setSize(sf::Vector2f(270, 50));
    returnToMenuButton.setFillColor(sf::Color::White);
}

void GameOverScreen::updateButtonColor() {
    returnToMenuButton.setFillColor(isButtonHovered ? sf::Color::Red : sf::Color::White);
}

void GameOverScreen::draw() {
    sf::View originalView = windowr.getView();
    windowr.setView(windowr.getDefaultView());
    
    // Update button hover state
    sf::Vector2i mousePos = sf::Mouse::getPosition(windowr);
    isButtonHovered = returnToMenuButton.getGlobalBounds().contains(mousePos.x, mousePos.y);
    updateButtonColor();
    
    gameOverText.setPosition(
        windowr.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2,
        windowr.getSize().y / 3 - gameOverText.getGlobalBounds().height / 2
    );
    
    returnToMenuButton.setPosition(
        windowr.getSize().x / 2 - returnToMenuButton.getSize().x / 2,
        windowr.getSize().y * 2 / 3 - returnToMenuButton.getSize().y / 2
    );
    
    returnToMenuText.setPosition(
        returnToMenuButton.getPosition().x + (returnToMenuButton.getSize().x - returnToMenuText.getGlobalBounds().width) / 2,
        returnToMenuButton.getPosition().y + (returnToMenuButton.getSize().y - returnToMenuText.getGlobalBounds().height) / 2 - 5
    );
    
    windowr.draw(gameOverText);
    windowr.draw(returnToMenuButton);
    windowr.draw(returnToMenuText);
    
    windowr.setView(originalView);
}

bool GameOverScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(windowr);
        sf::Vector2f worldPos = windowr.mapPixelToCoords(mousePosition);
        
        if (returnToMenuButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            gameOverMusic.stop();
            return true;
        }
    }
    
    return false;
}

void GameOverScreen::playMusic() {
    gameOverMusic.setLoop(true);
    gameOverMusic.play();
}

void GameOverScreen::stopMusic() {
    gameOverMusic.stop();
}

bool GameOverScreen::isPlayingMusic() {
    return gameOverMusic.getStatus() == sf::Music::Playing;
}