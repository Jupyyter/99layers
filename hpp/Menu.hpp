#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(sf::RenderWindow& window);
    void draw();
    bool handleEvent(const sf::Event& event);
    bool isPlayButtonClicked();
    void updateButtonColor();
    
private:
    sf::RenderWindow& windowr;
    sf::RectangleShape playButton;
    sf::Text playText;
    sf::Font font;
    
    void initializeText();
    void initializeButton();
};