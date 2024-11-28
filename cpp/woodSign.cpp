#include "../hpp/libs.hpp"
void woodSign::loadSprite()
{
    loadTexture("../imgs/woodSign.png");
}
woodSign::woodSign(sf::Vector2f position)
    : Npc(position,0.1,"../imgs/woodSign.png")
{
    priorityLayer = 1;
    loadSprite();
}