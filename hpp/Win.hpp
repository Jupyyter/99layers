#pragma once
class Win
{
private:
    sf::RenderWindow& window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::RectangleShape button;
    sf::Font font;
    sf::Text buttonText;
    bool isButtonHovered;

public:
    Win(sf::RenderWindow& window);
    void draw();
    void updateButtonColor();
    bool isExitButtonClicked();
    void updatePositions();
};