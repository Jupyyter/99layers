#include "../hpp/libs.hpp"

sign0::sign0(sf::Vector2f position)
    : Sprite(position)
{
    priorityLayer = 1;
    loadTexture("../imgs/sign0.png");
}
sign1::sign1(sf::Vector2f position)
    : Sprite(position)
{
    priorityLayer = 1;
    loadTexture("../imgs/sign1.png");
}