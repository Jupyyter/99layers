#include "../hpp/libs.hpp"

Sprite::Sprite() : Entity(), flipped(true), invisible(false) {}

void Sprite::loadTexture(const std::string &filename)
{
    if (!texture.loadFromFile(filename))
    {
        // Handle error
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
    sprite.setTexture(texture);
}

void Sprite::update(float deltaTime, GameMap& gamemap, const sf::Vector2u &screenres)
{
    // Basic update for sprite (e.g., updating position based on velocity)
    // position += velocity * deltaTime;
    // sprite.setPosition(position);
}
void Sprite::scale(float scaleX, float scaleY)
{
    sprite.setScale(scaleX, scaleY);
}
void Sprite::draw(sf::RenderWindow &window)const 
{
    if (!invisible)
    {
        window.draw(sprite);
    }
}
void Sprite::setPosition(const sf::Vector2f &pos)
{
    position = pos;
    sprite.setPosition(position);
}

void Sprite::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
}
sf::FloatRect Sprite::getBounds() const {
    return this->sprite.getGlobalBounds();
}