#include "../hpp/libs.hpp"
void sign0::loadSprite()
{
    loadTexture("../imgs/sign0.png");
}
sign0::sign0(sf::Vector2f position)
    : Sprite(position)
{
    priorityLayer = 1;
    loadSprite();
}