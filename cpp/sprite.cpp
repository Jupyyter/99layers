#include "../hpp/libs.hpp"

Sprite::Sprite(const sf::Vector2f& position) : Object(position), flipped(true), invisible(false) {updateSprite();}

void Sprite::loadTexture(const std::string &filename)
{
    if (!texture.loadFromFile(filename))
    {
        // Handle error
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
    sprite.setTexture(texture);
}

void Sprite::update(float deltaTime, const sf::Vector2u &screenres)
{
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
void Sprite::updateSprite()
{
    sprite.setPosition(position);
}

sf::FloatRect Sprite::getBounds() const {
    return this->sprite.getGlobalBounds();
}
const sf::Sprite Sprite::getSprite()const{
    return sprite;
}
bool Sprite::isOnScreen() const {
    return world->getPartBounds().intersects(this->getBounds());
}