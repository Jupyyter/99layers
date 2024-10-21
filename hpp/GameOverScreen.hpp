#pragma once

class GameOverScreen {
private:
    sf::RenderWindow& windowr;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text returnToMenuText;
    sf::RectangleShape returnToMenuButton;
    sf::Music gameOverMusic;  // Add this line
    
    void initializeText();
    void initializeButton();
    
public:
    GameOverScreen(sf::RenderWindow& window);
    ~GameOverScreen();  // Add destructor
    void draw();
    bool handleEvent(const sf::Event& event);
    void playMusic();
    void stopMusic();
    bool isPlayingMusic();
};