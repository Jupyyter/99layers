#pragma once
class GameOverScreen {
private:
    sf::RenderWindow& windowr;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text returnToMenuText;
    sf::RectangleShape returnToMenuButton;
    sf::Music gameOverMusic;
    bool isButtonHovered;  // New variable to track hover state
    
    void initializeText();
    void initializeButton();
    void updateButtonColor();  // New helper function
    
public:
    GameOverScreen(sf::RenderWindow& window);
    ~GameOverScreen();
    void draw();
    bool handleEvent(const sf::Event& event);
    void playMusic();
    void stopMusic();
    bool isPlayingMusic();
};