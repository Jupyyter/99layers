#include "../hpp/libs.hpp"
Background::Background(float x, float y, float width, float height, const std::string& texturePath)
    : Sprite(sf::Vector2f(x, y))
    , originalWidth(width)
    , originalHeight(height)
{
    loadTexture(texturePath);
    initializeSprite(width, height);
}

void Background::initializeSprite(float width, float height)
{
    priorityLayer = -1000; // Ensure background is drawn behind other object
    if (texture.getSize().x > 0 && texture.getSize().y > 0)
    {
        float scaleX = width / texture.getSize().x;
        float scaleY = height / texture.getSize().y;
        sprite.setScale(scaleX, scaleY);
    }
    //setPosition(getPosition()); // Ensure position is properly set
}

void Background::update(float deltaTime, const sf::Vector2u& windowSize)
{
    Sprite::update(deltaTime, windowSize);
}

void Background::draw(sf::RenderWindow& window) const
{
    if (!invisible)
    {
        window.draw(sprite);
    }
}

bool Background::isOnScreen() const
{
    return true; // Backgrounds are always considered on screen, or implement your own logic
}